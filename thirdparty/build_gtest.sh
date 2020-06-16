#!/bin/sh
set -e
. ./common.sh

VERSION=v1.10.x

WORK_DIR=$(mktemp -d)

git clone -b $VERSION https://github.com/google/googletest.git $WORK_DIR/googletest
cd $WORK_DIR/googletest

mkdir -p cmake/build
cd cmake/build

$CMAKE -DCMAKE_INSTALL_PREFIX=$THIRDPARTY_DIR ../..

make -j${NUM_PROCESSES}

make install 

cd $THIRDPARTY_DIR

rm -rf $WORK_DIR
