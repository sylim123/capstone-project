#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>

#define PORT_NUMBER 12345

using namespace std;

class spot_manager
{
private:
	std::string ncCommand, myIP, hostIP;
	std::string param1, param2;
	int saveInterval;
	float price;
	bool initFlag;
	void setPrice(float);

	bool getMyIP();
	bool getHostIP();
	bool sendMessage(); 

public:
	spot_manager();
	~spot_manager();
	
	bool init(float);
	bool exit();

	bool startAtom();
	bool endAtom();
	
	bool changePrice(float);

	bool startOndemand();
	bool endOndemand();

	bool changeSaveInterval(int);
};

spot_manager::spot_manager()
{
	saveInterval = 20;
	price = 0.0;
	initFlag = false;
}

spot_manager::~spot_manager()
{
}

void spot_manager::setPrice(float userPrice)
{
	price = userPrice;
	param1 = "CHANGE_PRICE";
	param2 = to_string(price);
}

bool spot_manager::getMyIP()
{
	char tempBuffer[50];
	FILE *fp;
	fp = popen("hostname -I | awk '{print $1}'", "r");

	if(fp == NULL) return false;

	fgets(tempBuffer, sizeof(tempBuffer), fp);

	if(pclose(fp) == -1) return false;

	string tmp(tempBuffer);
	tmp[tmp.size() - 1] = ' ';
	myIP = tmp;

	return true;
}

bool spot_manager::getHostIP()
{
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

bool spot_manager::sendMessage()
{
	if(initFlag == false)
	{
		cout << "start init first!\n";
		return false;
	}

	string send_msg = "echo ";
	send_msg += "\"";
	send_msg += myIP;
	send_msg += param1;
	send_msg += " ";
	send_msg += param2;
	send_msg += "\" | ";
	send_msg += ncCommand;

	cout << "result message: " << send_msg << endl;
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

bool spot_manager::init(float userPrice)
{
	initFlag = true;

	bool result;
	result = getMyIP();
	if(result == false) return false;

	result = getHostIP();
	if(result == false) return false;

	ncCommand += "nc ";
	ncCommand += hostIP;
	ncCommand += to_string(PORT_NUMBER);

	setPrice(userPrice);

	return sendMessage();
}

bool spot_manager::exit()
{
	// need to fill out something
	param1 = "param1";
	param2 = "param2";

	return sendMessage();
}

bool spot_manager::startAtom()
{
	param1 = "CHANGE_STATUS";
	param2 = "RUN_SPOT_NO_SAVE";

	return sendMessage();
}

bool spot_manager::endAtom()
{
	param1 = "CHANGE_STATUS";
	param2 = "RUN_SPOT";

	return sendMessage();
}

bool spot_manager::changePrice(float userPrice)
{
	setPrice(userPrice);
	
	return sendMessage();
}

bool spot_manager::startOndemand()
{
	param1 = "CHANGE_STATUS";
	param2 = "RUN_ONDEMAND";

	return sendMessage();
}

bool spot_manager::endOndemand()
{
	param1 = "CHANGE_STATUS";
	param2 = "RUN_SPOT";

	return sendMessage();
}

bool spot_manager::changeSaveInterval(int interval)
{
	saveInterval = interval;

	param1 = "CHANGE_SAVE_INTERVAL";
	param2 = to_string(interval);

	return sendMessage();
}

int main(void)
{
	spot_manager temp;
	temp.init(0.0);

	if(temp.changePrice(2.222)) cout << "success" << "\n";
	else cout << "fail" << "\n";

	return 0;
}
