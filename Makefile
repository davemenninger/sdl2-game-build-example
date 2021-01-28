all: linux_build

linux_build: example_game.c
	mkdir -p _build/x86_64
	cc example_game.c -I/usr/local/include -L/usr/local/lib -lSDL2 -o _build/x86_64/example_game.x86_64

run: _build/x86_64/example_game.x86_64
	./_build/x86_64/example_game.x86_64
