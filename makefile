SDL2=`sdl2-config --cflags --libs`

ifneq ($(shell uname -m),x86_64)
WAVESHARE := -DUSE_WAVESHARE=1 -lbcm2835 -lm
endif

main:
	g++ -o demo main.cpp $(SDL2) $(WAVESHARE)
	./demo