pwd
cd _build

INCLUDE=$cur__install/include/freetype2

if which x86_64-w64-mingw32-gcc; then
    CC=x86_64-w64-mingw32-gcc
    CCLIB=
else
    CC=gcc
    CCLIB="-lbz2 -lpng -lz"
fi

echo "Using compiler: $CC"

echo "include..."
ls -a $INCLUDE
echo "lib.."
ls -a $cur__lib
$CC ./../esy/test.c -o ./test -I$INCLUDE -L$cur__lib -lfreetype -L/usr/lib -L/usr/local/lib $CCLIB
./test
