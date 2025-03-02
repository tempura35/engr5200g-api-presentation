g++ -o ./build/single_download.o \
    ../support/download_configs.cpp ./src/single_download.cpp \
    -lcurl -lyaml-cpp
./build/single_download.o ./assets/download_configs.yaml