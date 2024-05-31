#!/bin/bash
BUILD_DIR=build-debug
mkdir -p ${BUILD_DIR}
pushd ${BUILD_DIR}
cmake -Wno-dev -DCMAKE_BUILD_TYPE=Debug ..
make
cd src/g.tests/
ctest
popd

