#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// ondemand price 는 2019 12 04 us-west-california 기준
/*
//// 이미지 classifier 시뮬레이션 /////

1. 데이터 프로세싱 (local non-atomic) 100 hours
2. 데이터 업데이트 (global atomic) 50 hours
3. 딥러닝 학습 (local atomic) 250 hours
4. 학습 모델로 결과 추출 (local non-atomic ondemand) 100 hours
5. 결과 업데이트 (global atomic) 50 hours

m5_4xlarge - us_west_2a 가격 모델 사용
Ondemand price = //////// (찾아서 적용해야함)
*/

typedef struct Action
{
	string name;
	float start_time;
	float end_time;
	float total_time;
	float total_price;
	float origin_total_time;
	float accumulated_time;
	bool isEnd;
	bool isAtomic;
	bool isRunning;
	bool isOndemand;
} Action;

vector<pair<float, float> > timePrice; // time, price
vector<pair<float, string> > workProcess;
vector<Action*> actions;
float onDemandPrice, spotPrice;

Action* createAction(string _name, float _total_time, bool _isAtomic, bool _isOndemand)
{
	Action* temp = new Action;
	temp->name = _name;
	temp->total_time = _total_time;
	temp->origin_total_time = _total_time;
	temp->isAtomic = _isAtomic;
	temp->start_time = 0;
	temp->total_price = 0;
	temp->isEnd = false;
	temp->isRunning = false;
	temp->isOndemand = _isOndemand;
	temp->accumulated_time = 0;

	return temp;
}

void init()
{
	ifstream in("../../edited_data/m5_4xlarge/us_west_2b.txt");

	float a, b;
	while (true)
	{
		in >> a >> b;
		if (a == -1 && b == -1)
		{
			in >> onDemandPrice;
			break;
		}
		timePrice.push_back(make_pair(a, b));
	}
	in.close();

	spotPrice = 0.310;
	//cout << "set Spot Price: " << spotPrice << "\n";
	// cin >> spotPrice;

	// 1. data_processing (local non-atomic, 100 hours)
	actions.push_back(createAction("data_processing", 100, false, false));
	// 2. data_uploading (global atomic, 50 hours)
	actions.push_back(createAction("data_uploading", 200, true, false)); // 200
	// 3. deep_learning_training (local atomic, 400 hours)
	actions.push_back(createAction("deep_learning_training", 100, false, false));
	// 4. getting_result (local non-atomic, 100 hours)
	actions.push_back(createAction("getting_result", 100, false, true));
	// 5. result_uploading (global atomic, 50 hours)
	actions.push_back(createAction("result_uploading", 50, false, false));
}

void runSimulation()
{
	for (int i = 0; i < timePrice.size(); i++)
	{
		// 설정된 스팟 가격보다 높을 때
		if (timePrice[i].second > spotPrice)
		{
			// 실행중인 작업 다 종료
			for (int j = 0; j < actions.size(); j++)
			{
				if (actions[j]->isRunning && actions[j]->isAtomic)
				{
					actions[j]->accumulated_time += (timePrice[i].first - actions[j]->start_time);
				}
			}

			for (int j = 0; j < actions.size(); j++) actions[j]->isRunning = false;

			workProcess.push_back(make_pair(timePrice[i].first, "none"));

			continue;
		}

		//cout << "i: " << i << ", time: " << timePrice[i].first << ", price: " << timePrice[i].second << "\n";
		// 스팟 가격 이하일 때
		for (int j = 0; j < actions.size(); j++)
		{
			if (actions[j]->isEnd) continue;

			workProcess.push_back(make_pair(timePrice[i].first, actions[j]->name));

			if (actions[j]->isRunning)
			{
				// 실행중
				//if(actions[j]->isOndemand) actions[j]->total_price += ((timePrice[i].first - timePrice[i - 1].first) * onDemandPrice);
				/*else*/ actions[j]->total_price += ((timePrice[i].first - timePrice[i - 1].first) * timePrice[i].second);

				if (actions[j]->isAtomic)
				{
					if (timePrice[i].first - actions[j]->start_time >= actions[j]->origin_total_time)
					{
						actions[j]->isRunning = false;
						actions[j]->isEnd = true;
						actions[j]->end_time = actions[j]->start_time + actions[j]->origin_total_time;
					}
				}
				else
				{
					actions[j]->total_time -= (timePrice[i].first - timePrice[i - 1].first);

					if (actions[j]->total_time <= 0)
					{
						actions[j]->isRunning = false;
						actions[j]->isEnd = true;
						actions[j]->end_time = timePrice[i].first + actions[j]->total_time;
					}
				}
			}
			else
			{
				// 아직 실행 안됨
				if (actions[j]->isAtomic) actions[j]->start_time = timePrice[i].first;
				else if (actions[j]->start_time == 0) actions[j]->start_time = timePrice[i].first;

				actions[j]->isRunning = true;
			}

			break;
		}
		
		// 모든 작업 끝남
		workProcess.push_back(make_pair(timePrice[i].first, "none"));
	}
}

void printOndemandInfo()
{
	float price = 0;
	float totalTime = 0;
	for (int i = 0; i < actions.size(); i++)
	{
		price += onDemandPrice * actions[i]->origin_total_time;
		totalTime += actions[i]->origin_total_time;
	}

	float spotTotalPrice = 0;
	float spotTotalTime = 0;
	for (int i = 0; i < actions.size(); i++)
	{
		spotTotalPrice += actions[i]->total_price;
		spotTotalTime += (actions[i]->end_time - actions[i]->start_time);
		spotTotalTime += (actions[i]->accumulated_time);
	}

	cout << "--------------------------------------------------\n";
	cout << "--------------------------------------------------\n";
	cout << "Ondemand Info          ||    SpotPrice Info" << "\n";
	cout << "--------------------------------------------------\n";
	cout << "--------------------------------------------------\n";
	cout << "ondemand price: " << onDemandPrice << "  ||    spot price:   " << spotPrice << "\n";
	cout << "total price:    " << price << "  ||    total price:  " << spotTotalPrice << "\n";
	cout << "total time :    " << totalTime << "    ||    total time:   " << spotTotalTime << "\n";
	cout << "--------------------------------------------------" << "\n";
}

void printResult()
{
	printOndemandInfo();

	cout << "\n*********** Detail Info ***********\n";

	for (int i = 0; i < actions.size(); i++)
	{
		cout << "\n--------------------------------------------------\n";
		cout << actions[i]->name << "\n";

		cout << "start time:  " << actions[i]->start_time << "\n";
		cout << "end time:    " << actions[i]->end_time << "\n";
		cout << "total price: " << actions[i]->total_price << "\n";

		cout << "--------------------------------------------------" << "\n";
	}
}

int main(void)
{
	init();

	runSimulation();

	printResult();

	return 0;
}