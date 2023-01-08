TEST_DIR=test/wc
CMDS=("wc" "wc -c" "wc -l" "wc -w")

echo "[INFO] Making wc"
make wc

for ((i = 0; i < ${#CMDS[@]}; i++)); do
    echo "[INFO] Testing ${CMDS[i]} [file]"
    for f in ${TEST_DIR}/*; do
        if [[ ${f} == ${TEST_DIR}/test.sh ]]; then
            continue
        fi

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