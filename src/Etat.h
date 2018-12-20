#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <vector>
#include <map>
#include <iostream>
#include "Vehicule.h"
#include "Zipcode.h"

using namespace std;

class Etat {
public:
	Etat();	
	~Etat();
	Etat(char value);
	void destroy(Etat* automate);
	void addChild(Etat* automate);
	bool isEmpty();
	void setValue(char value);
	char getValue();
	Etat* contains(char value);
	bool search(Zipcode zip);
	Zipcode generAdresse();
private:
	char _value;
	vector<Etat*> _nextState;
};
#endif // AUTOMATE_H


