/* Predictor - written by Luu Tuan Anh, Nagaoka University of Technology
 * Date : 18/09/2012
 * Completed : 27/09/2012
 */
#include "Machine.h"
#include "FeaturesSelection.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

void usage()
{
	cout << "./learner -i corpus_link -o model&map_folder_path -w window_length" << endl;
	return;
}

int main(int argc, char **argv)
{
	if (argc != 7) {
		usage();
		return 1;
	}

	extern char *optarg;
	int opt;
	string corpus_link, PATH;
	int WINDOW_LENGTH;
	while ((opt = getopt(argc, argv, "i:o:w:")) != -1) {
		switch (opt) {
		  case 'i':
			  corpus_link = optarg;
			  break;
			case 'o':
				PATH = optarg;
				break;
			case 'w':
				WINDOW_LENGTH = atoi(optarg);
				break;
			default:
				break;
		}
	}

	ifstream ifs(corpus_link.c_str()); // filename
	if ( !ifs ) {
		cout << "Failed to open corpus file. " << endl;
		return 1;
	}

	// Learner
	cout << "Learning..." << endl;
	string sentence;
	Machine learner(WINDOW_LENGTH, PATH, LEARN);
	size_t line_num = 0;
	clock_t start = clock(), finish;
	while (ifs && getline(ifs, sentence)) {
		learner.extract(sentence, LEARN);
		line_num ++;
		if (line_num % 10000 == 0)
			cout << line_num << endl;
	}
	cout << line_num << " line(s)." << endl; // prints
	learner.training();
	learner.close_test();
	learner.print();
	ifs.close();

	// Features Selection
	cout << "Features selection..." << endl;
	FeaturesSelection fselect(PATH);
	fselect.selection();
	fselect.save();

	finish = clock();
	printf("Learning took %f seconds\n",
	         ((double) (finish - start)) / CLOCKS_PER_SEC); //

	return 0;
}
