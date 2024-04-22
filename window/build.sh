cd ./window

gcc -shared -fPIC -lX11 -lGL -lGLEW -o libwindow.so window.c 

mv ./libwindow.so ../libs/ 

rm ../include/window.h
ln ./window.h ../include/window.h 

cd ../
