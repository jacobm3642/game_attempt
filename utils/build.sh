cd utils/
./stringStream/build.sh
cp ./stringStream/stringStream.o ./bin/
cp ./stringStream/stringStream.h ./include/

gcc -shared -o libutils.so ./bin/stringStream.o

mv ./libutils.so ../libs/

cd ../
