#!/bin/bash
set -e
. ./common.sh

WORK_DIR=$(mktemp -d)

cd ${WORK_DIR}
wget https://www.sqlite.org/2020/sqlite-autoconf-3320300.tar.gz
tar -xzf sqlite-autoconf-3320300.tar.gz
cd "$WORK_DIR/sqlite-autoconf-3320300"

./configure --prefix "$THIRDPARTY_DIR/sqlite3"

make -j${NUM_PROCESSES}

make install 

cd "$THIRDPARTY_DIR"

rm -rf "$WORK_DIR"
