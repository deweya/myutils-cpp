#include <iostream>

using namespace std;

const string USAGE = "wc [file]";

struct {
    string file;
} opts;

void validateInput(int argc, char** argv) {
    if (argc == 1) {
        throw invalid_argument("ERR: argument <file> is required\nUsage: " + USAGE);
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