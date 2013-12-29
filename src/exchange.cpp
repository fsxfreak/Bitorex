#define CURL_STATICLIB

/**
    If you're compiling for something other than windows, you need
    a replacement for _getch().
*/
#ifdef _WIN32
    #include <conio.h>
#endif
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <thread>

#include "curl/curl.h"
#include "json/json.h"

#include "exchange.hpp"

std::mutex lock;
volatile double close;
volatile double buy;
volatile double sell;
volatile bool run = true;
Account account;
extern std::string filename;

size_t curlToString(void *ptr, size_t size, size_t nmemb, void *data);
std::string getWebpage(const std::string &webpage);
void printTime();
void printAll();
double calculateRateToEvenLong(double fiat, double coins);
double calculateRateToEvenShort(double coins, double fiat);

void writeAccountToFile()
{
    std::ofstream out(filename.c_str());
    out << account.getFiat();
    out << '\n';
    out << account.getCoin();
    out.close();
}

Exchange::Exchange()
{

}

//this should run in another thread and update every 20 seconds
void Exchange::beginUpdating()
{
    while (run)
    {
        //Gets the json from btc-e. Change this whole parser to fit your exchange API.
        std::string exchangeInfo = getWebpage("https://btc-e.com/api/3/ticker/btc_usd");

        Json::Value root;
        Json::Reader reader;

        reader.parse(exchangeInfo, root);

        Json::Value realRoot = root.get("btc_usd", "error");

        lock.lock();
        close = realRoot.get("last", "0").asDouble(); 
        buy = realRoot.get("buy",  "0").asDouble();
        sell = realRoot.get("sell", "0").asDouble();
        lock.unlock();

        printTime();
        printAll();

        //Update interval. Slow it down if your API requires you to do so.
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }   
}

void Exchange::takeOrders()
{
    switch (_getch())
    {
    case 'w':
    case 'W':           //buy button
        transactBuy();
        break;
    case 'e': 
    case 'E':           //sell button
        transactSell();
        break;
    case 's': 
    case 'S':           //close buy
        transactCloseBuy();
        break;
    case 'd':
    case 'D':           //close sell
        transactCloseSell();
        break;
    case 'q':
    case 'Q':           //quit
        std::cout << "Quitting...(max 5 seconds)." << std::endl;
        run = false;
        break;
    case 'a':
    case 'A':
        std::cout << "Saving account..." << std::endl;
        writeAccountToFile();
        std::cout << "Saved." << std::endl;
        break;
    default:
        break;
    }
}

//TODO - Transaction ID, buy/sell/close with IDs
//returns the coins bought, with fee deducted
void Exchange::transactBuy()
{
    std::cout << "Max: " << account.fiat << " USD. Long: ";
    double amount;
    std::cin >> amount;
    amount = amount > account.fiat ? account.fiat : amount;

    lock.lock();
    double coins = amount / buy;
    lock.unlock();
    coins -= coins * FEE;

    account.coin += coins;
    account.fiat -= amount;

    Account::numOrders++;
    account.longOrders.emplace(std::make_pair(Account::numOrders, coins));

    std::cout << "You need to sell at: " << calculateRateToEvenLong(amount, coins)
              << " USD to break even." << std::endl;
}

//return the gross profit
void Exchange::transactCloseBuy()
{
    std::cout << "Close long trade with id: ";
    int id = -1;
    std::cin >> id;

    auto order = account.longOrders.find(id);
    account.longOrders.erase(id);
    double coins = order->second;

    lock.lock();
    double fiat = coins * sell;
    lock.unlock();
    fiat -= fiat * FEE;

    account.coin -= coins;
    account.fiat += fiat;
}

void Exchange::transactSell()
{
    std::cout << "Max: " << account.fiat << " USD. Short: ";
    double amount;
    std::cin >> amount;
    amount = amount > account.fiat ? account.fiat : amount;

    lock.lock();
    float coins = amount / sell;
    lock.unlock();

    amount -= amount * FEE;

    account.fiat += amount;

    Account::numOrders++;
    account.shortOrders.emplace(std::make_pair(Account::numOrders, coins));

    std::cout << "You need to buy back at: " << calculateRateToEvenShort(coins, amount)
              << " USD to break even." << std::endl;
}

void Exchange::transactCloseSell()
{
    std::cout << "Close short trade with id: ";
    int id = -1;
    std::cin >> id;

    auto order = account.shortOrders.find(id);
    account.shortOrders.erase(id);
    double coins = order->second;

    lock.lock();
    double fiat = coins * buy;
    lock.unlock();
    fiat -= fiat * FEE;

    account.fiat -= fiat;
}

