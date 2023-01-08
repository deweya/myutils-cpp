TEST_DIR=test/wc

echo "[INFO] Making wc"
make wc

echo "[INFO] Starting test..."
for f in ${TEST_DIR}/*; do
    if [[ ${f} == ${TEST_DIR}/test.sh ]]; then
        continue
    fi

    expected=$(wc ${f})
    actual=$(bin/wc ${f})

    if [[ ${expected} == ${actual} ]]; then
        echo "  ${f}: PASS!"
    else
        echo "  ${f}: FAIL!"
        echo "      expected: ${expected}"
        echo "      actual: ${actual}"
    fi
done