#ifndef SPOT_MANAGER_H
#define SPOT_MANAGER_H

class SpotManager
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
	SpotManager();
	~SpotManager();
	
	bool init(float);
	bool exit();

	bool startAtom();
	bool endAtom();
	
	bool changePrice(float);

	bool startOndemand();
	bool endOndemand();

	bool changeSaveInterval(int);
};

#endif
