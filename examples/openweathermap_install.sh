#!/bin/bash
set -e

make
sudo service openweathermap stop
sudo cp build/openweathermap /usr/local/bin
sudo service openweathermap start