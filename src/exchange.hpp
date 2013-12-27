#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "account.hpp"

const double FEE = 0.002; //0.2% BTC-e fee. Change this if pulling from another exchange.

void runExchange();             //runs the ticker and the ordering function

struct Exchange
{
    Exchange();

    void beginUpdating();       //update the ticker
    void takeOrders();          //listen for key presses

    void transactBuy();         //make a buy transaction for the account
    void transactCloseBuy();    //close a buy transaction for the account

    void transactSell();
    void transactCloseSell();
};

#endif