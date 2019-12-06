import sys
import json
import matplotlib.pyplot as plt
import time

#sys.stdout = open("./c5_2xlarge/us_west_2d.txt", 'w')

with open("./dataset/m5_4xlarge_price.txt", 'r') as f:
    json_data = json.load(f)

spotPriceHistory = json_data["SpotPriceHistory"]

us_west_2a = []
us_west_2b = []
us_west_2c = []
us_west_2d = []
for spot in spotPriceHistory:
    if spot["AvailabilityZone"] == "us-west-2a":
        us_west_2a.append({"SpotPrice": spot["SpotPrice"], "Timestamp": spot["Timestamp"]})
    elif spot["AvailabilityZone"] == "us-west-2b":
        us_west_2b.append({"SpotPrice": spot["SpotPrice"], "Timestamp": spot["Timestamp"]})
    elif spot["AvailabilityZone"] == "us-west-2c":
        us_west_2c.append({"SpotPrice": spot["SpotPrice"], "Timestamp": spot["Timestamp"]})
    elif spot["AvailabilityZone"] == "us-west-2d":
        us_west_2d.append({"SpotPrice": spot["SpotPrice"], "Timestamp": spot["Timestamp"]})
us_west_2a.reverse()
us_west_2b.reverse()
us_west_2c.reverse()
us_west_2d.reverse()

x = []
x2 = []
x3 = []
x4 = []
y = []
y2 = []
y3 = []
y4 = []

def setFirstTime(us_west):
    firstTime = us_west[0]["Timestamp"]
    month = float(firstTime[5] + firstTime[6])
    day = float(firstTime[8] + firstTime[9])
    time = float(firstTime[11] + firstTime[12]) + int(firstTime[14] + firstTime[15]) / 60
    return month, day, time

def timeStampToSec(timeStamp, _month, _day, _time):
    month = float(timeStamp[5] + timeStamp[6])
    day = float(timeStamp[8] + timeStamp[9])
    time = float(timeStamp[11] + timeStamp[12]) + int(timeStamp[14] + timeStamp[15]) / 60
    month = month - _month
    day = day - _day
    time = time - _time
    total = month * 30 * 24 + day * 24 + time
    #print(total)
    return total

m_month, m_day, m_time = setFirstTime(us_west_2a)
for data in us_west_2a:
    tempX = float(timeStampToSec(data["Timestamp"], m_month, m_day, m_time))
    tempY = float(data["SpotPrice"])
    x.append(tempX)
    y.append(tempY)
    #print(str(tempX) + " " + str(tempY))

m_month, m_day, m_time = setFirstTime(us_west_2b)
for data in us_west_2b:
    tempX = float(timeStampToSec(data["Timestamp"], m_month, m_day, m_time))
    tempY = float(data["SpotPrice"])
    x2.append(tempX)
    y2.append(tempY)
    #print(str(tempX) + " " + str(tempY))

m_month, m_day, m_time = setFirstTime(us_west_2c)
for data in us_west_2c:
    tempX = float(timeStampToSec(data["Timestamp"], m_month, m_day, m_time))
    tempY = float(data["SpotPrice"])
    x3.append(tempX)
    y3.append(tempY)
    #print(str(tempX) + " " + str(tempY))

m_month, m_day, m_time = setFirstTime(us_west_2d)
for data in us_west_2d:
    tempX = float(timeStampToSec(data["Timestamp"], m_month, m_day, m_time))
    tempY = float(data["SpotPrice"])
    x4.append(tempX)
    y4.append(tempY)
    #print(str(tempX) + " " + str(tempY))

#print("-1 -1")

plt.plot(x, y, label="us_west_2a")
plt.plot(x2, y2, label="us_west_2b")
plt.plot(x3, y3, label="us_west_2c")
plt.plot(x4, y4, label="us_west_2d")

plt.title("m5_4xlarge")
plt.legend()
plt.show()
