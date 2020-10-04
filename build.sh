#!/bin/sh
mkdir build
cmake . -B build
cmake --build build
./Demo
