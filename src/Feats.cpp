#include "Feats.h"

namespace std {

Feats::Feats()
{
	feats_.clear();
}

Feats::~Feats()
{
	vector<Feat*>::iterator it = feats_.begin();
	for(; it != feats_.end(); ++it ) {
		delete (*it)->second;
		delete *it;
	}
	feats_.clear();
}

size_t Feats::size()
{
	return feats_.size();
}

vector<Feat*>* Feats::get()
{
	return &feats_;
}

void Feats::add(Feat* f)
{
	feats_.push_back(f);
	return;
}

string Feats::type(string word)
{
	bool VH = false;
	bool vt = false;
	bool cs = false;
	bool kh = false;

	for(int i = 0; i < int(word.length()); ++i){
		if (int(SYMBOLS.find(word[i])) > -1)  kh = true;
		if (word[i] >= '0' && word[i] <= '9') cs = true;
		if (word[i] >= 'A' && word[i] <= 'Z') VH = true;
		if (word[i] >= 'a' && word[i] <= 'z') vt = true;
	}

	/*
	 * O : other
	 * N : number
	 * U : Upper
	 * L : lower
	 */
	if (kh) return "O";
	if (cs && (!(VH || vt))) return "N";
	if (cs && (VH || vt)) return "O";
	if (_syl.isVNESE(word)) {
		if (VH) return "U";
		if (vt) return "L";
	}
	return "O";
}

/* Regular Expressions */
string Feats::regex(string text, FeatsReference ref)
{
	string ans = "";

	// replace some UTF-8 char by one byte char
	string source[3] = {"…", "“", "”"};
	string replace[3]= {"...", "\"", "\""};
	for(int i = 0; i < 3; ++i) {
		size_t position = text.find(source[i]);
		while (position != string::npos) {
			text.replace(position, source[i].length(), replace[i]);
			position = text.find(source[i], position + sizeof(replace[i]));
		}
	}

	// segment symbols
	if (ref == PREDICT) {
		for(size_t i = 0; i < text.length(); ++i) {
			if (SYMBOLS.find(text[i]) != string::npos) {
  		   text.insert(i+1, " ");
	   	   text.insert(i, " ");
	  			i += 2;
			}
		}
	}

	// remove consecutive space and underscore
	for(size_t i = 0; i < text.length(); ++i) {
		if ((text[i] == SPACE || text[i] == UNDER)
				&& (text[i-1] == SPACE || text[i-1] == UNDER)) {
			// do nothing
		}
		else {
			ans += text[i];
		}
	}

	return ans;
}

vector<featuresOfSyllabel>* Feats::token(string text, FeatsReference ref)
{
	text = regex(text, ref);
	text += SPACE;

	vector<featuresOfSyllabel>* ans = new vector<featuresOfSyllabel>;
	featuresOfSyllabel dummy;
	size_t pos = 0, prev = -1, N = text.length();
	bool segment;
	ans->clear();

	for (pos = 0; pos < N; ++pos) {
		if (ref == LEARN) /* Learning OR training */
			segment = (text[pos] == SPACE || text[pos] == UNDER);
		else
			segment = (text[pos] == SPACE);

		if (segment) {
			if (pos == prev + 1) {
				prev = pos;
				continue;
			}
			dummy.syllabel	= text.substr(prev + 1, pos - prev - 1);
			dummy.type			=	type(dummy.syllabel);
			text[pos] == SPACE ? dummy.label = 1 : dummy.label = 2;
			prev	= pos;
			ans->push_back(dummy);
		}
	}

	return ans;
}

void Feats::erase(size_t x)
{
	set<size_t>::iterator it;

	for(size_t i = 0; i < feats_.size(); ++i) {
		it = feats_[i]->second->find(x);
		if (it != feats_[i]->second->end()) // found
			feats_[i]->second->erase(it);
	}
}

void Feats::clear()
{
	vector<Feat*>::iterator it = feats_.begin();
	for(; it != feats_.end(); ++it ) {
		delete (*it)->second;
		delete *it;
	}
	feats_.clear();
	return;
}
} /* namespace std */
