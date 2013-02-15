#ifndef FEATS_H_
#define FEATS_H_

#include "configure.h"
#include "SylMap.h"

#include <vector>
#include <set>
#include <string>

namespace std {

typedef struct _featuresOfSyllabel {
	string syllabel;
	string type;
	int label; /* 0 : SPACE, 1 : UNDER */
} featuresOfSyllabel;

class Feats{
private:
	vector<Feat*> feats_;
	string regex(string text, FeatsReference ref);
	SylMap _syl;

public:
	Feats();
	virtual ~Feats();
	size_t size(void);
	vector<Feat*>* get();
	void add(Feat* f);
	string type(string syl);
	vector<featuresOfSyllabel>* token(string text, FeatsReference ref);
	void erase(size_t x);
	void clear();
};

} /* namespace std */
#endif /* FEATS_H_ */
