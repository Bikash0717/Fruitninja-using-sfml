#!/bin/bash

# Set the DISPLAY variable
#export DISPLAY=:0

# Compile each source file
g++ -c main.cpp -I/home/user/cpp/FruitNinjaGame-using-sfml/src #project folder
g++ -c Fruit.cpp -I/home/user/cpp/FruitNinjaGame-using-sfml/src
g++ -c Utility.cpp -I/home/user/cpp/FruitNinjaGame-using-sfml/src
g++ -c Button.cpp -I/home/user/cpp/FruitNinjaGame-using-sfml/src

# Link the object files into the final executable
g++ main.o Fruit.o Utility.o Button.o -o runsfml -L/home/user/cpp/FruitNinjaGame-using-sfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Set the library path and run the executable
export LD_LIBRARY_PATH=/home/user/cpp/FruitNinjaGame-using-sfml/lib
./runsfml
