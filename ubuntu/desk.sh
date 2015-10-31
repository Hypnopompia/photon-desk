#!/bin/bash

# Run this script on a cronjob every minute. Make sure screenlock-monitor.sh is running in the background.

DATE=`date +"%Y-%m-%d %H:%M:%S"`
ATDESK=`cat $HOME/.screenunlocked`
HEIGHT=`particle call Desk getheight`

if [ $HEIGHT -gt 82 ]
then
	STANDING=1
else
	STANDING=0
fi

echo "Height" $HEIGHT
echo "Standing" $STANDING
echo "At Desk" $ATDESK

echo "\""$DATE"\","$HEIGHT","$STANDING","$ATDESK >> $HOME/desk.csv

sqlite3 desk.db "INSERT INTO desk (height, standing, atdesk) VALUES ($HEIGHT, $STANDING, $ATDESK);"