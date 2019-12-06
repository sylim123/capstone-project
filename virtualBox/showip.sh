#!/bin/bash
	
VBoxManage list runningvms | cut -d "{" -f1 | sed "s/\"//g" | while read VBOXNAME; do
	IP=$(VBoxManage guestproperty get "$VBOXNAME" /VirtualBox/GuestInfo/Net/1/V4/IP | cut -d":" -f2)
	echo "$VBOXNAME $IP"
done
