# # Clean up client
# sudo ip addr del 10.0.0.2/24 dev tun1
# sudo ip link set dev tun1 down
# sudo ip route del default via 10.0.0.1 dev tun1
#!/bin/bash

echo "Starting VPN client in the background..."
sudo ./vpn_client &

echo "Waiting for tun1 device to be created..."
sleep 2

echo "Configuring network interfaces and routing..."
sudo ip addr add 10.0.0.2/24 dev tun1
sudo ip link set dev tun1 up
sudo ip route add default via 10.0.0.1 dev tun1
sudo ip link set dev tun1 mtu 1400
echo "Client is running and configured."
echo "Press Ctrl+C to stop the client."
wait %1