#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

string msg, params, name, hostIP, port;
int param1, param2;

string getUUID()
{
	char fileBuffer[50];
	FILE *fp;
	fp = popen("sudo dmidecode -s system-uuid", "r");
	fgets(fileBuffer, sizeof(fileBuffer), fp);
	pclose(fp);
	
	string result(fileBuffer);
	return result;
}

void init()
{
	// get Host IP
	hostIP = "0.0.0.0 ";
	// get uuid sudo dmidecode -s system-uuid
	name = getUUID();
	// set port
	port = "12345 ";

	msg += "nc ";
	msg += hostIP;
	msg += port;
}

void setParams(int a, int b)
{
	param1 = a;
	param2 = b;
	params = "params1=";
	params += ('0' + a);
	params += " params2=";
	params += ('0' + b);
}

bool sendMessage()
{
	string sys_msg = msg;
	sys_msg += "< temp.txt";

	string send_msg = " ";
	send_msg += name;
	send_msg += params;

	ofstream out("temp.txt");
	out << send_msg << endl;
	
	char fileBuffer[50];
	FILE *fp;
	fp = popen(sys_msg.c_str(), "r");
	fgets(fileBuffer, sizeof(fileBuffer), fp);
	pclose(fp);
	string result(fileBuffer);
	//system(sys_msg.c_str());
	remove("./temp.txt");
	//cout << result << endl;
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
