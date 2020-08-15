#include "Client.h"

void Client::setRequest(int value) { request = value; }
int Client::getRequest() { return request; }

void Client::setAccoutnID(int id) { account_id = id; }
int Client::getAccoutnID() { return account_id; }

void Client::setBufSum(int value) { sum = value; }
int Client::getBufSum() { return sum; }
