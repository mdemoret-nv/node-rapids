// Copyright (c) 2020, NVIDIA CORPORATION.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "cpp_to_napi.hpp"
#include "napi_to_cpp.hpp"

#include <napi.h>

namespace nv {

struct CPPToNapiValues {
  CPPToNapiValues(Napi::Env const& env) : env_(env) {}

  inline Napi::Env Env() const { return env_; }
  inline operator Napi::Env() const { return Env(); }

  template <typename... Ts>
  inline std::vector<napi_value> operator()(Ts&&... args) const {
    CPPToNapi cast_t{env_};
    std::vector<napi_value> napi_values{};
    napi_values.reserve(sizeof...(Ts));
    nv::casting::for_each(std::make_tuple<Ts...>(std::forward<Ts>(args)...),
                          [&](auto x) { napi_values.push_back(cast_t(x)); });
    return napi_values;
  }

 private:
  Napi::Env env_;
};

struct CallbackArgs {
  // Constructor that accepts the same arguments as the Napi::CallbackInfo constructor
  CallbackArgs(napi_env env, napi_callback_info info)
    : CallbackArgs(new Napi::CallbackInfo(env, info), true) {}

  // Construct a CallbackArgs by proxying to an Napi::CallbackInfo instance
  CallbackArgs(Napi::CallbackInfo const* info, bool owns_info = false)
    : owns_info_(owns_info), info_(info){};

  // Construct a CallbackArgs by proxying to an Napi::CallbackInfo instance
  CallbackArgs(Napi::CallbackInfo const& info) : info_(&info){};

  ~CallbackArgs() {
    if (owns_info_) { delete info_; }
    info_ = nullptr;
  }

  // Proxy all the public methods
  Napi::Env Env() const { return info_->Env(); }
  Napi::Value NewTarget() const { return info_->NewTarget(); }
  bool IsConstructCall() const { return info_->IsConstructCall(); }
  size_t Length() const { return info_->Length(); }
  Napi::Value This() const { return info_->This(); }
  void* Data() const { return info_->Data(); }
  void SetData(void* data) { const_cast<Napi::CallbackInfo*>(info_)->SetData(data); }

  // the [] operator returns instances with implicit conversion operators from JS to C++ types
  NapiToCPP const operator[](size_t i) const { return info_->operator[](i); }

  inline Napi::CallbackInfo const& info() const { return *info_; }
  inline operator Napi::CallbackInfo const &() const { return *info_; }

 private:
  bool owns_info_{false};
  Napi::CallbackInfo const* info_{nullptr};
};

struct ConstructorReference : public Napi::FunctionReference {
  inline static ConstructorReference Persistent(Napi::Function const& value) {
    return Napi::Reference<Napi::Function>::New(value, 1);
  }

  inline ConstructorReference() : Napi::FunctionReference() {}
  inline ConstructorReference(napi_env env, napi_ref ref) : Napi::FunctionReference(env, ref) {}

  // A reference can be moved but cannot be copied.
  inline ConstructorReference(Napi::Reference<Napi::Function>&& other)
    : Napi::FunctionReference(std::move(other)){};
  inline ConstructorReference& operator=(Napi::Reference<Napi::Function>&& other) {
    static_cast<Napi::Reference<Napi::Function>*>(this)->operator=(std::move(other));
    return *this;
  }
  inline ConstructorReference(ConstructorReference&& other)
    : Napi::FunctionReference(std::move(other)){};
  inline ConstructorReference& operator=(ConstructorReference&& other) {
    static_cast<Napi::Reference<Napi::Function>*>(this)->operator=(std::move(other));
    return *this;
  }

  template <typename... Args>
  inline Napi::Object New(Args&&... xs) const {
    return Napi::FunctionReference::New(CPPToNapiValues{_env}(std::forward<Args>(xs)...));
  }
};

}  // namespace nv
