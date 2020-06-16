#!/bin/sh
set -e
. ./common.sh

VERSION=v5.15.0

WORK_DIR=$(mktemp -d)
#WORK_DIR=$(pwd)/tmp

git clone -b $VERSION https://code.qt.io/qt/qt5.git $WORK_DIR/qt5

cd $WORK_DIR/qt5

git submodule update --init --recursive

mkdir -p $WORK_DIR/build
cd $WORK_DIR/build

$WORK_DIR/qt5/configure -release -opensource -nomake examples -nomake tests -confirm-license \
    -prefix $THIRDPARTY_DIR/qt5 \
    -skip qtgamepad -skip qtlocation \
    -skip qtmacextras -skip qtpurchasing \
    -skip qtscript -skip qtwebsockets \
    -skip qtwebengine -skip qtdocgallery \
    -c++std c++17

make -j${NUM_PROCESSES}

make install 

cd $THIRDPARTY_DIR

rm -rf $WORK_DIR
