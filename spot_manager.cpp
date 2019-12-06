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
	bool printDemand();

	bool incAtom();
	bool decAtom();

	bool incDemand();
	bool decDemand();
	
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
	//send_msg += myIP;
	send_msg += param1;
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

	//setPrice(userPrice);

	param1 = "INIT";

	return sendMessage();
}

bool spot_manager::exit()
{
	// need to fill out something
	param1 = "END";

	return sendMessage();
}

bool spot_manager::incAtom()
{
	param1 = "INC_ATOM";

	return sendMessage();
}

bool spot_manager::decAtom()
{
	param1 = "DEC_ATOM";

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

bool spot_manager::printDemand()
{
	param1 = "PRINT_DEMAND";

	return sendMessage();
}

bool spot_manager::incDemand()
{
	param1 = "INC_DEMAND";

	return sendMessage();
}

bool spot_manager::decDemand()
{
	param1 = "DEC_DEMAND";

	return sendMessage();
}

long long fibo(int n)
{
	if(n == 1 || n == 2) return 1;
	else return fibo(n-1) + fibo(n-2);
}

int main(void)
{
	spot_manager temp;
	temp.init(0.0);
	temp.printDemand();

	temp.incAtom();
	temp.printDemand();

	cout << fibo(45) << "\n";

	temp.decAtom();
	temp.printDemand();

	temp.incDemand();
	temp.printDemand();

	cout << fibo(44) << "\n";

	temp.decDemand();
	temp.printDemand();

	temp.exit();

	return 0;
}
