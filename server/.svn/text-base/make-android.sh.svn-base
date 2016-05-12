#!/bin/bash

sh android-env.sh
sh make-makefile.sh

make clean -f makefile
make -f makefile && make serveredition -f makefile
make GMServer -f makefile && make gmedition -f makefile