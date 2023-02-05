wc:
	clang++ -std=gnu++17 -fcolor-diagnostics -fansi-escape-codes -g src/wc.cpp -o bin/wc

wc-quality:
	cppcheck --std=c++17 --enable=all src/wc.cpp