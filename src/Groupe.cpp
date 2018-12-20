#include "Groupe.h"

Groupe::Groupe()
{

}

void Groupe::addClient(Client client)
{
	_clients.push_back(client);
}

vector<Client> Groupe::getClient()
{
	return _clients;
}

void Groupe::setVecClients(vector<Client> clients)
{
	_clients = clients;
}


