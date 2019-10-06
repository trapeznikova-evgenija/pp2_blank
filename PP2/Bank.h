#pragma once
#include "BankClient.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <mutex>

namespace Primitive
{
	enum Synchronization
	{
		CriticalSection,
		Mutex,
		None
	};
}


class CBank
{
public:

	CBank(int primitive);
	~CBank();

	CBankClient* CreateClient();
	void UpdateClientBalance(CBankClient& client, int value);
	std::vector<CBankClient> GetAllBankClients();
	int GetTotalBalance();
	int GetClientBalance(int clientId);
	void SetClientBalance(int clientId, int value);
	Primitive::Synchronization GetPrimitive();

private:
	std::vector<CBankClient> m_clients;
	int m_totalBalance;
	std::map<int, int> m_clientBalance;
	Primitive::Synchronization m_primitive;

	void SetTotalBalance(int value);
	void SomeLongOperations();

	void StartSynchronization();
	void StopSynchronization();

	CRITICAL_SECTION csUpdateBalance;
	std::mutex mutexUpdateBalance;
};
