#!/bin/bash

TEST_DIR=test/wc
CMDS=("wc" "wc -c" "wc -l" "wc -w")

echo "[INFO] Making wc"
make wc

echo ""
echo "==============================================================================="
echo ""

## Compare the runtime against the original tool
for ((i = 0; i < ${#CMDS[@]}; i++)); do
    echo "[INFO] Comparing runtimes for ${CMDS[i]} [file]"
    for f in ${TEST_DIR}/*; do
        echo "  benchmark:"
        time ${CMDS[i]} ${f}
        echo "  actual:"
        time bin/${CMDS[i]} ${f}
        echo ""
    done
done