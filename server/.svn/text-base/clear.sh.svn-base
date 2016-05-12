#!/bin/sh

getcurdate(){
        date | awk '{print $6 $2 $3 $4} ' | sed "s/://g"
}

curdate=$(getcurdate)
cd /var/alllogs/
mkdir $curdate -p

cd /var/hotdance/
cp logs/* ../alllogs/$curdate -rf
rm * -rf

cd /var/hotdance2/
cp logs/* ../alllogs/$curdate -rf
rm * -rf

cd /var/hotdance3/
cp logs/* ../alllogs/$curdate -rf
rm * -rf

cd /var/hotdance4/
cp logs/* ../alllogs/$curdate -rf
rm * -rf
