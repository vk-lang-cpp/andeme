#!/bin/bash
set -e
. ./common.sh

VERSION=5.12

WORK_DIR="$(mktemp -d)"

git clone -b $VERSION https://code.qt.io/qt/qt5.git "$WORK_DIR/qt5"

cd "$WORK_DIR/qt5"

perl init-repository

mkdir -p "$WORK_DIR/build"
cd "$WORK_DIR/build"

"$WORK_DIR/qt5/configure" -release  -opensource \
    -nomake examples -nomake tests -confirm-license \
    -skip qtgamepad -skip qtlocation \
    -skip qtmacextras -skip qtpurchasing \
    -skip qtscript -skip qtwebsockets \
    -skip qtwebengine -skip qtdocgallery \
    -prefix "$THIRDPARTY_DIR/qt5"

make -j${NUM_PROCESSES}

make install 

cd "$THIRDPARTY_DIR"

rm -rf "$WORK_DIR"
