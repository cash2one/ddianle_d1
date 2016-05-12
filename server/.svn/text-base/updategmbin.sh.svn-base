#!/bin/sh

Path="/var/gmserver"

if [ ! -d "$Path" ];then
echo making directory $Path...
mkdir -p "$Path"
fi

ver=$1
if [ "$ver" = "" ];then
	echo $"Usage: $prog {update *.tgz}"
	exit 1
fi

echo "version = $ver"
echo "updating gmserver"
cd	/var/gmserver/
cp -f /opt/gmserver/archive/$ver	/var/gmserver
tar	-zxvf $ver GMServer && echo "------------------update gm server ok!"



