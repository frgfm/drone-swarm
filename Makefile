# Makefile for GNU g++

CC=g++
CFLAGS=-g -Wall
all: demo
	g++ -std=c++11 -o demo src/core/Point.cpp src/core/Elements.cpp src/core/Rectangle.cpp src/core/Circle.cpp \
	src/core/Prism.cpp src/core/RectPrism.cpp src/core/CirclePrism.cpp src/core/Simulator.cpp \
	src/interface/ui.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`
demo: src/core/Point.cpp
	g++ -std=c++11 -o demo src/core/Point.cpp src/core/Elements.cpp src/core/Rectangle.cpp src/core/Circle.cpp \
	src/core/Prism.cpp src/core/RectPrism.cpp src/core/CirclePrism.cpp src/core/Simulator.cpp \
	src/interface/ui.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm demo
