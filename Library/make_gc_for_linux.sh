#!/bin/sh
tar xvpfz gc-7.1.tar.gz
cd gc-7.1
./configure --prefix=${HOME}/Programing/cria/Library
make
make install
