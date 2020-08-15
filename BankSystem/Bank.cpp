#include "Bank.h"

void Bank::addList(Client* cl) {

	list.setID(cl->getAccoutnID());
	list.addSum(cl->getBufSum());

	DB.push_back(list);
}

int Bank::SearchID(int id) {

	for (BankAccount acc : DB) {

		if (id == acc.getID())
		{
			return 1;
		}
	}

	return 0;
}

Client* Bank::parseRequest(Client* cl) {

	Client* newCl = new Client;

		if (DB.empty()) {

			newCl->setRequest(-1);
		}
		else {

			switch (cl->getRequest()) {
			case 0:

				for (BankAccount acc : DB) {

					if (cl->getAccoutnID() == acc.getID()) {

						newCl->setBufSum(acc.getSum());
						newCl->setRequest(0);
						break;
					}
				}

				break;
			case 1:

				for (BankAccount acc : DB) {

					if (cl->getAccoutnID() == acc.getID()) {

						acc.addSum(cl->getBufSum());
						newCl->setBufSum(acc.getSum());
						newCl->setRequest(0);
						break;
					}
				}

				break;
			case 2:

				for (BankAccount acc : DB) {

					if (cl->getAccoutnID() == acc.getID()) {

						if (acc.takeSum(cl->getBufSum()) == -1) {

							newCl->setRequest(-1);
						}
						else {

							newCl->setBufSum(acc.getSum());
							newCl->setRequest(0);
						}

						break;
					}
				}

				break;
			}
		}

		return newCl;
}
