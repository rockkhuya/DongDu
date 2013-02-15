#include "FeaturesSelection.h"

namespace std {

FeaturesSelection::FeaturesSelection(string str)
{
	PATH = str;
	string dummy;
	cout << "Load strmap" << endl;
	dummy = PATH + "dongdu.map";
	_strmap.load(dummy.c_str());
	cout << "Load model" << endl;
	dummy = PATH + "dongdu.model";
	_model = load_model(dummy.c_str());

	return;
}

FeaturesSelection::~FeaturesSelection()
{
	_strmap.~StrMap();
	delete _model;
}

/* a double is 0 ? */
bool isZero(double x) {
	if (x < 1e-10 && x > -1e-10) return true;
	return false;
}

void FeaturesSelection::selection()
{
	cout << "Start selection" << endl;

	// convert map<string, size_t> to string's array[size_t]
	size_t mapsize = _strmap.size(); 
	cout << mapsize << endl;
	string dummy[mapsize + 1]; 
	memset(dummy, sizeof(dummy), 0);
	map<string, size_t>::iterator it = _strmap.begin();
	for(; it != _strmap.end(); ++it)
		dummy[it->second] = it->first;

	// find features that have weight is 0
	int i, n, nr_feature = _model->nr_feature;
	(_model->bias >= 0) ? n = nr_feature + 1 : n = nr_feature;
	int w_size = n;

	set<int> unused; 
	unused.clear();
	for(i = 0; i < w_size; i++) {
		if (isZero(_model->w[i])) unused.insert(i + 1);
	}
	// create new strmap
	for(size_t i = 1; i < mapsize; ++i) {
		if (unused.find(i) == unused.end()) {
			new_strmap.getNum(dummy[i], LEARN);
		}
	}
	cout << "Unused features is " << unused.size() << endl;
	return;
}

void FeaturesSelection::save()
{
	string dummy;
	// save new strmap
	dummy = PATH + "dongdu.map";
	new_strmap.print(dummy.c_str());

	// save new model
	dummy = PATH + "dongdu.model";
	FILE* ofs = fopen(dummy.c_str(), "w");
	fprintf(ofs, "solver_type L1R_LR\n");
	fprintf(ofs, "nr_class 2\n");
	fprintf(ofs, "label 1 2\n");
	fprintf(ofs, "nr_feature %d\n", new_strmap.size());
	fprintf(ofs, "bias -1\n");
	fprintf(ofs, "w\n");

	int i, n, nr_feature = _model->nr_feature;
	(_model->bias >= 0) ? n = nr_feature + 1 : n = nr_feature;
	int w_size = n;

	for(i = 0; i < w_size; i++) {
		if (!isZero(_model->w[i])) fprintf(ofs, "%.15lf\n", _model->w[i]);
	}

	return;
}

} /* namespace std */
