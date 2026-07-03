mkdir -p _build
cd _build

cmake -G "Unix Makefiles" ../ -DCMAKE_INSTALL_PREFIX=$cur__install -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DCMAKE_DISABLE_FIND_PACKAGE_HarfBuzz=TRUE

