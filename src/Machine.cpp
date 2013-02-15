#include "Machine.h"

namespace std {

/* return number of bytes of a UTF8 char */
size_t Machine::getByteOfUTF8(unsigned char c)
{
  if      (c <= 0x7F)             { return 1; }
  else if (0xC2 <= c && c <= 0xDF){ return 2; }
  else if (0xE0 <= c && c <= 0xEF){ return 3; }
  else if (0xF0 <= c && c <= 0xF7){ return 4; }
  else if (0xF8 <= c && c <= 0xFB){ return 5; }
  else if (0xFC <= c && c <= 0xFD){ return 6; }
  else { return 0; } // c is not head byte of character
}

Machine::Machine(int window_length, string path, StrMapReference ref)
{
	WINDOW_LENGTH = window_length;
	PATH 				= path;
	index_SPACE = 1;
	index_UNDER = 2;
	reference   = ref;
	feats				= new Feats;
	_model 			= new model;
	vfeats 			= new vector<featuresOfSyllabel>;
	vfeats->clear();
	if (ref == LEARN ) extract("một ví_dụ", ref); // dont erase it.
}

Machine::~Machine()
{
	delete feats;
}

/* Convert a string to vector of featuresOfSyllabel */
void Machine::convert(string sentence)
{
	// delete control characters, such as Return, tabs, ... and SPACE
	// in begin and end of string. Control characters's oct is smaller than 41.
	while (sentence[0] <= 40 && sentence.length() > 0) sentence.erase(0);

	size_t length = sentence.length();
	while (sentence[length - 1] <= 40 && length > 0) sentence.erase(--length);

	for(int i = 0; i < WINDOW_LENGTH; ++i)
		sentence = "BOS " + sentence + " BOS";
	vfeats->clear();
	vfeats = feats->token(sentence, reference);
	return;
}

/* Convert a integer(single char) to string */
string Machine::itostr(int x)
{
	string ans = "";
	(x < 0) ? ans = "-" : ans = "+"; // sign of x
	if (x < 0) x = -x;
	ans += x + '0';

	return ans;
}

/* Convert a string to vfeats, extract features and put it in feats */
void Machine::extract(string sentence, StrMapReference ref)
{
	// convert sentence(string) to vfeats;
	convert(sentence);

	int length = vfeats->size();
	int label;
	string index, dummy;

	// debug
	for (int i = WINDOW_LENGTH; i < length - WINDOW_LENGTH - 1; ++i) {
		set<size_t>* featset = new set<size_t>;
		featset->clear();
		// get 1-gram
		for (int j = i - WINDOW_LENGTH + 1; j <= i + WINDOW_LENGTH; ++j) {
			index = itostr(j-i) + "|";
			featset->insert(strmap.getNum(index + vfeats->at(j).syllabel, ref));
			featset->insert(strmap.getNum(index + vfeats->at(j).type    , ref));
		}
		// get 2-gram
		for (int j = i - WINDOW_LENGTH + 1; j < i + WINDOW_LENGTH; ++j) {
			index = itostr(j-i) + "||";
			featset->insert(strmap.getNum(index + vfeats->at(j).syllabel + " " + vfeats->at(j+1).syllabel, ref));
			featset->insert(strmap.getNum(index + vfeats->at(j).type     + " " + vfeats->at(j+1).type    , ref));
		}
		// get Dictionary-features
		for (int j = 1; j < MAX_WORD_LENGTH; ++j) {
			for (int k = i - j + 1; k <= i + 1; ++k ) {
				dummy = vfeats->at(k).syllabel ;
				for (int z = k + 1; z < k + j; ++z)
					dummy += " " + vfeats->at(z).syllabel;
				if (dicmap.isWord(dummy)){
					// word segment is LEFT of dictionary features
					if (				k	== i + 1 ) 	index = "L(" + itostr(k - i) + ")|";
					// word segment is RIGHT of dictionary features
					if (k + j - 1 == i		 ) 	index = "R(" + itostr(k - i) + ")|";
					// word segment is INSIDE of dictionary features
					if (k <= i && k+j-1 > i)	index = "I(" + itostr(k - i) + ")|";
					featset->insert(strmap.getNum(index + dummy, ref));
				}
			}
		}
		// get label
		(vfeats->at(i).label == 1) ? label = index_SPACE : label = index_UNDER;
		if (featset->size() > 0) {
			Feat* thisfeat 	= 		new Feat(label, featset);
			feats->add(thisfeat);
		}
	} // end of i
}

/* Convert a feats format to liblinear's problem struct */
void Machine::getProblem()
{
	size_t sizeOfFeats = feats->size();
	feature_node** x = new feature_node*[sizeOfFeats];
	double* y = new double[sizeOfFeats];

	for (size_t i = 0; i < sizeOfFeats; ++i) {
		y[i] = feats->get()->at(i)->first;
		feature_node* xx = new feature_node[feats->get()->at(i)->second->size()+1];
		x[i] = xx;
		size_t j = 0;
		set<size_t>::iterator it = feats->get()->at(i)->second->begin();
		for ( ; it != feats->get()->at(i)->second->end(); ++it) {
			xx[j].index = *it;
			xx[j].value = 1;
			++j;
		}
		xx[j].index = -1;
	}

	_problem.l 		= sizeOfFeats;
	_problem.n 		= strmap.size();
	_problem.y 		= y;
	_problem.x 		= x;
	_problem.bias = -1;

	return;
}

void Machine::delProblem()
{
	for (int i = 0; i < _problem.l; ++i) delete[] _problem.x[i];
	delete[] _problem.x;
	delete[] _problem.y;

	return;
}

/* Train problem by Liblinear L1R_LR */
void Machine::training()
{
	// init parameter
	parameter _parameter;
	_parameter.solver_type = L1R_LR; // type of Machine learning Algorithm
	_parameter.eps = 0.01;
	_parameter.C = 1;
	_parameter.nr_weight = 0;
	_parameter.weight_label = new int(1);
	_parameter.weight = new double(1.0);

	clock_t start, finish;

	start = clock();

	cout << "Start training ..." << endl;
	getProblem();
	_model = train( &_problem, &_parameter);
	cout << "Finish training. " 	<< endl;

	finish = clock();
	printf("training took %f seconds to execute\n",
	         ((double) (finish - start)) / CLOCKS_PER_SEC); //

	return;
}

/* print dongdu.model and dongdu.map */
void Machine::print()
{
	string 	modelfile = PATH + "dongdu.model";
	cout << "Save model file : " << modelfile << endl;
	save_model(modelfile.c_str(), _model);

	// write map file from strmap
	string mapfile = PATH + "dongdu.map";
	cout << "Save map file : " << mapfile << endl;
	strmap.print(mapfile);

	return;
}

/* do close test */
double Machine::close_test()
{
	int count = 0;
	for (size_t i = 0; i < feats->size(); ++i) {
		if (predict(_model, _problem.x[i]) != feats->get()->at(i)->first){
		  count++;
		}
	}

	double P = 100.0-double(count)/double(feats->size())*100.0;
	cout  << "Close test : " << P << "%" << endl;
	cout  << count << " : "<< feats->size() << endl;

	return P;
}

/* a double is 0 ? */
bool zero(double x) {
	if (x < 1e-10 && x > -1e-10) return true;
	return false;
}

/* do features selection */
void Machine::featuresSelection()
{
	int i;
	int nr_feature = _model->nr_feature;
	int n;

	(_model->bias >= 0) ? n = nr_feature + 1 : n = nr_feature;
	int w_size = n;

	for(i = 0; i < w_size; i++) {
		// unused features
		if (zero(_model->w[i])) {
			// find i in feats and delete it-
		}
	}

	return;
}

bool Machine::load()
{
	cout << "Start loading ..." << endl;
	_model = load_model("/home/anh/workspace/dongdu/Exp/newdongdu.model");
	if (   !strmap.load("/home/anh/workspace/dongdu/Exp/newdongdu.map")){
		cout << "End loading ..." << endl;
		return false;
	}
	cout << "End loading ..." << endl;
	return true;
}

string Machine::segment(string sentence)
{
	// initialize feats
	feats->clear();

	// extract sentence to feats
	extract(sentence, PREDICT);
	if (feats->size() == 0)	return "";

	// convert feats to Liblinear's problem struct
	getProblem();

	string ans = "";
	for (size_t i = 0; i < feats->size(); ++i) {
		if (predict(_model, _problem.x[i]) == index_SPACE)
			ans += vfeats->at(i + WINDOW_LENGTH).syllabel + SPACE;
		else
			ans += vfeats->at(i + WINDOW_LENGTH).syllabel + UNDER;
	}
	ans += vfeats->at(feats->size() + WINDOW_LENGTH).syllabel;

	while (ans[0] <= 40 && ans.length() > 0) ans.erase(0);
	string dummy = "";
	dummy += ans[0];
	for(size_t i = 1; i < ans.length(); ++i) {
		if ((ans[i] == SPACE || ans[i] == UNDER)
				&& (ans[i-1] == SPACE || ans[i-1] == UNDER)) {
			// do nothing
		}
		else {
			dummy += ans[i];
		}
	}

	// check the last character. If last character is '.' and previous character
	// is underscore, we will replace underscore by space
	if (dummy[dummy.size()-1] == '.' && dummy[dummy.size()-2] == UNDER) {
		dummy[dummy.size()-2] = SPACE;
	}

	//delete problem and free memory
	delProblem();
	feats->clear();

	return dummy;
}

} /* namespace std */
