#!/bin/sh
getjavapid(){
	ps ax | grep "java" | grep -v "grep" | awk '{if(NR==1) {print $1}}'
}

start() {
        /var/hotdance/bin/debug_hotdance start

	sleep 5

	echo "Start GateServer"
        cd /var/rwserver/
        nohup ./rungate.sh &
	echo "Start GareServer OK!"
}


stop(){
        /var/hotdance/bin/debug_hotdance stop

        javapid=$(getjavapid)
        #echo $javapid
        #javapid=0;

        if [ "$javapid" = "" ];then
           echo "GateServer is not running!"
        else
           kill $javapid
           echo "Close GateServer Ok!"
        fi
}


#restart() {
#	javapid=$(getjavapid)
#       echo $javapid
#
#       if [ $javapid > 0 ];then
#          kill $javapid
#       fi
#       /var/hotdance/bin/debug_hotdance restart   
#
#	echo "Start GateServer"
#       cd /root/rwserver/
#       nohup ./rungate.sh &
#}

updateserver(){
        echo "Version = $ver"
        echo "Updating All Server...... "
        
        cd /var/hotdance4/
        cp -f /opt/smartphone/server/$ver /var/hotdance4/
        tar zxvf $ver
        cd /var/hotdance3/
        cp -f /opt/smartphone/server/$ver /var/hotdance3/
        tar zxvf $ver
        cd /var/hotdance2/
        cp -f /opt/smartphone/server/$ver /var/hotdance2/
        tar zxvf $ver
        cd /var/hotdance/
        cp -f /opt/smartphone/server/$ver /var/hotdance/
        tar zxvf $ver
        
        echo "Updating All Server OK! "
}


# see how we were called.

case "$1" in
  start)
        start
        ;;
  stop)
        stop
        ;;
  restart)
        stop
        start
        ;;
  update)
        ver=$2
        if [ "$ver" = "" ];then
           echo $"Usage: $prog {update *.tgz}"
           exit 1
        fi
        updateserver
        ;;
  *)
        echo $"Usage: $prog {start|stop|restart|update *.tgz}"
        exit 1
esac
