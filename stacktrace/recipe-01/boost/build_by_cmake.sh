#!/bin/bash

current_dir=$( cd $(dirname ${BASH_SOURCE[0]}) && pwd )
src_dir=${current_dir}
build_dir=${current_dir}/build

cmake -S ${src_dir} -B ${build_dir} -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build ${build_dir} -j 4


