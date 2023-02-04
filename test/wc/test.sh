#!/bin/bash

TEST_DIR=test/wc
CMDS=("wc" "wc -c" "wc -l" "wc -w" "wc -c -l" "wc -c -w" "wc -w -c")

echo "[INFO] Making wc"
make wc

echo ""
echo "==============================================================================="
echo ""

## Use a single file as an argument
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

## Use multiple files as an argument
for ((i = 0; i < ${#CMDS[@]}; i++)); do
    echo "[INFO] Testing ${CMDS[i]} [file ...]"
    for f in ${TEST_DIR}/*; do
        for g in ${TEST_DIR}/*; do
            expected=$(${CMDS[i]} ${f} ${g})
            actual=$(bin/${CMDS[i]} ${f} ${g})

            if [[ ${expected} == ${actual} ]]; then
                echo "  ${f}, ${g}: PASS!"
            else
                echo "  ${f}, ${g}: FAIL!"
                echo "      expected: ${expected}"
                echo "      actual: ${actual}"
            fi
        done
    done
done