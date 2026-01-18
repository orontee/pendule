#!/bin/bash

TARGET_SERVICE="pendule.service"
DISPLAY_STATUS_FILE="/sys/class/drm/card0-HDMI-A-1/status"

function reconcile_target_service {
    local display_status=$(cat ${DISPLAY_STATUS_FILE})
    if [[ "${display_status}" == "connected" ]]; then
	echo "Will start pendule.service since display was connected"
        systemctl start ${TARGET_SERVICE}
    else
	echo "Will stop pendule.service since display was disconnected"
        systemctl stop ${TARGET_SERVICE}
    fi
}

reconcile_target_service

while inotifywait -e modify ${DISPLAY_STATUS_FILE}; do
    reconcile_target_service
done
