SOURCES = $(shell find . -name '*.proto')
FROM = '.'
DEST = './include'
PATH = /usr/src/environment/vcpkg/installed/x64-linux/tools/protobuf/
# PATH = /usr/src/environment/vcpkg/buildtrees/protobuf/x64-linux-dbg/

all: fbuild
	@$(PATH)/protoc -I $(FROM) \
	--grpc_out=$(DEST) --plugin=protoc-gen-grpc=/usr/src/environment/vcpkg/installed/x64-linux/tools/grpc/grpc_cpp_plugin $(SOURCES)
	@$(PATH)protoc -I $(FROM) --cpp_out=$(DEST) $(SOURCES)
	@echo "makefile: \033[;32mBuild have done!\033[0m"

fbuild:
	@if [ ! -d "include" ]; then \
        mkdir include; \
    fi