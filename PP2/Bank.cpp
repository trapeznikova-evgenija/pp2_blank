#include "Bank.h"

CBank::CBank(int primitive)
{
	m_clients = std::vector<CBankClient>();
	m_totalBalance = 0;
	m_primitive = Primitive::Synchronization(primitive);
	InitializeCriticalSection(&csUpdateBalance);
}

CBank::~CBank()
{
	DeleteCriticalSection(&csUpdateBalance);
}

Primitive::Synchronization CBank::GetPrimitive()
{
	return m_primitive;
}

void CBank::StartSynchronization()
{
	switch (CBank::GetPrimitive())
	{
	case (Primitive::Synchronization::CriticalSection):
		EnterCriticalSection(&csUpdateBalance);
	case (Primitive::Synchronization::Mutex):
		mutexUpdateBalance.lock();
	default:
		break;
	}
}

void CBank::StopSynchronization()
{
	switch (CBank::GetPrimitive())
	{
	case (Primitive::Synchronization::CriticalSection):
		LeaveCriticalSection(&csUpdateBalance);
	case (Primitive::Synchronization::Mutex):
		mutexUpdateBalance.unlock();
	default:
		break;
	}
}

CBankClient* CBank::CreateClient()
{
	unsigned clientId = unsigned(m_clients.size());
	CBankClient* client = new CBankClient(this, clientId);
	m_clients.push_back(*client);

	return client;
}

std::vector<CBankClient> CBank::GetAllBankClients()
{
	return m_clients;
}

int CBank::GetTotalBalance()
{
	return m_totalBalance;
}

int CBank::GetClientBalance(int id)
{
	return m_clientBalance.at(id);
}

void CBank::SetClientBalance(int id, int value)
{
	auto it = m_clientBalance.find(id);
	if (it != m_clientBalance.end())
	{
		it->second += value;
	}
	else
	{
		m_clientBalance.emplace(id, value);
	}
}

void CBank::UpdateClientBalance(CBankClient& client, int value)
{
	StartSynchronization();
	int totalBalance = GetTotalBalance();
	std::cout << "Client " << client.GetId() << " initiates reading total balance. Total = " << totalBalance << "." << std::endl;

	SomeLongOperations();
	totalBalance += value;
	SetClientBalance(client.GetId(), value);

	std::cout
		<< "Client " << client.GetId() << " updates his balance with " << value
		<< " and initiates setting total balance to " << totalBalance
		<< ". Must be: " << GetTotalBalance() + value << "." << std::endl;

	// Check correctness of transaction through actual total balance
	if (totalBalance != GetTotalBalance() + value)
	{
		std::cout << "! ERROR !" << std::endl;
	}

	SetTotalBalance(totalBalance);
	StopSynchronization();
}

void CBank::SetTotalBalance(int value)
{
	m_totalBalance = value;
}

void CBank::SomeLongOperations()
{
	Sleep(1000);
}
