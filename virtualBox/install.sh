#!/bin/bash

ACSDIR=~/acs/

#if [ "$(id -u)" -ne 0 ]; then
#	echo 'Please run as root or using sudo.' >&2
#	exit 1
#fi

if ! [ -x "$(command -v sqlite3)" ]; then
	echo 'Error : sqlite3 is not installed.' >&2
	exit 1
fi

if [ -d $ACSDIR ]; then
	while true; do
		read -p "ACS is already installed. Do you want to remove and reinstall? [y/n]" yn
		case $yn in
			[Yy]* ) rm -r $ACSDIR; break;;
			[Nn]* ) exit 0;;
			* ) echo "please answer yes or no." >&2 ;;
		esac
	done
fi

mkdir -p $ACSDIR
moveFiles="control"
#Movefiles should be changed
for file in $moveFiles; do
	if [ ! -f $flie ]; then
		echo "Error : $file does not exists." >&2
		rm -r $ACSDIR
		exit 1
	fi
	chmod +x $file
#cp $file /usr/local/bin/ACS$file
done

cd $ACSDIR
sqlite3 -batch vminfo.db "CREATE TABLE VM (NAME TEXT PRIMARY KEY,STATUS TEXT,ATOMNUM INTEGER,DEMANDNUM INTEGER,PREVATOMNUM INTEGER,SAVEDATA TEXT,CURDATA TEXT);"

echo "Install complete!" >&2
exit 0
