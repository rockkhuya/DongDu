#ifndef MACHINE_H_
#define MACHINE_H_

#include "Feats.h"
#include "DicMap.h"
#include "StrMap.h"
#include "SylMap.h"
#include "configure.h"
#include "./liblinear/linear.h"

#include <string>
#include <time.h>
#include <ctime>

namespace std {

class Machine {
private:
	size_t index_SPACE;
	size_t index_UNDER;
	size_t reference;
	Feats* 		feats;
	DicMap		dicmap;
	StrMap		strmap;
	model*		_model;
	problem   _problem;
	vector<featuresOfSyllabel>* vfeats;
	int  WINDOW_LENGTH;
	string PATH;

	size_t getByteOfUTF8(unsigned char c);
	string itostr(int x);
	void convert(string sentence);

public:
	Machine(int window_length, string path, StrMapReference ref);
	virtual ~Machine();

	void extract(string sentence, StrMapReference ref);
	void getProblem();
	void delProblem();
	/* Learner */
	void training();
	void print();
	double close_test();
	void featuresSelection();

	/* Predictor */
	bool load();
	string segment(string sentence);
};

} /* namespace std */
#endif /* Machine_H_ */
