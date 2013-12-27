#include <iostream>

#include "account.hpp"
#include "exchange.hpp"

int Account::numOrders = 0;
Account::Account() : fiat(100.0f), coin(0.0f) { }
Account::Account(float fiat, float coin) : fiat(fiat), coin(coin) { }
Account::~Account() { }

float Account::getFiat() const { return fiat; }
float Account::getCoin() const { return coin; }

void Account::printStatement() const 
{
    std::cout << "You have " << fiat << " in fiat and "
                             << coin << " BTC." << std::endl;
}

void Account::printOrders() const
{
    if (longOrders.size() > 0)
    {
        std::cout << "Long orders: " << std::endl;
        std::map<int, double>::const_iterator it    = longOrders.begin();
        auto                                  itend = longOrders.end();

        for (; it != itend; ++it)
        {
            std::cout << "ID: " << it->first << "\tCoins: " << it->second << std::endl; 
        }
    }

    if (shortOrders.size() > 0)
    {
        std::cout << "Short orders: " << std::endl;
        std::map<int, double>::const_iterator it    = shortOrders.begin();
        auto                                  itend = shortOrders.end();

        for (; it != itend; ++it)
        {
            std::cout << "ID: " << it->first << "\tFiat: " << it->second << std::endl; 
        }
    }
        
}