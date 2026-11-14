all:
	cmake --build build-win64 --parallel

construct:
	cmake -S . -B build-win64 \
		-DCMAKE_SYSTEM_NAME=Windows \
		-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
		-DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++

clean:
	rm -rf build-win64