#!/bin/bash

cd scripts
./download_freetype.sh
./build_freetype.sh
./download_sdl2.sh
./build_sdl2.sh
./download_sdl2_image.sh
./build_sdl2_image.sh
./download_sdl2_ttf.sh
./build_sdl2_ttf.sh
./download_sdl2_gfx.sh
./build_sdl2_gfx.sh
./download_lua.sh
./build_lua.sh
./download_tinyxml.sh
./build_tinyxml.sh
cd ..
