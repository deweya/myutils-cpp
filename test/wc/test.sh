#!/bin/bash

TEST_DIR=test/wc
CMDS=("wc" "wc -c" "wc -l" "wc -w" "wc -c -l" "wc -c -w" "wc -w -c")

echo "[INFO] Making wc"
make wc

echo ""
echo "==============================================================================="
echo ""

## Verify that the output is correct
for ((i = 0; i < ${#CMDS[@]}; i++)); do
    echo "[INFO] Testing ${CMDS[i]} [file]"
    for f in ${TEST_DIR}/*; do
        expected=$(${CMDS[i]} ${f})
        actual=$(bin/${CMDS[i]} ${f})

        if [[ ${expected} == ${actual} ]]; then
            echo "  ${f}: PASS!"
        else
            echo "  ${f}: FAIL!"
            echo "      expected: ${expected}"
            echo "      actual: ${actual}"
        fi
    done
done