mkdir -p _build
cd _build

INSTALL_PATH="$(cygpath -u $cur__install)"

echo INSTALL: $INSTALL_PATH

cmake -G "Unix Makefiles" ../ -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_LINKER=/usr/bin/x86_64-w64-mingw32-ld -DCMAKE_AR=/usr/bin/x86_64-w64-mingw32-ar -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=TRUE
