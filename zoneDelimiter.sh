#!/bin/bash

# Installation:
# sudo apt-get install eibd-server eibd-clients

# Add route the messages
#sudo route add 224.0.23.12 dev eth0

# Find the IP Adress
#java -cp "calimero-core-2.2.jar:calimero-tools-2.2.jar" tuwien.auto.calimero.tools.Discover -s

# Launch the daemon
#eibd -D -S -T -i --eibaddr=0.0.1 --daemon=/var/log/eibd.log --no-tunnel-client-queuing ipt:169.254.108.51


# Read transferred data with timestamp
#groupsocketlisten ip:127.0.0.1 | ts '[%H:%M:%S]'
while true;do

	if groupsocketlisten ip:127.0.0.1 | grep -q "1.1.9"
	then
		  echo "9" |ts -i %s
	fi
done
