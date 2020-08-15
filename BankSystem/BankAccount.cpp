#include "account.h"

int BankAccount::getSum() { return total_sum; }
int BankAccount::getID() { return client_id; }
void BankAccount::setID(int id) { client_id = id; }

void BankAccount::addSum(int value) { total_sum += value; }
int BankAccount::takeSum(int value) {

	if (value <= total_sum) {

		total_sum -= value;
		return 1;
	}
	else {
		return -1;
	}
}
