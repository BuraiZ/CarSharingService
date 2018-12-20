#ifndef CLIENT_H
#define CLIENT_H

#include "Zipcode.h"
#include <iostream>

using namespace std;


class Client {

public:

	Client();
	Client(const Zipcode& pointDepart, const Zipcode& destination);
	void afficherInformations() const;
	Zipcode getPointDepart() const;
	Zipcode getDestination() const;


private:
	Zipcode _pointDepart;
	Zipcode _destination;
};








#endif
