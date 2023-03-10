wc:
	clang++ -std=gnu++17 -fcolor-diagnostics -fansi-escape-codes -g src/wc.cpp -o bin/wc

wc-test:
	./test/wc/test.sh

wc-quality:
	cppcheck --std=c++17 --platform=unix64 --enable=all src/wc.cpp