
#ifndef GROUPE_H
#define GROUPE_H

#include "CLient.h"
#include <iostream>
#include <vector>
using namespace std;

class Groupe {

public:
	Groupe();
	void addClient(Client client);
	vector<Client> getClient();
	void setVecClients(vector<Client> clients);
private:
	vector<Client> _clients;
 };








#endif
