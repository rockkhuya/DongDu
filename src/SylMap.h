#ifndef SYLMAP_H_
#define SYLMAP_H_

#include <fstream>
#include <set>
#include <string>
#include <iostream>

namespace std {

class SylMap {
private:
	set<string> _syl;
public:
	SylMap();
	virtual ~SylMap();
	bool isVNESE(string syllabel);
};

} /* namespace std */
#endif /* SYLMAP_H_ */
