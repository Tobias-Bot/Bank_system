#include "Terminal.h"

void Terminal::EnterData(int account_id, int request, double value) {
	client.setAccoutnID(account_id);
	client.setRequest(request);
	client.setBufSum(value);
}
