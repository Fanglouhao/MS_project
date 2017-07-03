#ifndef MS_PROJECT_FOR_MS_H
#define MS_PROJECT_FOR_MS_H
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <queue>
#include <sys/time.h>
#include <stdio.h>
#include <assert.h>
#include "std_lib_facilities.h"
using namespace std;

struct DatainList //用于存储level1_book中每个格子中的数据
{
    int size = 0;
    double price = 0;
};
struct List //用于存储level1——book
{
    string symbol;
    string side;
    DatainList NYSE;
    DatainList NASDAQ;
    DatainList IEX;
    List(string sy,string si):symbol(sy),side(si){};
    string output();
};


class Order  //用于存储一个订单
{
private:
    struct OneExchange  //用于存储一次交易的信息
    {
        string Exchange;
        int size;
        double price;
        OneExchange(string E, int s, double p):Exchange(E),size(s),price(p){}
    };
    int Orderid;
    string Side;
    string Symbol;
    double Price = 0;
    int Quantity = 0;
    double sum = 0; // 表示总共的交易额
    int index = 0; //为用于订单分类的编号
    bool haveFilled = false;//这个订单是否被填满
    vector<OneExchange> exchanges;//表示被该订单吃掉的quote
    int present_quantity = 0;//表示当前的被吃掉的size
    double average_price;
public:
    Order(int o, string si, string sy, double p, int q);
    void print();
    int getOrderid();
    string getSide();
    string getSymbol();
    double getPrice();
    int getQuantity();
    int getIndex();
    bool ifhaveFilled();
    void eatExchange(string Exchange ,int &size, double price, List &list);//表示吃掉一次交易
    double getSum(){return sum;}
    double getAverage();
    void fill();
    void output(ofstream &ost);//表示将结果输出到一文件中
    void output2(vector<string> ratios);//表示将结果输出在命令行中。
};


class Orders
{
private:
    vector<Order> VectorOfOrder;
    string filename;
public:
    Orders(string filename);
    Order getOrder(int orderid);
    vector<Order> getVector();//表示将Orders以vector的形式输出
    void printAll();
};




#endif //MS_PROJECT_FOR_MS_H
