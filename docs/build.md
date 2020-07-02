# Сборка проекта

## Сборка зависимостей 

### Под Windows

1. Установите Build Tools https://visualstudio.microsoft.com/ru/visual-cpp-build-tools/
2. Установите cmake (https://cmake.org/download/)
3. Установите openssl https://slproweb.com/products/Win32OpenSSL.html
4. Установите perl и пропишите его в Path http://strawberryperl.com/
5. После этого запустите Build Tools консоль (x64 Native Tools Command Promt for VS 2019
    - перейдите в папку andeme/thirdparty (cd andeme/thirdparty)
    - запустите по очереди скрипты build_*.cmd build_*.cmd для сборки grpc, qt5, gtest

### Под linux

Зависимости можете поставить через системный менеджер пакетов (нужны qt5, grpc, gtest), либо зайти в папку
andeme/thirdparty и вызвать build_*.sh скрипты.

## Сборка проекта

### Windows
Под windows это лучше делать в Build Tools консоли

```bash
cd andeme
mkdir build
cd build
cmake -G "NMake Makefiles" ..
nmake
```

### Linux

```bash
cd andeme
mkdir build
cd build
cmake ..
make
```
