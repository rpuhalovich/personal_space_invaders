build:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Debug
	cmake --build build -j16
	cp build/compile_commands.json compile_commands.json

build-release:
	cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
	cmake --build build -j16 --config Release

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file --verbose

clean:
	rm -rf build compile_commands.json

.PHONY: build format clean
.SILENT:
