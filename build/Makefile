# Makefile for GNU g++

CC=g++
CFLAGS=-g -Wall
all: simu
	g++ -std=c++11 -o simu Point.cpp Elements.cpp Rectangle.cpp Circle.cpp Prism.cpp RectPrism.cpp CirclePrism.cpp Simulator.cpp dessin.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`
simu: Point.cpp
	g++ -std=c++11 -o simu Point.cpp Elements.cpp Rectangle.cpp Circle.cpp Prism.cpp RectPrism.cpp CirclePrism.cpp Simulator.cpp dessin.cc -Wl,--export-dynamic `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm simu
