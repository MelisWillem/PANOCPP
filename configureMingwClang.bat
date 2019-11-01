cd ./build

cmake ../ -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON 

cd ../
