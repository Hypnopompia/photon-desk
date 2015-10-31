#!/bin/bash

# http://unix.stackexchange.com/questions/28181/run-script-on-screen-lock-unlock
# http://askubuntu.com/questions/48321/how-do-i-start-applications-automatically-on-login

echo 1 > $HOME/.screenunlocked;

dbus-monitor --session "type='signal',interface='com.ubuntu.Upstart0_6'" | \
(
  while true; do
    read X
    if echo $X | grep "desktop-lock" &> /dev/null; then
      echo 0 > $HOME/.screenunlocked;
    elif echo $X | grep "desktop-unlock" &> /dev/null; then
      echo 1 > $HOME/.screenunlocked;
    fi
  done
)