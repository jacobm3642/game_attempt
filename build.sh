./utils/build.sh
./window/build.sh

pwd=$(pwd)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$pwd/libs/
lib_flags="-lX11 -lGL -lGLEW -lutils -lwindow"

gcc -o main.a main.c -L./libs/ $lib_flags
echo running main
./main.a
