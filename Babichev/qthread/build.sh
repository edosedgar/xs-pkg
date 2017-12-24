#!/bin/bash

moc -o main.moc main.cpp
g++ -o main ./main.cpp -DQT_SHARED -I/usr/include/qt4 -I/usr/include/qt4/QtCore -lQtCore
