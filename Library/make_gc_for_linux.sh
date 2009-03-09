#!/bin/sh
tar xvpfz gc-7.1.tar.gz
cd gc-7.1
./configure --prefix=${HOME}/Programing/cria/Library --disable-threads
make
make install