void runExchange()
{
    Exchange exchange;
    std::thread ticker(&Exchange::beginUpdating, exchange);

    while (run)
    {
        exchange.takeOrders();
    }

    ticker.join();
}

void printPrices()
{
    lock.lock();
    std::cout << "Close:\t" << close << "\n"
              << "Buy:\t"   << buy   << "\n"
              << "Sell:\t"  << sell  << "\n" << std::endl;
    lock.unlock();
}

void printAll()
{
    //this will get very messy
    //please let me know if there's a better way

    std::map<int, double>::const_iterator longOrder1;
        auto id1 = -1;
        auto value1 = -1.0;
    if (account.longOrders.size() > 0)
    {
        longOrder1 = account.longOrders.begin();
            id1 = longOrder1->first;
            value1 = longOrder1->second;
    }

    std::map<int, double>::const_iterator longOrder2;
        auto id2 = -1;
        auto value2 = -1.0;
    if (account.longOrders.size() > 1)
    {
        ++longOrder1;
        longOrder2 = longOrder1;
            id2 = longOrder2->first;
            value2 = longOrder2->second;
    }

    //i hope i copypasta-ed right
    std::map<int, double>::const_iterator shortOrder1;
        auto id3 = -1;
        auto value3 = -1.0;
    if (account.shortOrders.size() > 0)
    {
        shortOrder1 = account.shortOrders.begin();
            id3 = shortOrder1->first;
            value3 = shortOrder1->second;
    }
    
    std::map<int, double>::const_iterator shortOrder2;
        auto id4 = -1;
        auto value4 = -1.0;
    if (account.shortOrders.size() > 1)
    {
        ++shortOrder1;
        shortOrder2 = shortOrder1;
            id4 = shortOrder2->first;
            value4 = shortOrder2->second;
    }

    //I now institute the 106 column master race rule for the next seven lines
    account.printStatement();
    std::cout << "Close:\t" << close << "\t|  Longs:"                               << "\n"
              << "Buy:\t"   << buy   << "\t|  ID: " << id1 << "\tCoins: " << value1 << "\n"  
              << "Sell:\t"  << sell  << "\t|  ID: " << id2 << "\tCoins: " << value2 << "\n"    
              <<                      "\t\t|  Shorts:"                              << "\n"
              <<                      "\t\t|  ID: " << id3 << "\tCoins:  " << value3 << "\n"
              <<                      "\t\t|  ID: " << id4 << "\tCoins:  " << value4 << "\n" << std::endl;
    //I apologize
}

size_t curlToString(void *ptr, size_t size, size_t nmemb, void *data)
{
    /* SOURCE: http://stackoverflow.com/questions/5525613/
                    how-do-i-fetch-a-html-page-source-with-libcurl-in-c
       Thanatos (Retrieved 2013-12-24)
    */

    std::string *str = static_cast<std::string*>(data);
    char *sptr = static_cast<char*>(ptr);

    for (unsigned int i = 0; i < size * nmemb; ++i)
    {
        (*str) += sptr[i];
    }

    return size * nmemb;
}

std::string getWebpage(const std::string &webpage)
{
    CURL *curl = nullptr;
    CURLcode curlTest;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    std::string exchangeInfo;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, webpage.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &exchangeInfo);

        //both options below are unsafe, but doesn't matter in this application
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curlTest = curl_easy_perform(curl);
        if (curlTest != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " 
                      << curl_easy_strerror(curlTest) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return exchangeInfo;
}

void printTime()
{
    //would use chrono, but std::put_time() STILL NOT IMPLEMENTED 2.5 YEARS AFTER THE STANDARD
    std::time_t time = std::time(0);
    struct tm *local = std::localtime(&time);
    std::cout << "Time: ";

    int hour = local->tm_hour;
    int min  = local->tm_min;
    int sec  = local->tm_sec;

    std::cout << std::setw(2) << std::setfill('0')
              << hour << ":" 
              << std::setw(2) << std::setfill('0')
              << min  << ":" 
              << std::setw(2) << std::setfill('0')
              << sec  << std::endl;
}

double calculateRateToEvenLong(double fiat, double coins)
{
    double rate = fiat / coins;
    rate *= (1 + FEE);  //account for the fee
    return rate;

}

double calculateRateToEvenShort(double coins, double fiat)
{
    double rate = fiat / coins;
    rate *= (1 - FEE);
    return rate;
}