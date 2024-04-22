cd utils/

./algorithms/build.sh
cp ./algorithms/algorithms.o ./bin/
cp ./algorithms/algorithms.h ./include/

./stringStream/build.sh
cp ./stringStream/stringStream.o ./bin/
cp ./stringStream/stringStream.h ./include/

./math/build.sh
cp ./math/math.o ./bin/
cp ./math/math.h ./include/

gcc -shared -o libutils.so ./bin/stringStream.o ./bin/math.o

mv ./libutils.so ../libs/

cd ../
