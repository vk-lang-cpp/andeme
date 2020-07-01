set VERSION=5.12

set THIRDPARTY_DIR=%cd%
mkdir tmp
set WORK_DIR=%cd%\tmp

git clone -b %VERSION% https://code.qt.io/qt/qt5.git "%WORK_DIR%\qt5"

cd "%WORK_DIR%\qt5"

perl.exe init-repository

mkdir "%WORK_DIR%\build"
cd "%WORK_DIR%\build"

"%WORK_DIR%\qt5\configure" -release  -opensource -nomake examples -nomake tests -confirm-license -skip qtdatavis3d -skip qtdeclarative -skip qtdocgallery -skip qtfeedback -skip qtgamepad -skip qtlocation -skip qtmacextras -skip qtnetworkauth -skip qtpim -skip qtpurchasing -skip qtqa -skip qtquickcontrols -skip qtquickcontrols2 -skip qtremoteobjects -skip qtrepotools -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qttranslations -skip qtwebengine -skip qtwebsockets -prefix "%THIRDPARTY_DIR%\qt5" && exit 0

nmake

nmake install 

cd "%THIRDPARTY_DIR%"

rmdir /S /Q "%WORK_DIR%"
