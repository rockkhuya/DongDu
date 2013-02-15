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
	cout << "./predictor -i input_file_path -o output_file_path" << endl;
	return;
}

int main(int argc, char **argv)
{
	if (argc != 5) {
		usage();
		return 1;
	}

	extern char *optarg;
	int opt;

	string inputfile, outputfile;
	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch (opt) {
		  case 'i':
			  inputfile = optarg;
			  break;
			case 'o':
				outputfile = optarg;
				break;
			default:
				break;
		}
	}

	//Predictor
	ifstream ifs1(inputfile.c_str());
	if (!ifs1) {
		cout << "Failed to open " << inputfile << endl;
		usage();
		return 1;
	}

	Machine predictor(3, "", PREDICT);
	if (!predictor.load()) {
		cout << "Failed to load data from dongdu.model and dongdu.map" << endl;
		return 1;
	}

	clock_t start = clock(), finish;
	cout << "Start segmenting ..." << endl;
	ofstream ofs(outputfile.c_str());
	string buf;
	size_t number_lines = 0;
	while (getline(ifs1, buf)) {
		ofs << predictor.segment(buf) << endl;
		number_lines ++;
	}

	cout << "End segmenting." << endl;
	finish = clock();
	printf("Segment %d line(s). \n", number_lines);
	printf("Segmentation took %f seconds to execute\n",
	         ((double) (finish - start)) / CLOCKS_PER_SEC); //

	ofs.close();
	ifs1.close();

	return 0;
}
