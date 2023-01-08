#include <iostream>
#include <fstream>
using namespace std;

const string USAGE = "wc [file]";

struct options {
    string file;
};

struct output {
    int lines = 0;
    int words = 0;
    int bytes = 0;
    string file;
};

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
    ifile.close();
}

output processFile(options opts) {
    output o;
    o.file = opts.file;

    ifstream ifile;
    ifile.open(opts.file);
    bool word;
    while (ifile) {
        char c = ifile.get();
        if (c != -1) {
            o.bytes++;
        }
        
        if (c == '\n') {
            o.lines++;
            if (word) {
                o.words++;
                word = false;
            }
        } else if (c == ' ') {
            if (word) {
                o.words++;
                word = false;
            }
        } else {
            word = true;
        }
    }

    ifile.close();
    return o;
}

int main(int argc, char** argv) {
    try {
        validateInput(argc, argv);
    } catch(invalid_argument e) {
        cerr << e.what() << endl;
        return -1;
    }

    options opts;
    opts.file = argv[1];
    
    output o = processFile(opts);
    cout << setw(8) << o.lines;
    cout << setw(8) << o.words;
    cout << setw(8) << o.bytes;
    cout << ' ' << o.file;
    cout << endl;
    
    return 0;
}