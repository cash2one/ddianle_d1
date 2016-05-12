#!/bin/sh

Path="/var/gmserver"

cd $Path
echo "Starting GMServer..."
nohup ./GMServer > GMServer.output 2>&1 &
echo "Starting GMServer OK!"
