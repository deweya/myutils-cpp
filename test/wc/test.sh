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

## Test stdin
echo "[INFO] Testing stdin"
actual=$(cat <<EOF | ./bin/wc
Never gonna give you up
Never gonna let you down
Never gonna run around and desert you
Never gonna make you cry
Never gonna say goodbye
Never gonna tell a lie and hurt you
EOF
)
expected=$(cat <<EOF | wc
Never gonna give you up
Never gonna let you down
Never gonna run around and desert you
Never gonna make you cry
Never gonna say goodbye
Never gonna tell a lie and hurt you
EOF
)
if [[ ${expected} == ${actual} ]]; then
    echo "  Stdin: PASS!"
else
    echo "  Stdin: FAIL!"
    echo "      expected: ${expected}"
    echo "      actual: ${actual}"
fi