#!/bin/sh
tar xvpfz gc-7.1.tar.gz
cd gc-7.1
./configure --prefix=`pwd'/.. --disable-threads
make
make install
