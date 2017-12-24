#!/bin/bash

moc -o main.moc main.cpp
g++ -o main ./main.cpp `pkg-config --libs --cflags QtCore`
