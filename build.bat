call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" ..
nmake