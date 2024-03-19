build:
	cmake -B build -S .
	cmake --build build
	cp build/compile_commands.json compile_commands.json

format:
	find src -iname "*.h" -o -iname "*.c" | xargs clang-format -i --style=file --verbose

clean:
	rm -rf build compile_commands.json

.PHONY: build format clean
.SILENT:
