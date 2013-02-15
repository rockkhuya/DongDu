#ifndef FEATURESSELECTION_H_
#define FEATURESSELECTION_H_

#include "./liblinear/linear.h"
#include "StrMap.h"

#include <set>
#include <vector>
#include <fstream>
#include <string>

namespace std {

class FeaturesSelection {
private:
	StrMap _strmap, new_strmap;
	model* _model;
	string PATH;
public:
	FeaturesSelection(string str);
	virtual ~FeaturesSelection();
	void selection();
	void save();
};

} /* namespace std */
#endif /* FEATURESSELECTION_H_ */
