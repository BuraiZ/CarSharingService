#ifndef VEHICULE_H
#define VEHICULE_H

#include "Zipcode.h"

class Vehicule {
public:

	Vehicule();
	Vehicule(Zipcode positionActuelle, Zipcode positionFinale, int id);

	bool getDisponibilite() const;
	Zipcode getPositionActuelle() const;
	Zipcode getPositionFinale() const;

	void setDisponibilite(const bool& disponible);
	void setPositionFinale(const Zipcode& positionFinale);
	void setPositionActuelle(const Zipcode& positionActuelle);
	void incrementTrajetClient();
	void incrementTrajetVide();
	int getNbTrajetClient();
	int getNbTrajetVide();
	int getId();
private:
	Zipcode _positionActuelle;
	Zipcode _positionFinale;
	bool _disponible;
	int _nbTrajetClient = 0;
	int _nbTrajetVide = 0;
	int _id;
};


#endif
