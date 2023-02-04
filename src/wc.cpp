#include <iostream>
#include <fstream>
using namespace std;

const string USAGE = "wc [-c] [file]";

struct options {
    string file;
    bool bytes = false;
    bool lines = false;
    bool words = false;
};

struct output {
    int lines = 0;
    int words = 0;
    int bytes = 0;
    string file;
};

options processInput(int argc, char** argv) {
    options opts;

    for (int i = 1; i < argc; i++) {
        string s = argv[i];
        if (s == "-c") {
            opts.bytes = true;
        } else if (s == "-l") {
            opts.lines = true;
        } else if (s == "-w") {
            opts.words = true;
        } else {
            opts.file = s;
        }
    }
    if (!opts.bytes && !opts.lines && !opts.words) {
        opts.bytes = opts.lines = opts.words = true;
    }

    // Check that file arg is passed
    if (opts.file == "") {
        throw invalid_argument("ERR: not enough arguments\nUsage: " + USAGE);
    }

    // Check that file exists
    ifstream ifile;
    ifile.open(opts.file);
    if (!ifile) {
        throw invalid_argument("ERR: file " + opts.file + " does not exist\nUsage " + USAGE);
    }
    ifile.close();

    return opts;
}

void processEachByte(output& o, options opts) {
    ifstream ifile;
    ifile.open(o.file);
    bool word;
    while (ifile) {
        char c = ifile.get();

        if (opts.lines) {
            if (c == '\n') {
                o.lines++;
            }
        }

        if (opts.words) {
            if (c == '\n' || c == ' ' || c == -1) {
                if (word) {
                    o.words++;
                    word = false;
                }
            } else {
                word = true;
            }
        }
    }

    ifile.close();
}

void getTotalBytes(output& o) {
    ifstream ifile;
    ifile.open(o.file);

    ifile.seekg(0, ios::end);
    o.bytes = ifile.tellg();

    ifile.close();
}

void getNewlineChars(output& o) {
    ifstream ifile;
    ifile.open(o.file);

    string line;
    while (getline(ifile, line)) {
        o.lines++;
        if (o.bytes == ifile.tellg()) {
            o.lines++;
        }
    }

    o.lines--;

    ifile.close();
}

output processFile(options opts) {
    output o;
    o.file = opts.file;

    getTotalBytes(o);

    if (opts.lines && !opts.words) {
        getNewlineChars(o);
    } else {
        processEachByte(o, opts);
    }

    return o;
}

void printResult(options opts, output o) {
    if (opts.lines) {
        cout << setw(8) << o.lines;
    }
    if (opts.words) {
        cout << setw(8) << o.words;
    }
    if (opts.bytes) {
        cout << setw(8) << o.bytes;
    }
    cout << ' ' << o.file;
    cout << endl;
}

int main(int argc, char** argv) {
    options opts;

    try {
        opts = processInput(argc, argv);
    } catch(invalid_argument e) {
        cerr << e.what() << endl;
        return -1;
    }
    
    output o = processFile(opts);
    printResult(opts, o);
    
    return 0;
}