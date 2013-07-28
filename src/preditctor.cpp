/* Predictor - written by Luu Tuan Anh, Nagaoka University of Technology
 * Date : 18/09/2012
 * Completed : 27/09/2012
 */
#include "Machine.h"
#include "FeaturesSelection.h"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "predictor.h"

using namespace std;

void predictor_usage()
{
	cout << "Usage: dongdu predict -i input_file_path -o output_file_path" << endl;
	return;
}

void segment(Machine &predictor, string &inputfile, string &outputfile, int &number_lines) {
	ifstream ifs1(inputfile.c_str());
	if (!ifs1) {
		cout << "Failed to open " << inputfile << endl;
	}

	ofstream ofs(outputfile.c_str());
	string buf;
	while (getline(ifs1, buf)) {
		ofs << predictor.segment(buf) << endl;
		number_lines ++;
	}

	ofs.close();
	ifs1.close();
}

bool endsWith (std::string const &fullString, std::string const &ending)
{
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (
        		fullString.length() - ending.length(), ending.length(), ending));
    }
    return false;
}

void ensureEndingSlash(string& str) {
	if (!endsWith(str, "/")) {
		str.append("/");
	}
}

int predictor_main(int argc, char **argv)
{
	if (argc != 5) {
		predictor_usage();
		return 1;
	}

	extern char *optarg;
	int opt;

	string input, output;
	while ((opt = getopt(argc, argv, "i:o:")) != -1) {
		switch (opt) {
		  case 'i':
			  input = optarg;
			  break;
			case 'o':
				output = optarg;
				break;
			default:
				break;
		}
	}

	clock_t start = clock(), finish;
	int number_lines = 0;

	//Predictor
	Machine predictor(3, "", PREDICT);
	if (!predictor.load()) {
		cout << "Failed to load data from dongdu.model and dongdu.map" << endl;
		return 1;
	}

	cout << "Start segmenting ..." << endl;

	struct stat st;
	lstat(input.c_str(), &st);
	if(S_ISDIR(st.st_mode)) {
		ensureEndingSlash(input);
		ensureEndingSlash(output);

		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(input.c_str())) != NULL) {
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL) {
				std::string inputfile = input + ent->d_name;
				std::string outputfile = output + ent->d_name;
				segment(predictor, inputfile, outputfile, number_lines);
			}
			closedir(dir);
		} else {
			cout << "Error opening dir: " << input.c_str() << endl;
			return 2;
		}
	} else {
		segment(predictor, input, output, number_lines);
	}

	cout << "End segmenting." << endl;
	finish = clock();
	printf("Segment %d line(s). \n", number_lines);
	printf("Segmentation took %f seconds to execute\n",
	         ((double) (finish - start)) / CLOCKS_PER_SEC); //

	return 0;
}
