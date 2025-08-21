

#!/bin/bash

echo "Starting VPN server in the background..."
sudo ./vpn_server &

echo "Waiting for tun0 device to be created..."
sleep 2

echo "Configuring network interfaces and routing..."
sudo ip addr add 10.0.0.1/24 dev tun0
sudo ip link set dev tun0 up
sudo sysctl -w net.ipv4.ip_forward=1
sudo iptables -t nat -A POSTROUTING -o enp7s0 -j MASQUERADE
sudo ip route add 10.0.0.0/24 dev tun0
sudo ip link set dev tun0 mtu 1400
echo "Server is running and configured."
echo "Press Ctrl+C to stop the server."
wait %1
# Clean up server
# sudo ip addr del 10.0.0.1/24 dev tun0
# sudo ip link set dev tun0 down
# sudo ip route del 10.0.0.0/24 dev tun0
# sudo iptables -t nat -D POSTROUTING -o wlp8s0 -j MASQUERADE

