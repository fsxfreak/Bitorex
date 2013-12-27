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

void writeAccountToFile(const std::string &filename, const Account &account)
{
	std::ofstream out(filename.c_str());
	out << account.getFiat();
	out << '\n';
	out << account.getCoin();
	out.close();
}

void printUsage()
{
	std::cout << "Usage -" << std::endl;
	std::cout << "Quit: 		q\n"
			  << "Buy:  		w\n"
			  << "Sell: 		e\n"
			  << "Close buy:	r\n"
			  << "Close sell: 	t\n" << std::endl;	
}

int main(int argc, char *argv[])
{
	printUsage();
	std::string filename = getFilename(argc, argv);
	account = readAccountFromFile(filename);
	
	runExchange();

	writeAccountToFile(filename, account);
}