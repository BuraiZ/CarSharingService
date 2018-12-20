#ifndef ZIPCODE_H
#define ZIPCODE_H

#include <string>

using namespace std;

class Zipcode {
public:

	Zipcode();
	Zipcode(char zipCodeString[]);
	Zipcode(const Zipcode& zipCode);
	char getZipCodeNameAt(const unsigned int& index) const;
	string getZipCodeName() const;
	bool compare(Zipcode zip);

private:
	char _zipCodeName[6];
};
#endif
