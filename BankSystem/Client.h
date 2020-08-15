#pragma once

class Client
{
private:
	int sum;
	int request;
	int account_id;

public:
	void setRequest(int value);
	int getRequest();

	void setAccoutnID(int id);
	int getAccoutnID();

	void setBufSum(int value);
	int getBufSum();
};

