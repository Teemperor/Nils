#!/bin/bash

set -e

export CC="$1"
export CXX="$2"

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR/.."

mkdir -p build
cd build

cmake ..
make -j2
ctest -j1 -VV
