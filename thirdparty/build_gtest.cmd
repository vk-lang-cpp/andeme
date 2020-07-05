set VERSION=v1.10.x
set THIRDPARTY_DIR=%cd%

mkdir tmp
set WORK_DIR=%THIRDPARTY_DIR%\tmp

git clone -b %VERSION% https://github.com/google/googletest.git "%WORK_DIR%\googletest"
cd %WORK_DIR%\googletest

mkdir cmake
mkdir cmake\build
cd cmake\build

cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" -DCMAKE_INSTALL_PREFIX="%THIRDPARTY_DIR%\gtest" ..\.. 

nmake                                                                                                                                             

nmake install 

cd "%THIRDPARTY_DIR%"

rmdir /S /Q "%WORK_DIR%"
