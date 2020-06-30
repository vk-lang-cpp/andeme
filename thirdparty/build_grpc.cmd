set VERSION=v1.28.x

set THIRDPARTY_DIR=%cd%

mkdir tmp
set WORK_DIR="%cd%\tmp"

git clone -b %VERSION% https://github.com/grpc/grpc "%WORK_DIR%\grpc"

cd "%WORK_DIR%\grpc"
git submodule update --init

mkdir build
mkdir cmake\build
cd cmake\build

cmake -G "NMake Makefiles" -DgRPC_INSTALL=ON -DgRPC_SSL_PROVIDER=package -DgRPC_BUILD_TESTS=OFF --config Release -DCMAKE_INSTALL_PREFIX="%THIRDPARTY_DIR%\grpc" ..\..

nmake

nmake install 

cd "%THIRDPARTY_DIR%"

rmdir /S /Q "%WORK_DIR%"
