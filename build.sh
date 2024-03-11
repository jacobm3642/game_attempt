./utils/build.sh
pwd=$(pwd)
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$pwd/libs/
lib_flags="-lX11 -lGL -lGLEW -lutils"

gcc -o main main.c -L./libs/ $lib_flags
./main
