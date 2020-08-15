#pragma once

#include "Client.h"

class BankAccount
{
private: 
	int client_id;
	int total_sum = 0;

public:
	int getSum();
	int getID();
	void setID(int id);
	
	void addSum(int value);
	int takeSum(int value);

};

