// 0 - показать сумму на счете
// 1 - внести деньги
// 2 - снять деньги

#pragma once

#include "account.h"
#include "Client.h"
#include <list>

using namespace std;

class Bank
{
private:
	list <BankAccount> DB;
	BankAccount list;
	Client* client;
	int bufSum;

public:
	void addList(Client* cl);
	int SearchID(int id);
	Client* parseRequest(Client* cl);
};

