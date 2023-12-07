DIR_Config   = ./waveshare/Config
DIR_EPD      = ./waveshare/LCD
DIR_FONTS    = ./waveshare/Fonts
DIR_GUI      = ./waveshare/GUI

WAVESHARE_C= $(shell find $(./waveshare) -type f -name '*.c' -not -path '*/\.*')

SDL2=`sdl2-config --cflags --libs`

ifneq ($(shell uname -m),x86_64)
WAVESHARE := -DUSE_WAVESHARE=1 -lbcm2835 -lm -DUSE_BCM2835_LIB=1
endif

# WAVESHARE=-DUSE_WAVESHARE=1

main:
	g++ -o demo main.cpp $(WAVESHARE_C) $(SDL2) $(WAVESHARE) -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD)
	./demo
