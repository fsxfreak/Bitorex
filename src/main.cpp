#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "account.hpp"
#include "exchange.hpp"

extern Account account;

std::string getFilename(const int argc, char *argv[])
{
    if (argc > 1)
    {
        return argv[1];
    }
    return std::string("default_account.txt");
}

Account readAccountFromFile(const std::string &filename)
{
    std::ifstream in(filename.c_str());
    Account account;
    if (in.is_open())
    {
        //pull all the numbers from the file
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(in, line))
        {
            lines.push_back(line);
        }

        account = Account(std::atof(lines[0].c_str()), std::atof(lines[1].c_str()));
    }

    in.close();
    return account;
}

void writeAccountToFile();

void printUsage()
{
    std::cout << "Usage -" << std::endl;
    std::cout << "Quit:         q\n"
              << "Buy:          w\n"
              << "Sell:         e\n"
              << "Close buy:    s\n"
              << "Close sell:   d\n" 
              << "Save account: a\n" << std::endl;  
}

std::string filename;

int main(int argc, char *argv[])
{
    printUsage();
    filename = getFilename(argc, argv);
    account = readAccountFromFile(filename);
    
    runExchange();

    writeAccountToFile();
}