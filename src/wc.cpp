#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

const string USAGE = "wc [-c | -l | -w] [file ...]";

struct options {
    bool bytes = false;
    bool lines = false;
    bool words = false;
    vector<string> files;
};

struct output {
    int lines = 0;
    int words = 0;
    int bytes = 0;
    string file;
    bool err = false;
    string errmsg = "";
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
            opts.files.push_back(s);
        }
    }
    if (!opts.bytes && !opts.lines && !opts.words) {
        opts.bytes = opts.lines = opts.words = true;
    }

    // Check that file arg is passed
    if (opts.files.size() == 0) {
        throw invalid_argument("ERR: not enough arguments\nUsage: " + USAGE);
    }

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

bool fileExists(string file) {
    ifstream ifile;
    ifile.open(file);
    if (!ifile) {
        return false;
    }

    return true;
}

output processFile(options opts, string file) {
    output o;
    o.file = file;

    if (!fileExists(file)) {
        o.err = true;
        o.errmsg = "wc: " + file + ": open: No such file or directory";
        return o;
    }

    getTotalBytes(o);

    if (opts.lines && !opts.words) {
        getNewlineChars(o);
    } else {
        processEachByte(o, opts);
    }

    return o;
}

void printResult(options opts, vector<output> outputs) {
    for (int i = 0; i < outputs.size(); i++) {
        output o = outputs[i];

        if (o.err) {
            cout << o.errmsg << endl;
            continue;
        }

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
}

int main(int argc, char** argv) {
    options opts = processInput(argc, argv);
    vector<output> outputs;

    int totBytes = 0;
    int totLines = 0;
    int totWords = 0;
    for (int i = 0; i < opts.files.size(); i++) {
        string file = opts.files[i];
        output o = processFile(opts, file);
        outputs.push_back(o);

        totBytes += o.bytes;
        totLines += o.lines;
        totWords += o.words;
    }

    if (opts.files.size() > 1) {
        output tot;
        tot.bytes = totBytes;
        tot.lines = totLines;
        tot.words = totWords;
        tot.file = "total";

        outputs.push_back(tot);
    }

    printResult(opts, outputs);
    
    return 0;
}