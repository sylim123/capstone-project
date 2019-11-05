#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

// need to localize the params for using as a header
// string msg, params, name, hostIP, port;
string ncCommand, uuid;
int param1, param2;

bool getUUID(string &uuid)
{
	char tempBuffer[50];
	FILE *fp;
	fp = popen("sudo dmidecode -s system-uuid", "r");

	if(fp == NULL) return false;

	fgets(tempBuffer, sizeof(tempBuffer), fp);
	
	if(pclose(fp) == -1) return false;
	
	string tmp(tempBuffer);
	uuid = tmp;

	return true;
}

bool init()
{
	//static string ncCommand, uuid;
	//static int param1, param2;

	string hostIP, port;

	// get Host IP
	// ifconfig | grep ****
	hostIP = "0.0.0.0 ";
	// set port
	port = "12345 ";

	ncCommand += "nc ";
	ncCommand += hostIP;
	ncCommand += port;

	if(getUUID(uuid) == false)
	{
		return false;
	}

	return true;
}

void setParams(int a, int b)
{
	param1 = a;
	param2 = b;
}

bool sendMessage()
{
	string sys_msg = ncCommand;
	sys_msg += "< temp.txt";

	string send_msg;
	send_msg += uuid;
	send_msg += " param1=";
	send_msg += ('0' + param1);
	send_msg += " param2=";
	send_msg += ('0' + param2);

	ofstream out("temp.txt");
	out << send_msg << endl;
	
	char tempBuffer[50];
	FILE *fp;
	fp = popen(sys_msg.c_str(), "r");

	if(fp == NULL) return false;

	fgets(tempBuffer, sizeof(tempBuffer), fp);
	
	if(pclose(fp) == -1) return false;;

	string result(tempBuffer);
	remove("./temp.txt");

	if(result.find("success") != -1) return true;
	else return false;
}


int main(void)
{
	init();
	setParams(1, 2);
	if(sendMessage()) cout << "success!!" << endl;
	else cout << "failed!!" << endl;

	return 0;
}
