#!/bin/bash

# Run this script on a cronjob every minute. Make sure screenlock-monitor.sh is running in the background.

DATE=`date +"%Y-%m-%d %H:%M:%S"`
ATDESK=`cat $HOME/.screenunlocked`
HEIGHT=`particle call Desk getheight`
SSID=`/sbin/iwgetid -r`

if lsusb|grep 2b04:c006 &> /dev/null; then
	DOCKED=1
else
	DOCKED=0
	ATDESK=0
fi

if [ $SSID == "HitLabs-5" ]
then
	ATWORK=1
else
	ATWORK=0
	ATDESK=0
fi

if [ $HEIGHT -gt 82 ]
then
	STANDING=1
else
	STANDING=0
fi

sqlite3 desk.db "INSERT INTO desk (created_at, height, standing, docked, atwork, atdesk) VALUES (datetime('now', 'localtime'),$HEIGHT, $STANDING, $DOCKED, $ATWORK, $ATDESK);"
