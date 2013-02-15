#include "StrMap.h"
#include "configure.h"

namespace std {

StrMap::StrMap() {
	smap_.clear();
	size_ = 1;
}

StrMap::~StrMap() {
	smap_.clear();
}

size_t StrMap::getNum(const string str, StrMapReference ref)
{
	map<string, size_t>::iterator it = smap_.find(str);
	if (it == smap_.end()) {
		if (ref == LEARN ) {
			size_t num = size_;
			smap_.insert(pair<string, size_t>(str, num));
			size_ ++;
			return num;
		}
		else { // ref == PREDICT
			return size_ + 1;
		}
	} else {
		return it -> second;
	}
}

void StrMap::insert(pair<string, size_t> pa)
{
	smap_.insert(pa);
	size_ ++;
	return;
}

size_t StrMap::size()
{
	return smap_.size();
}

void StrMap::print(string mapfile)
{
	size_t dummy = smap_.size();
	ofstream ofs(mapfile.c_str());

	ofs << dummy << endl;
	map<string, size_t>::iterator it = smap_.begin();
	int count = 0;
	for(; it != smap_.end(); ++it) {
		ofs << it->first << " " << it->second << endl;
		count++;
	}

	ofs.close();
	return;
}

bool StrMap::load(string path)
{
	fstream ifs(path.c_str());
	if (!ifs) {
		printf("Failed to open %s", path.c_str());
		return false;
	}

	size_t index, pos;
	string buf;
	while (getline(ifs, buf)) {
		pos = buf.length() - 1;
		while (buf[pos] >= '0' && buf[pos] <= '9') --pos;
		index = 0;
		for(size_t i = pos + 1; i < buf.length(); ++i) {
			index = index * 10 + buf[i] - '0';
		}
		smap_.insert(pair<string, size_t>(buf.substr(0, pos), index));
		size_ ++;
	}

	return true;
}

} /* namespace std */
