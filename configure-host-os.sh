#!/bin/bash

set - e

# Must be root
if [[ $(id -u) -ne 0 ]] ; then echo "This script requires root privileges." ; exit 1 ; fi

echo "Apt Update"
apt update
apt install -y --no-install-recommends git

echo "Installing Docker / Docker-Compose"
apt install -y --no-install-recommends docker docker-compose 

echo "Installing Video Nice To Haves"
apt install -y --no-install-recommends v4l-utils

echo "Configuring GPSD Device"
apt remove -y gpsd

echo "Cloning REPO"
cd /opt
git clone https://github.com/Newton-17/en605.715-drone-sensor-suite.git

echo "Building Repo"
cd /opt/en605.715-drone-sensor-suite
docker-compose build

echo "Starting Streaming Applications"
docker-compose up
