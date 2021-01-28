APPNAME=ExampleGame

all: linux_build macos_build

linux_build: example_game.c
	mkdir -p _build/x86_64
	cc example_game.c -I/usr/local/include -L/usr/local/lib -lSDL2 -o _build/x86_64/example_game.x86_64

macos_build: example_game.c info_plist iconset
	mkdir -p _build/macos/$(APPNAME).app/Contents/MacOS
	cc example_game.c -I/usr/local/include -L/usr/local/lib -lSDL2 -o _build/macos/$(APPNAME).app/Contents/MacOS/$(APPNAME)

info_plist:
	mkdir -p _build/macos/$(APPNAME).app/Contents/MacOS
	cp Info.plist _build/macos/$(APPNAME).app/Contents/Info.plist

iconset:
	mkdir -p _build/macos/$(APPNAME).app/Contents/Resources
	mkdir -p _build/macos/$(APPNAME).iconset
	sips -z 16 16     $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_16x16.png
	sips -z 32 32     $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_16x16@2x.png
	sips -z 32 32     $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_32x32.png
	sips -z 64 64     $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_32x32@2x.png
	sips -z 128 128   $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_128x128.png
	sips -z 256 256   $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_128x128@2x.png
	sips -z 256 256   $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_256x256.png
	sips -z 512 512   $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_256x256@2x.png
	sips -z 512 512   $(APPNAME)Icon.png --out _build/macos/$(APPNAME).iconset/icon_512x512.png
	cp $(APPNAME)Icon.png _build/macos/$(APPNAME).iconset/icon_512x512@2x.png
	iconutil -c icns -o _build/macos/$(APPNAME).app/Contents/Resources/$(APPNAME).icns _build/macos/$(APPNAME).iconset
	rm -r _build/macos/$(APPNAME).iconset

run: _build/x86_64/example_game.x86_64
	./_build/x86_64/example_game.x86_64
