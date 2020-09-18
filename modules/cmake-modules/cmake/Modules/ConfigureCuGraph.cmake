#=============================================================================
# Copyright (c) 2020, NVIDIA CORPORATION.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#=============================================================================

include(get_cpm)

CPMAddPackage(NAME cugraph
    VERSION        ${CUGRAPH_VERSION}
    GIT_REPOSITORY https://github.com/rapidsai/cugraph.git
    GIT_TAG        branch-${CUGRAPH_VERSION}
    DOWNLOAD_ONLY
)

message(STATUS "cugraph source dir: " ${cugraph_SOURCE_DIR})

set(CUGRAPH_INCLUDE_DIR_REAL "${cugraph_SOURCE_DIR}/cpp/include")
set(CUGRAPH_INCLUDE_DIR "${cugraph_SOURCE_DIR}/cpp/fake_include")

execute_process(COMMAND mkdir -p "${CUGRAPH_INCLUDE_DIR}")
execute_process(COMMAND ln -s -f ${CUGRAPH_INCLUDE_DIR_REAL} "${CUGRAPH_INCLUDE_DIR}/cugraph")