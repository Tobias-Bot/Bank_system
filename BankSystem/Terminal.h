#pragma once

#include "Client.h"

class Terminal
{
private:
	Client client;

public:
	void EnterData(int account_id, int request, double value);
};


