#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

// need to localize the params for using as a header
string ncCommand, uuid;
int param1, param2;
vector<string> IP;

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

bool getHostIP(string &hostIP)
{
	// ifconfig | grep -oE "\b([0-9]{1,3}\.){2}[0-9]{1,3}\b"
	char tempBuffer[500];
	FILE *fp;
	size_t len = 0;
	string msg = "route -n | grep 'UG[ \\t]' | awk '{print $2}'";
	fp = popen(msg.c_str(), "r");

	if(fp == NULL) return false;

	if(fgets(tempBuffer, sizeof(tempBuffer), fp) == NULL)
	{
		return false;
	}

	string t(tempBuffer);
	t[t.size() - 1] = ' ';
	hostIP = t;

	return true;
}

bool init()
{
	//static string ncCommand, uuid;
	//static int param1, param2;

	string hostIP, port;

	// get Host IP
	// ifconfig | grep ****
	getHostIP(hostIP);
	//cout << hostIP << endl;
	// set port
	port = "12345 ";
	// echo "asdfasdfasdf hh" | 
	ncCommand += "nc ";
	ncCommand += hostIP;
	ncCommand += port;

	/*if(getUUID(uuid) == false)
	{
		return false;
	}*/

	return true;
}

void setParams(int a, int b)
{
	param1 = a;
	param2 = b;
}

bool sendMessage()
{
	string send_msg = "echo ";
	send_msg += "\"";
	//send_msg += uuid;
	send_msg += " param1=";
	send_msg += ('0' + param1);
	send_msg += " param2=";
	send_msg += ('0' + param2);
	send_msg += "\" | ";
	send_msg += ncCommand;
	
	//cout << send_msg << endl;
	char tempBuffer[50];
	FILE *fp;
	fp = popen(send_msg.c_str(), "r");

	if(fp == NULL) return false;

	fgets(tempBuffer, sizeof(tempBuffer), fp);
	
	if(pclose(fp) == -1) return false;;

	string result(tempBuffer);

	if(result.find("success") != -1) return true;
	else return false;
}


int main(void)
{
	bool result = init();
	if(result == false) return -1;

	setParams(1, 2);

	result = sendMessage();
	if(result == false) cout << "failed!!" << endl;
	else cout << "success!!" << endl;

	return 0;
}
