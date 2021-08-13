SOURCES = $(shell find . -name '*.proto')
FROM = '.'
DEST = './build'

all: fbuild
	@/usr/src/environment/vcpkg/installed/x64-linux/tools/protobuf/protoc -I $(FROM) \
	--grpc_out=$(DEST) --plugin=protoc-gen-grpc=/usr/src/environment/vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin $(SOURCES)
	@/usr/src/environment/vcpkg/installed/x64-linux/tools/protobuf/protoc -I $(FROM) --cpp_out=$(DEST) $(SOURCES)
	@echo "makefile: \033[;32mBuild have done!\033[0m"

fbuild:
	@if [ ! -d "build" ]; then \
        mkdir build; \
    fi