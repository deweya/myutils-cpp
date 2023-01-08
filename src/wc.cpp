#include <iostream>
#include <fstream>
using namespace std;

const string USAGE = "wc [file]";

struct {
    string file;
} opts;

void validateInput(int argc, char** argv) {
    // Check that file arg is passed
    if (argc == 1) {
        throw invalid_argument("ERR: argument <file> is required\nUsage: " + USAGE);
    }
    string file = argv[1];

    // Check that file exists
    ifstream ifile;
    ifile.open(argv[1]);
    if (!ifile) {
        throw invalid_argument("ERR: file " + file + " does not exist\nUsage " + USAGE);
    }
}

int main(int argc, char** argv) {
    try {
        validateInput(argc, argv);
    } catch(invalid_argument e) {
        cerr << e.what() << endl;
        return -1;
    }

    opts.file = argv[1];
    cout << opts.file << endl;
}