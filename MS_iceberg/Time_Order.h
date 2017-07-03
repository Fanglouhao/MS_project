//
// Created by apple on 2017/6/30.
//

#ifndef MS_ICEBERG_TIME_ORDER_H
#define MS_ICEBERG_TIME_ORDER_H

#include <iostream>
#include <vector>
using namespace std;
struct Time
{
    int hour;
    int minute;
    Time(int h,int m):hour(h),minute(m){}
};


enum TimeRange
{
    first = 1, second, third, fourth, fifth, sixth, seventh, eighth, ninth, tenth, eleventh, twelfth, thirteenth, fourteenth
};
// first means 9:00 to 9:30 ........


TimeRange getTimeRange(Time t);//给定时间返回相应的时间段

Time getTimeBase(TimeRange tr); //给定时间段返回时间

struct Order
{
    double price;
    int quantity;
    Time time;
    Order(double p, int q, Time t):price(p),quantity(q),time(t){}
};



#endif //MS_ICEBERG_TIME_ORDER_H
