#include "for_MS.h"
using namespace std;
string List::output()
{
    stringstream ss;
    ss<<NYSE.size<<"@"<<NYSE.price<<","<<NASDAQ.size<<"@"<<NASDAQ.price<<","<<IEX.size<<"@"<<IEX.price;
    string temp;
    ss>>temp;
    temp = temp + "\n";
    return temp;
}
Order::Order(int o, string si, string sy, double p, int q)
        :Orderid(o),Side(si),Symbol(sy),Price(p),Quantity(q)
{
    if(Side == "Buy"&&Symbol == "GOOG")
    {
        index = 0;
    }
    else if(Side == "Buy"&&Symbol == "AAPL")
    {
        index = 1;
    }
    else if(Side == "Sell"&&Symbol == "GOOG")
    {
        index = 2;
    }
    else if(Side== "Sell"&&Symbol == "AAPL")
    {
        index = 3;
    }
}

void Order::print()
{
    cout<<Orderid<<" "<<Side<<" "<<Symbol<<" "<<Price<<" "<<Quantity<<endl;
}
int Order::getOrderid()
{
    return Orderid;
}
double Order::getPrice()
{
    return Price;
}

int Order::getQuantity()
{
    return Quantity;
}

string Order::getSide()
{
    return Side;
}

string Order::getSymbol()
{
    return Symbol;
}
bool Order::ifhaveFilled()
{
    return haveFilled;
}
void Order::fill()
{
    haveFilled = true;
}
int Order::getIndex()
{
    return index;
}

void Order::eatExchange(string Exchange,int &size, double price, List &list)
{
    if(size + present_quantity < Quantity)
    {
        present_quantity += size;
        OneExchange oneExchange(Exchange,size,price);
        exchanges.push_back(oneExchange);
        sum += size * price;
        size = 0;
    }
    else
    {
        haveFilled = true;
        int size2 = Quantity - present_quantity;
        size = size - size2;
        present_quantity = Quantity;
        OneExchange oneExchange(Exchange,size2,price);
        sum += size2 * price;
        exchanges.push_back(oneExchange);
    }
}



double Order::getAverage()
{
    return sum/present_quantity;
}


void Order::output(ofstream& ost)
{
    for (int i = 0; i < exchanges.size(); ++i)
    {
        ost<<exchanges[i].Exchange<<":"<<exchanges[i].size<<"@"<<exchanges[i].price<<endl;
    }
    ost<<"Quantity:"<<present_quantity<<endl;
    ost<<"average price:"<<getAverage()<<endl;

}

void Order::output2(vector<string> ratios)
{
    for (int i = 0; i < exchanges.size(); ++i)
    {
        cout<<exchanges[i].Exchange<<":"<<exchanges[i].size<<"@"<<exchanges[i].price<<endl;
    }
    cout<<"Quantity:"<<present_quantity<<endl;
    cout<<"average price:"<<getAverage()<<endl;
    cout<<ratios[index];
}




Orders::Orders(string filename)
{
    ifstream ist(filename.c_str());
    if(!ist) cout<<"can't open this file";
    int Orderid = 0;
    string Side;
    string Symbol;
    double Price = 0;
    int Quantity = 0;
    string temp;
    getline(ist,temp);
    stringstream ss;
    unsigned long n = temp.find("\r");
    temp.erase(0,n+1);
    while(temp.find("\r") <= temp.size()-1)
    {
        temp = temp.replace(temp.find("\r"),1,"\n");
    }
    while(temp.find(",") <= temp.size()-1)
    {
        temp = temp.replace(temp.find(","),1," ");
    }
    ss<<temp;
    while(ss>>Orderid>>Side>>Symbol>>Price>>Quantity)
    {
        Order anOrder(Orderid,Side,Symbol,Price,Quantity);
        VectorOfOrder.push_back(anOrder);
    }
}

Order Orders::getOrder(int orderid)
{
    if(orderid<=5 && orderid >= 0)
    {
        return VectorOfOrder[orderid - 1];
    }
    else
    {
        cout<<"unexpected orderid. This is the first order"<<endl;
        return VectorOfOrder[0];
    }
}

void Orders::printAll()
{
    for (int i = 0; i < VectorOfOrder.size(); ++i)
    {
        VectorOfOrder[i].print();
    }
}


vector<Order> Orders::getVector()
{
    return VectorOfOrder;
}
