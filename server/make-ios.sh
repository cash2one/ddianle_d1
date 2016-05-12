#!/bin/bash

sh ios-env.sh
sh make-makefile.sh

make clean -f ios-makefile
make -f ios-makefile && make serveredition -f ios-makefile
make GMServer -f ios-makefile && make gmedition -f ios-makefile