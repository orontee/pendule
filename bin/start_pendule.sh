#!/bin/bash

sleep 2s

weston --config $HOME/.config/weston.ini &

sleep 5s

export WAYLAND_DISPLAY=wayland-1
export DISPLAY=:1

exec /usr/bin/pendule --fullscreen
