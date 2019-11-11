#!/bin/bash

sudo docker-compose -f /home/pi/Desktop/embarcados_proj/RASP_SERVER/docker-compose.yml stop

sudo killall python3
sudo killall client
