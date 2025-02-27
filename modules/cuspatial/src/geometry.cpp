// Copyright (c) 2021, NVIDIA CORPORATION.
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

#include <node_cuspatial/geometry.hpp>

#include <node_cudf/column.hpp>
#include <node_cudf/table.hpp>

#include <node_rmm/utilities/napi_to_cpp.hpp>

#include <cuspatial/error.hpp>
#include <cuspatial/polygon_bounding_box.hpp>
#include <cuspatial/polyline_bounding_box.hpp>

#include <nv_node/utilities/args.hpp>
#include <nv_node/utilities/wrap.hpp>

namespace nv {

Napi::Value compute_polygon_bounding_boxes(CallbackArgs const& args) {
  auto poly_offsets                   = Column::Unwrap(args[0]);
  auto ring_offsets                   = Column::Unwrap(args[1]);
  auto point_x                        = Column::Unwrap(args[2]);
  auto point_y                        = Column::Unwrap(args[3]);
  rmm::mr::device_memory_resource* mr = args[4];
  auto result                         = [&]() {
    try {
      return cuspatial::polygon_bounding_boxes(
        *poly_offsets, *ring_offsets, *point_x, *point_y, mr);
    } catch (cuspatial::logic_error const& err) { throw Napi::Error::New(args.Env(), err.what()); }
  }();
  auto output = Napi::Object::New(args.Env());
  auto names  = Napi::Array::New(args.Env(), 4);
  names.Set(0u, "x_min");
  names.Set(1u, "y_min");
  names.Set(2u, "x_max");
  names.Set(3u, "y_max");
  output.Set("names", names);
  output.Set("table", Table::New(std::move(result)));
  return output;
}

Napi::Value compute_polyline_bounding_boxes(CallbackArgs const& args) {
  auto poly_offsets                   = Column::Unwrap(args[0]);
  auto point_x                        = Column::Unwrap(args[1]);
  auto point_y                        = Column::Unwrap(args[2]);
  double expansion_radius             = args[3];
  rmm::mr::device_memory_resource* mr = args[4];
  auto result                         = [&]() {
    try {
      return cuspatial::polyline_bounding_boxes(
        *poly_offsets, *point_x, *point_y, expansion_radius, mr);
    } catch (cuspatial::logic_error const& err) { throw Napi::Error::New(args.Env(), err.what()); }
  }();
  auto output = Napi::Object::New(args.Env());
  auto names  = Napi::Array::New(args.Env(), 4);
  names.Set(0u, "x_min");
  names.Set(1u, "y_min");
  names.Set(2u, "x_max");
  names.Set(3u, "y_max");
  output.Set("names", names);
  output.Set("table", Table::New(std::move(result)));
  return output;
}

}  // namespace nv
