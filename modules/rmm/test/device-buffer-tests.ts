// Copyright (c) 2020-2021, NVIDIA CORPORATION.
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

import {DeviceBuffer} from '@nvidia/rmm';

test(`DeviceBuffer empty initialization`, () => {
  const db = new DeviceBuffer(0);
  expect(db.byteLength).toBe(0);
  expect(db.capacity).toBe(0);
  expect(db.isEmpty).toBe(true);
});

test(`DeviceBuffer initialization`, () => {
  const db = new DeviceBuffer(1000);
  expect(db.byteLength).toBe(1000);
  expect(db.capacity).toBe(1000);
  expect(db.isEmpty).toBe(false);
});

test(`DeviceBuffer resize`, () => {
  const db = new DeviceBuffer(1000);
  db.resize(1234);
  expect(db.byteLength).toBe(1234);
  expect(db.capacity).toBe(1234);
  expect(db.isEmpty).toBe(false);

  db.resize(0);
  expect(db.byteLength).toBe(0);
  expect(db.capacity).toBe(1234);
  expect(db.isEmpty).toBe(true);
});
