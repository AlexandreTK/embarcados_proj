#!/bin/bash

sudo docker-compose -f /home/pi/Desktop/embarcados_proj/RASP_SERVER/docker-compose.yml up -d

sleep 5
cd /home/pi/Desktop/embarcados_proj/RASP_CLIENT/telegram_bot/
python3 telegram_reminder_bot.py &

sleep 5
cd /home/pi/Desktop/embarcados_proj/RASP_CLIENT/
./client &
