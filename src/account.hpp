#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <map>
#include <utility>

struct Account
{
    Account();
    Account(float fiat, float coin);
    ~Account();

    float getFiat() const;
    float getCoin() const;

    void printStatement() const;
    void printOrders() const;

    float fiat;
    float coin;

    static int numOrders;

    std::map<int, double> longOrders;   //holds trade ID and amount of coins bought
    std::map<int, double> shortOrders;  //holds trade ID and amount of fiat bought
};



#endif