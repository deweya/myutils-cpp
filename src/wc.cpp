#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

const string USAGE = "wc [-c | -l | -w] [file ...]";

struct options {
    bool bytes = false;
    bool lines = false;
    bool words = false;
    vector<string> inputs;
    bool useStdin = false;
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
            opts.inputs.push_back(s);
        }
    }
    if (!opts.bytes && !opts.lines && !opts.words) {
        opts.bytes = opts.lines = opts.words = true;
    }

    // Read from stdin if the user didn't pass any files
    if (opts.inputs.size() == 0) {
        opts.useStdin = true;

        string line;
        string tmp = "";
        while (getline(cin, line)) {
            tmp += line + "\n";
        }
        opts.inputs.push_back(tmp);
    }

    return opts;
}

void processEachByte(output& o, options opts, istream& is) {
    bool word;
    while (is) {
        char c = is.get();

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
}

void getTotalBytes(output& o, istream& is) {
    is.seekg(0, is.end);
    o.bytes = is.tellg();

    is.seekg(0, is.beg);
}

void getNewlineChars(output& o, istream& is) {
    string line;
    while (getline(is, line)) {
        o.lines++;
        if (o.bytes == is.tellg()) {
            o.lines++;
        }
    }

    o.lines--;
}

void getCounts(output& o, options opts, istream& is) {
    getTotalBytes(o, is);

    if (opts.lines && !opts.words) {
        getNewlineChars(o, is);
    } else {
        processEachByte(o, opts, is);
    }
}

output processStream(options opts, string s) {
    output o;

    if (opts.useStdin) {
        stringbuf buf(s);
        istream is(&buf);
        getCounts(o, opts, is);
    } else {
        o.file = s;

        filebuf buf;
        if (!buf.open(s, ios::in)) {
            o.err = true;
            o.errmsg = "wc: " + s + ": open: No such file or directory";
            buf.close();
            return o;
        }

        istream is(&buf);
        getCounts(o, opts, is);

        buf.close();
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
        
        if (!opts.useStdin) {
            cout << ' ' << o.file;
        }
        
        cout << endl;
    }
}

int main(int argc, char** argv) {
    options opts = processInput(argc, argv);
    vector<output> outputs;

    int totBytes = 0;
    int totLines = 0;
    int totWords = 0;
    for (int i = 0; i < opts.inputs.size(); i++) {
        string s = opts.inputs[i];
        output o = processStream(opts, s);
        outputs.push_back(o);

        totBytes += o.bytes;
        totLines += o.lines;
        totWords += o.words;
    }

    if (opts.inputs.size() > 1) {
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