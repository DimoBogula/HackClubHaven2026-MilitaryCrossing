all:
	cmake --build build-win64 --parallel

linux:
	cmake --build build-linux --parallel

construct:
	cmake -S . -B build-win64 \
		-DCMAKE_SYSTEM_NAME=Windows \
		-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
		-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

constructlinux:
	cmake -S . -B build-linux \
		-DCMAKE_SYSTEM_NAME=Linux \
		-DCMAKE_C_COMPILER=gcc \
		-DCMAKE_CXX_COMPILER=g++

cleanlinux:
	rm -rf build-linux

clean:
	rm -rf build-win64