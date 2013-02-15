#ifndef STRMAP_H_
#define STRMAP_H_

#include "configure.h"

#include <map>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <iostream>

namespace std {

class StrMap {
private:
	map<string, size_t> smap_;
	size_t size_;

public:
	StrMap();
	virtual ~StrMap();
	size_t getNum(const string str, StrMapReference ref);
	size_t size();
	void insert(pair<string, size_t> pa);
	void print(string mapfile);
	bool load(string path);
	map<string, size_t>::iterator begin() {return smap_.begin(); };
	map<string, size_t>::iterator end() {return smap_.end(); };
};

} /* namespace std */
#endif /* STRMAP_H_ */
