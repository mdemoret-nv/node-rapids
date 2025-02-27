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

#include <node_cugraph/addon.hpp>
#include <node_cugraph/graph_coo.hpp>

#include <nv_node/macros.hpp>

#include <napi.h>

namespace nv {
Napi::Value cugraphInit(Napi::CallbackInfo const& info) {
  // todo
  return info.This();
}
}  // namespace nv

Napi::Object initModule(Napi::Env env, Napi::Object exports) {
  EXPORT_FUNC(env, exports, "init", nv::cugraphInit);
  nv::GraphCOO::Init(env, exports);
  return exports;
}

NODE_API_MODULE(node_cugraph, initModule);
