#ifndef AUTOMATEGENERALE_H
#define AUTOMATEGENERALE_H
#include <vector>
#include <map>
#include <iostream>
#include "Vehicule.h"
#include "Etat.h"


using namespace std;

class Automate {
public:
	Automate(Etat* automate);
	Automate();
	~Automate();
	void setVecVehicule(vector<Vehicule*> vec);
	void addVehicule(Vehicule* vec);
	void eraseVehicule(Vehicule* vec);
	unsigned int nbVehicule();
	bool deplacerUtilisateurMemeQuartier(Zipcode depart, Zipcode destination);
	bool deplacerVehiculeMemeZone(Zipcode depart);
	void finDeplacement(vector<Automate*> automate);
	Etat* getRacine();
	bool contientVehicule();
	void nbDeplacementClient();
	void transferer(Automate* automate, unsigned int nbVec);
	Zipcode generAdresse();
	void clearVehicules();
private:
	Automate* findAutomateGenerale(vector<Automate*> automate, Zipcode destination);
	Etat* _racine;
	vector<Vehicule*> _vecVehicule;
};
#endif // AUTOMATE_H


