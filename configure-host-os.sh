#!/bin/bash

set - e

# Must be root
if [[ $(id -u) -ne 0 ]] ; then echo "This script requires root privileges." ; exit 1 ; fi

echo "Apt Update"
apt update
apt install -y --no-install-recommends git

# Access Point Update
echo "Configuring Access Point"
apt install hostapd dnsmasq bridge-utils dhcpcd5
systemctl stop hostapd
systemctl stop dnsmasq

cat <<EOT >> /etc/dhcpcd.conf
interface wlan0
static ip_address=192.168.0.10/24
EOT

mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
cat <<EOT >> /etc/dnsmasq.conf
interface=wlan0
dhcp-range=192.168.0.11,192.168.0.30,255.255.255.0,24h
domain=wlan
address=/gw.wlan/192.168.0.10
EOT

cat <<EOT >> /etc/hostapd/hostapd.conf
interface=wlan0
hw_mode=g
channel=7
wmm_enabled=0
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
ssid=pinet
wpa_passphrase=pinet1234!
EOT

echo "DAEMON_CONF=\"/etc/hostapd/hostapd.conf\"" >> /etc/default/hostapd


echo "net.ipv4.ip_forward=1" >> /etc/sysctl.conf

iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
sudo sh -c "iptables-save > /etc/iptables.ipv4.nat"
cat <<EOT >> /etc/rc.local
iptables-restore < /etc/iptables.ipv4.nat
exit 0
EOT

sudo brctl addbr br0
sudo brctl addif br0 eth0
cat <<EOT >> /etc/network/interfaces
auto br0
iface br0 inet manual
bridge_ports eth0 wlan0
EOT

# Docker/Compose Update
echo "Installing Docker / Docker-Compose"
apt install -y --no-install-recommends docker docker-compose

# Add Video Tools
echo "Installing Video Nice To Haves"
apt install -y --no-install-recommends v4l-utils

# Remove GPSD Installed by default
echo "Removing HOST GPSD Device"
apt remove -y gpsd

# Clone the Repo
echo "Cloning REPO"
cd /opt
git clone https://github.com/Newton-17/en605.715-drone-sensor-suite.git

# Build out the code
echo "Building Repo"
cd /opt/en605.715-drone-sensor-suite
docker-compose build

# Start the code
echo "Starting Streaming Applications"
docker-compose up
