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

import {Column} from './column';
import {ColumnsMap, TypeMap} from './types/mappings';

export class ColumnAccessor<T extends TypeMap = any> {
  private _data: ColumnsMap<T>;
  private _labels_to_indices: Map<keyof T, number> = new Map();

  constructor(data: ColumnsMap<T>) {
    const columns = Object.values(data);
    if (columns.length > 0) {
      const N = columns[0].length;
      if (!columns.every((col) => col.length == N)) {
        throw new Error('Column lengths must all be the same')
      }
    }
    this._data = data;
    this.names.forEach((val, index) => this._labels_to_indices.set(val, index));
  }

  get names() { return Object.keys(this._data) as ReadonlyArray<keyof T>; }

  get columns(): ReadonlyArray<Column> { return Object.values(this._data); }

  get length() { return this._labels_to_indices.size; }

  get<R extends keyof T>(name: R) {
    if (!(name in this._data)) { throw new Error(`Unknown column name: ${name.toString()}`); }
    return this._data[name];
  }

  addColumns<R extends TypeMap>(data: ColumnsMap<R>) {
    return new ColumnAccessor({...this._data, ...data} as ColumnsMap<T&R>);
  }

  dropColumns<R extends keyof T>(names: R[]) {
    const data     = {} as any;
    const namesMap = names.reduce((xs, x) => ({...xs, [x]: true}), {});
    for (const name of this.names) {
      if (!(name in namesMap)) { data[name] = this._data[name]; }
    }
    return new ColumnAccessor<Omit<T, R>>(data);
  }

  selectByColumnName<R extends keyof T>(name: R) { return this.selectByColumnNames([name]); }

  selectByColumnNames<R extends keyof T>(names: R[]) {
    const data: ColumnsMap<{[P in R]: T[P]}> = {} as any;
    for (const name of names) {
      if (this._data[name]) { data[name] = this._data[name]; }
    }
    return new ColumnAccessor(data);
  }

  columnNameToColumnIndex(name: keyof T): number|undefined {
    return this._labels_to_indices.get(name);
  }
}
