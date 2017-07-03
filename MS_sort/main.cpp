#include <iostream>
#include <fstream>
#include <sstream>
#include "for_MS.h"
using namespace std;
queue<string> outputtemps1;
queue<string> outputtemps2;
queue<string> outputtemps3;
queue<string> outputtemps4;      //用于保存需要输出的数据
queue<string> latencyOutput;     //用于保存每一行的latency
Orders orders("Orders.csv");
vector<Order> VectorOfOrder = orders.getVector();
List list1("GOOG","Buy");
List list2("AAPL","Buy");
List list3("GOOG","Sell");
List list4("AAPL","Sell");
void readQuotes()
{
    vector<List> vectorOfList;
    vectorOfList.push_back(list1);
    vectorOfList.push_back(list2);
    vectorOfList.push_back(list3);
    vectorOfList.push_back(list4);
    vector<queue<string>*> temps;
    temps.push_back(&outputtemps1);
    temps.push_back(&outputtemps2);
    temps.push_back(&outputtemps3);
    temps.push_back(&outputtemps4);
    string filename = "Quotes.csv";
    ifstream ist(filename.c_str());
    if(!ist) cout<<"can't open this file";
    string temp;
    getline(ist,temp);
    string Exchange;
    string Side;
    string Symbol;
    int Size;
    double Price;
    while(getline(ist,temp))
    {
        double time_use = 0;
        struct timeval start;
        struct timeval end;
        gettimeofday(&start,NULL);
        while (temp.find(",") <= temp.size() - 1)
        {
            temp = temp.replace(temp.find(","), 1, " ");
        }
        stringstream ss;
        ss << temp;
        ss >> Exchange >> Side >> Symbol >> Size >> Price;
        int index = 0;                                       //在每一行的处理中，先判断该quote的类别
        if(Side == "Ask"&&Symbol == "GOOG")
        {
            index=0;
        }
        else if(Side == "Ask"&&Symbol == "AAPL")
        {
            index=1;
        }
        else if(Side == "Bid"&&Symbol == "GOOG")
        {
            index=2;
        }
        else if(Side == "Bid"&&Symbol == "AAPL")
        {
            index=3;
        }
        for (int i = 0; i < VectorOfOrder.size() ; ++i)    //并在vector中与相应的Order对应
        {
            if(Size == 0) break;
            if(VectorOfOrder[i].ifhaveFilled()) continue;
            if(index == VectorOfOrder[i].getIndex()) {
                if (index < 2 && VectorOfOrder[i].getPrice() >= Price)
                {
                    VectorOfOrder[i].eatExchange(Exchange, Size, Price, vectorOfList[index]);
                }
                if (index >= 2 && VectorOfOrder[i].getPrice() <= Price)
                {
                    VectorOfOrder[i].eatExchange(Exchange, Size, Price, vectorOfList[index]);
                }
            }
        }
        if(Size != 0)                  //遍历完所有Order后，如果quote的size还有 剩余，则将该quote的数据放入相应的level1——book中。
        {
            if(Exchange == "NYSE")
            {
                vectorOfList[index].NYSE.size = Size;
                vectorOfList[index].NYSE.price = Price;
                string oneOutput = vectorOfList[index].output();
                (*temps[index]).push(oneOutput);
            }
            if(Exchange == "NASDAQ")
            {
                vectorOfList[index].NASDAQ.size = Size;
                vectorOfList[index].NASDAQ.price = Price;
                string oneOutput = vectorOfList[index].output();
                (*temps[index]).push(oneOutput);
            }
            if(Exchange == "IEX")
            {
                vectorOfList[index].IEX.size = Size;
                vectorOfList[index].IEX.price = Price;
                string oneOutput = vectorOfList[index].output();
                (*temps[index]).push(oneOutput);
            }
        }
        gettimeofday(&end,NULL);
        time_use = (end.tv_usec - start.tv_usec);
        stringstream ss2;
        ss2<<time_use<<"ns";
        string latency;
        ss2>>latency;
        latencyOutput.push(latency);
    }
}
string getRatio(queue<string> &outputtemps)
{
    string back = outputtemps.back();
    while (back.find(",") <= back.size() - 1) {
        back = back.replace(back.find(","), 1, " ");
    }
    while (back.find("@") <= back.size() - 1) {
        back = back.replace(back.find("@"), 1, " ");
    }
    stringstream ss;
    ss<<back;
    double n1, n2, n3, n4, n5, n6;
    ss >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
    double a = n1 / (n1 + n3 + n5) * 100;
    double b = n3 / (n1 + n3 + n5) * 100;
    double c = n5 / (n1 + n3 + n5) * 100;
    stringstream ss2;
    ss2<<"NYSE:"<<a<<"%"<<"NASDAQ:"<<b<<"%"<<"IEX:"<<c<<"%"<<endl;
    string s;
    ss2>>s;
    string output = "the ratio: ";
    output += s;
    return output;
}
int main()
{
    cout<<"if you want to know order1 status, please input 1"<<endl;
    cout<<"if you want to know order2 status, please input 2"<<endl;
    cout<<"if you want to know order3 status, please input 3"<<endl;
    cout<<"if you want to know order4 status, please input 4"<<endl;
    cout<<"if you want to know order5 status, please input 5"<<endl;
    cout<<"if you want to know level1 book for GOOG_Ask, please input 6"<<endl;
    cout<<"if you want to know level1 book for AAPL_Ask, please input 7"<<endl;
    cout<<"if you want to know level1 book for GOOG_Bid, please input 8"<<endl;
    cout<<"if you want to know level1 book for AAPL_Bid, please input 9"<<endl;
    cout<<"if you want to quit, please input 0"<<endl;
    thread t(readQuotes);
    t.join();
    queue<string> copy1 = outputtemps1;
    queue<string> copy2 = outputtemps2;
    queue<string> copy3 = outputtemps3;
    queue<string> copy4 = outputtemps4;
    string ratio1 = getRatio(outputtemps1);
    string ratio2 = getRatio(outputtemps2);
    string ratio3 = getRatio(outputtemps3);
    string ratio4 = getRatio(outputtemps4);
    vector<string> ratios;
    ratios.push_back(ratio1);
    ratios.push_back(ratio2);
    ratios.push_back(ratio3);
    ratios.push_back(ratio4);
    bool quit = false;
    while(!quit)
    {
        int num;
        cin>>num;
        switch(num)
        {
            case 1:
                VectorOfOrder[0].output2(ratios);
                cout<<endl;
                break;
            case 2:
                VectorOfOrder[1].output2(ratios);
                cout<<endl;
                break;
            case 3:
                VectorOfOrder[2].output2(ratios);
                cout<<endl;
                break;
            case 4:
                VectorOfOrder[3].output2(ratios);
                cout<<endl;
                break;
            case 5:
                VectorOfOrder[4].output2(ratios);
                cout<<endl;
                break;
            case 6:
                cout<<"NYSE,NASDAQ,IEX"<<endl;
                while(!copy1.empty())
                {
                    cout<<copy1.front();
                    copy1.pop();
                }
                break;
            case 7:
                cout<<"NYSE,NASDAQ,IEX"<<endl;
                while(!copy2.empty())
                {
                    cout<<copy2.front();
                    copy2.pop();
                }
                break;
            case 8:
                cout<<"NYSE,NASDAQ,IEX"<<endl;
                while(!copy3.empty())
                {
                    cout<<copy3.front();
                    copy3.pop();
                }
                break;
            case 9:
                cout<<"NYSE,NASDAQ,IEX"<<endl;
                while(!copy4.empty())
                {
                    cout<<copy4.front();
                    copy4.pop();
                }
                break;
            case 0:
                cout<<"Quit! The result will be in the output files"<<endl;
                quit = true;
                while(!latencyOutput.empty())
                {
                    cout<<"the latency of processing each quote update: ";
                    cout<<latencyOutput.front()<<endl;
                    latencyOutput.pop();
                }
                break;
            default:
                cout<<"unexpected command"<<endl;
                break;
        }

    }
    string filename1 = "Passive_Placement1.csv";
    string filename2 = "Passive_Placement2.csv";
    string filename3 = "Passive_Placement3.csv";
    string filename4 = "Passive_Placement4.csv";
    string filename5 = "Aggressive_Take_Report1.txt";
    string filename6 = "Aggressive_Take_Report2.txt";
    string filename7 = "Aggressive_Take_Report3.txt";
    string filename8 = "Aggressive_Take_Report4.txt";
    string filename9 = "Aggressive_Take_Report5.txt";
    ofstream ost1(filename1.c_str());
    ofstream ost2(filename2.c_str());
    ofstream ost3(filename3.c_str());
    ofstream ost4(filename4.c_str());
    ofstream ost5(filename5.c_str());
    ofstream ost6(filename6.c_str());
    ofstream ost7(filename7.c_str());
    ofstream ost8(filename8.c_str());
    ofstream ost9(filename9.c_str());
    ost1<<"NYSE"<<","<<"NASDAQ"<<","<<"IEX"<<endl;
    ost2<<"NYSE"<<","<<"NASDAQ"<<","<<"IEX"<<endl;
    ost3<<"NYSE"<<","<<"NASDAQ"<<","<<"IEX"<<endl;
    ost4<<"NYSE"<<","<<"NASDAQ"<<","<<"IEX"<<endl;
    VectorOfOrder[0].output(ost5);
    VectorOfOrder[1].output(ost6);
    VectorOfOrder[2].output(ost7);
    VectorOfOrder[3].output(ost8);
    VectorOfOrder[4].output(ost9);
    while(!outputtemps1.empty())
    {
        ost1<<outputtemps1.front();
        outputtemps1.pop();
    }
    while(!outputtemps2.empty())
    {
        ost2<<outputtemps2.front();
        outputtemps2.pop();
    }
    while(!outputtemps3.empty())
    {
        ost3<<outputtemps3.front();
        outputtemps3.pop();
    }
    while(!outputtemps4.empty())
    {
        ost4<<outputtemps4.front();
        outputtemps4.pop();
    }
    return 0;
}
