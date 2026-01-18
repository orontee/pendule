#!/bin/bash

STATUS_FILE="/sys/class/drm/card0-HDMI-A-1/status"

case "$1" in
    on)
	echo "Turning HDMI display on"
        echo "on" > $STATUS_FILE
        ;;
    off)
        echo "Turning HDMI display off"
        echo "off" > $STATUS_FILE
        ;;
    *)
        echo "Usage: $0 {on|off}"
        exit 1
        ;;
esac
