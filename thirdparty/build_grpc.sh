#!/bin/bash
set -e
. ./common.sh

VERSION=v1.28.x

WORK_DIR="$(mktemp -d)"

git clone -b $VERSION https://github.com/grpc/grpc "$WORK_DIR/grpc"

cd "$WORK_DIR/grpc"
git submodule update --init

mkdir -p cmake/build
cd cmake/build

$CMAKE -DgRPC_INSTALL=ON -DgRPC_SSL_PROVIDER=package -DgRPC_BUILD_TESTS=OFF \
    --config Release \
    -DCMAKE_INSTALL_PREFIX="$THIRDPARTY_DIR/grpc" ../..

make -j${NUM_PROCESSES}

make install 

cd "$THIRDPARTY_DIR"

rm -rf "$WORK_DIR"
