#define _CRT_SECURE_NO_WARNINGS

#include "Bank.h"
#include "account.h"
#include "Client.h"
#include "Terminal.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <mpi.h>

MPI_Status status;

const int TermCount = 4;
const int BankCount = 4;
const int ClientCount = 4;

const int iterations = 15;
int k = 1;

//char fnClients[] = "transactions [clients].txt";
//char fnTerminals[] = "transactions [terminals].txt";
//char fnBanks[] = "transactions [banks].txt";
char fnServer[] = "transactions [server].txt";

int main(int argc, char** argv) {

	ofstream fServer(fnServer, ios_base::app);

	if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
		return 1;
	}

	int size, rank, msgtag = 5;

	if (MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS) {
		MPI_Finalize();
		return 2;
	}

	if (MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS) {
		MPI_Finalize();
		return 3;
	}

	while (k <= iterations) {

		// SERVER
		if (!rank) {

			int ClientID;
			int request;
			int bufSum = 0;

			int pr;	// результат поиска клиента в БД банка

			fServer.open(fnServer, ios_base::app);
			fServer << "\n_________________iteration " << k << endl;
			fServer.close();

			for (int i = 0; i < TermCount; i++)
			{
				MPI_Recv(&ClientID, 1, MPI_INT, MPI_ANY_SOURCE, msgtag, MPI_COMM_WORLD, &status);
				int term = status.MPI_SOURCE;
				MPI_Recv(&request, 1, MPI_INT, term, msgtag, MPI_COMM_WORLD, &status);
				MPI_Recv(&bufSum, 1, MPI_INT, term, msgtag, MPI_COMM_WORLD, &status);

				fServer.open(fnServer, ios_base::app);
				fServer << "\n\tServer got request from terminal " << term << endl;
				fServer.close();

				for (int j = 1; j <= BankCount; j++)
				{
					MPI_Send(&ClientID, 1, MPI_INT, j, msgtag, MPI_COMM_WORLD);

					fServer.open(fnServer, ios_base::app);
					fServer << "\nServer sent clientID to bank " << j << endl;
					fServer.close();

					MPI_Recv(&pr, 1, MPI_INT, j, msgtag, MPI_COMM_WORLD, &status);

					fServer.open(fnServer, ios_base::app);
					fServer << "Server got pr = " << pr << " from bank " << j << endl;
					fServer.close();

					if (pr == 1)
					{
						fServer.open(fnServer, ios_base::app);
						fServer << "Bank " << j << " has found client with this id = " << ClientID << endl;
						fServer.close();

						MPI_Send(&request, 1, MPI_INT, j, msgtag, MPI_COMM_WORLD);
						MPI_Send(&bufSum, 1, MPI_INT, j, msgtag, MPI_COMM_WORLD);

						fServer.open(fnServer, ios_base::app);
						fServer << "\nServer sent rest data to bank " << j << endl;
						fServer.close();

						MPI_Recv(&bufSum, 1, MPI_INT, j, msgtag, MPI_COMM_WORLD, &status);

						fServer.open(fnServer, ios_base::app);
						fServer << "\nServer got sum = " << bufSum << " from bank " << j << endl;
						fServer.close();

						pr = -1;
						for (int k = j + 1; k <= BankCount; k++) {
							MPI_Send(&pr, 1, MPI_INT, k, msgtag, MPI_COMM_WORLD);
						}

						break;
					}
				}

				MPI_Send(&bufSum, 1, MPI_INT, term, msgtag, MPI_COMM_WORLD);

				fServer.open(fnServer, ios_base::app);
				fServer << "\nServer sent response to terminal " << term << endl;
				fServer.close();
			}
		}
		// BANKS
		if ((rank == 1) || (rank == 2) || (rank == 3) || (rank == 4)) {

			Bank* bank = new Bank;
			Client* cl = new Client;

			cl->setAccoutnID(rank + 8);
			cl->setBufSum(rand() % 10000 + 1000);

			bank->addList(cl);

			int id;
			int request;
			int bufSum;

			for (int i = 0; i < TermCount; i++)
			{
				MPI_Recv(&id, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);

				if (id != -1) {

					int pr = bank->SearchID(id);

					if (pr == 1)
					{
						MPI_Send(&pr, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);

						MPI_Recv(&request, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);
						MPI_Recv(&bufSum, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);

						cl->setAccoutnID(id);
						cl->setRequest(request);
						cl->setBufSum(bufSum);

						bufSum = (bank->parseRequest(cl))->getBufSum();

						MPI_Send(&bufSum, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
					}
					else
					{
						MPI_Send(&pr, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
					}
				}
			}
		}
		// TERMINALS
		if ((rank == 5) || (rank == 6) || (rank == 7) || (rank == 8)) {

			Terminal* term = new Terminal;

			int ClientID;
			int request;
			int bufSum;

			MPI_Recv(&ClientID, 1, MPI_INT, MPI_ANY_SOURCE, msgtag, MPI_COMM_WORLD, &status);
			int cl = status.MPI_SOURCE;
			MPI_Recv(&request, 1, MPI_INT, cl, msgtag, MPI_COMM_WORLD, &status);
			MPI_Recv(&bufSum, 1, MPI_INT, cl, msgtag, MPI_COMM_WORLD, &status);

			term->EnterData(ClientID, request, bufSum);

			MPI_Send(&ClientID, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
			MPI_Send(&request, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);
			MPI_Send(&bufSum, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD);

			MPI_Recv(&bufSum, 1, MPI_INT, 0, msgtag, MPI_COMM_WORLD, &status);

			MPI_Send(&bufSum, 1, MPI_INT, cl, msgtag, MPI_COMM_WORLD);
		}
		// CLIENTS
		if (rank > 8) {

			Client* client = new Client;

			int bufSum;									// сумма, которую вносим / снимаем
			int request = rand() % 3;					// запрос (действие)
			int dest = rank - TermCount;				// терминал клиента

			if (!request) bufSum = 0;
			else bufSum = rand() % 1000 + 100;

			client->setAccoutnID(rank);
			client->setRequest(request);
			if (request) {
				client->setBufSum(bufSum);
			}
			else {
				client->setBufSum(0);
			}

			MPI_Send(&rank, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
			MPI_Send(&request, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
			MPI_Send(&bufSum, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD);

			int currentSum;

			MPI_Recv(&currentSum, 1, MPI_INT, dest, msgtag, MPI_COMM_WORLD, &status);
		}

		k++;
	}

	MPI_Finalize();

	return 0;
}