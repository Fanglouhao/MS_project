#include <iostream>
#include <fstream>
#include "Time_Order.h"
#include <map>
#include <sstream>
#include <vector>
using namespace std;

map<int,map<TimeRange, vector<Order>>> readfile(ifstream&ist)
{
    map<int,map<TimeRange, vector<Order>>>data;
    string temp;
    while(getline(ist,temp))
    {
        if(temp[1] == '*') continue; //如果遇到分割线则跳过
        stringstream ss;
        ss<<temp;
        int int1,int2,day,hour,minute,int3,quantity = 0;
        double price;
        char ch1,ch2,ch3,ch4,ch5,ch6;
        ss>>int1>>ch1>>int2>>ch2>>day>>hour>>ch3>>minute>>ch4>>int3>>ch5>>price>>ch6>>quantity;
        Time time(hour,minute);
        TimeRange tr = getTimeRange(time);
        Order order(price,quantity,time);
        data[day][tr].push_back(order);
    }
    return data;
}

double getAllQuantity(vector<Order> &data) // 得到在该时段的总的quantity
{
    double sum = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        sum += data[i].quantity;
    }
    return sum;
}

double getAllCost(vector<Order> &data) // 得到在该时间段总的quantity * price
{
    double sum = 0;
    for (int i = 0; i < data.size(); ++i)
    {
        sum += data[i].quantity * data[i].price;
    }
    return sum;
}

vector<double> getQuantityRatio(map<int,map<TimeRange, vector<Order>>>&data)
{
    vector<double> quantityRatio;
    map<int, map<TimeRange, double>> RatioData;
    for (int i = 10; i <= 24 ; ++i)
    {
        int sum = 0; // 当前一天内quantity的总合
        for (int j = 1; j <= 14 ; ++j)
        {
            sum += getAllQuantity(data[i][(TimeRange)j]);
        }
        for (int k = 1; k <= 14 ; ++k)
        {
            double quantityInRange = getAllQuantity(data[i][(TimeRange)k]); //表示在该时间段内的quantity总量
            double ratio = quantityInRange/sum;
            RatioData[i].emplace((TimeRange)k,ratio);
        }
    }
    for (int l = 1; l <= 14; ++l)
    {
        double sum = 0;
        for (int i = 10; i <= 24 ; ++i)
        {
            sum += RatioData[i][(TimeRange)l];
        }
        quantityRatio.push_back(sum/15);
    }
    return quantityRatio;
}

vector<double> getAveragePrice(map<int,map<TimeRange, vector<Order>>>&data)
{
    vector<double> AveragePrrice;
    for (int i = 1; i <= 14; ++i)
    {
        double allCost = 0; //该时间段总的quantity * price
        double allQuantity = 0; //该时段的总的quantity
        for (int j = 10; j <= 24; ++j)
        {
            allCost += getAllCost(data[j][(TimeRange)i]);
            allQuantity += getAllQuantity(data[j][(TimeRange)i]);
        }
        AveragePrrice.push_back(allCost/allQuantity);
    }
    return  AveragePrrice;
}

vector<double> getQuantityRatioForTest(map<int,map<TimeRange, vector<Order>>>&data)
{
    vector<double> quantityRatio;
    double sum = 0;
    for (int i = 1; i <= 14 ; ++i)
    {
        sum += getAllQuantity(data[25][(TimeRange)i]);
    }
    for (int j = 1; j < 14 ; ++j)
    {
        quantityRatio.push_back(getAllQuantity(data[25][(TimeRange)j])/sum);
    }
    return quantityRatio;
}

vector<double> getAveragePriceForTest(map<int,map<TimeRange, vector<Order>>>&data)
{
    vector<double> averagePrice;
    for (int i = 1; i <= 14 ; ++i)
    {
        double allCost = getAllCost(data[25][(TimeRange)i]);
        double allQuantity = getAllQuantity(data[25][(TimeRange)i]);
        averagePrice.push_back(allCost/allQuantity);
    }
    return averagePrice;
}
void output(int quantity, double price, Time time)
{
    int outputQuantity = quantity/5;
    int minute = time.minute;
    int hour = time.hour;
    if(minute == 5)
    {
        cout<<"2015-5-25 "<<hour<<":0"<<minute<<":00 "<<"price: "<<price<<" quantity: "<<outputQuantity<<endl;
        minute += 5;
    }
    else
    {
        cout<<"2015-5-25 "<<hour<<":"<<minute<<":00 "<<"price: "<<price<<" quantity: "<<outputQuantity<<endl;
        minute += 5;
    }
    for (int i = 0; i < 3; ++i)
    {
        cout<<"2015-5-25 "<<hour<<":"<<minute<<":00 "<<"price: "<<price<<" quantity: "<<outputQuantity<<endl;
        minute += 5;
    }
    cout<<"2015-5-25 "<<hour<<":"<<minute<<":00 "<<"price: "<<price<<" quantity: "<<quantity-4*outputQuantity<<endl;
}
int main()
{
    int quantity;
    cout<<"Please input a big order"<<endl;
    cout<<"quantity:";
    cin>>quantity;
    string s = "iceberg.csv";
    ifstream ist(s.c_str());
    map<int,map<TimeRange, vector<Order>>> data = readfile(ist);
    vector<double> quantityRatio = getQuantityRatio(data);
    vector<double> averagePrice = getAveragePrice(data);
    string s2 = "test.csv";
    ifstream ist2(s2.c_str());
    map<int,map<TimeRange, vector<Order>>> data2 = readfile(ist2);
    vector<double> quantityRatio_former = getQuantityRatioForTest(data2);
    vector<double> averagePrice_former = getAveragePriceForTest(data2);
    double totalCost = 0; //总的花费
    int presentQuantity = 0; //当前总量
    for (int i = 0; i < quantityRatio.size()-1; ++i)
    {
        Time time = getTimeBase((TimeRange)(i+1));
        int quantityLimited = int(quantityRatio[i] * quantity);
        double price = averagePrice[i];
        output(quantityLimited, price, time);
        totalCost += quantityLimited * price;
        presentQuantity += quantityLimited;
        Order order(price,quantityLimited,time);
        data2[25][(TimeRange)(i+1)].push_back(order);
    }
    int quantityLimited = quantity - presentQuantity;
    Time time = getTimeBase((TimeRange)(14));
    double price = averagePrice[13];
    Order order(price,quantityLimited,time);
    data2[25][(TimeRange)14].push_back(order);
    output(quantityLimited, price, time);
    totalCost += quantityLimited * price;
    cout<<"totalCost: "<<totalCost<<endl;
    cout<<"averagePrice: "<<totalCost/quantity<<endl;
    vector<double> quantityRatio_after = getQuantityRatioForTest(data2);
    vector<double> averagePrice_after = getAveragePriceForTest(data2);
    cout<<"the list of quantityRatio_former: ";
    for (int j = 0; j < 13; ++j)
    {
        cout<<quantityRatio_former[j]<<" ";
    }
    cout<<endl;
    cout<<"the list of averagePrice_former: ";
    for (int j = 0; j < 13; ++j)
    {
        cout<<averagePrice_former[j]<<" ";
    }
    cout<<endl;
    cout<<"the list of quantityRatio_after: ";
    for (int j = 0; j < 13; ++j)
    {
        cout<<quantityRatio_after[j]<<" ";
    }
    cout<<endl;
    cout<<"the list of averagePrice_after: ";
    for (int j = 0; j < 13; ++j)
    {
        cout<<averagePrice_after[j]<<" ";
    }
    cout<<endl;
}
