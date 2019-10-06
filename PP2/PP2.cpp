#include "stdafx.h"
#include "Bank.h"
#include "BankClient.h"

void ShowUsage()
{
	std::cout << "Params must be like: PP2.exe <number of clients> <number synchronization primitive>" << std::endl;
	std::cout << "To not use synchronization primitive <number synchronization primitive> = 2" << std::endl;
	std::cout << "Params by default:" << std::endl;
	std::cout << "clientsNumber = 2" << std::endl;
	std::cout << "primitive = 0" << std::endl;
}

int main(int argc, char *argv[])
{
	if (std::string(argv[1]) == "/?")
	{
		ShowUsage();
	}
	int clientsNumber = (argc == 3) ? atoi(argv[1]) : 2;
	int primitive = (argc == 3) ? atoi(argv[2]) : 0;

	CBank* bank = new CBank(primitive);
	for (int i = 0; i < clientsNumber; i++)
	{
		CBankClient* client = bank->CreateClient();
	}

	// TODO: WaitForMultipleObjects
	std::string input = "";
	std::cin >> input;
	bool exit = (input == "");

	while (std::cin && !exit)
	{
		std::cin >> input;
		if (input == "/?")
		{
			ShowUsage();
		}
		else
		{
			exit = ((input == "quit") || (input == "exit"));
		}
	}

	auto allClients = bank->GetAllBankClients();

	std::cout << "CLIENTS INFO" << std::endl;
	int amountBalance = 0;
	for (auto client : allClients)
	{
		auto balance = bank->GetClientBalance(client.GetId());
		std::cout << "Id: " << client.GetId() << " Balance: " << balance << std::endl;
		amountBalance += balance;
	}

	std::cout << "Amount of clients balances: " << amountBalance << std::endl;
	std::cout << "Bank balance: " << bank->GetTotalBalance() << std::endl;

	return 0;
}
