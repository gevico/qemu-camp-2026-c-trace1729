#!/bin/bash

set -e

TMP_CMD_FILE=$(mktemp)
trap 'rm -f "$TMP_CMD_FILE"' EXIT

cat >"$TMP_CMD_FILE" <<'EOF'
myfile ../exercises/20_mybash/bin/mybash
myfile ../exercises/20_mybash/obj/mybash/main.o

mysed s/unix/linux/ "unix is opensource. unix is free os."

mytrans ../exercises/20_mybash/src/mytrans/text.txt

mywc ../exercises/20_mybash/src/mytrans/text.txt
EOF

../exercises/20_mybash/bin/mybash "$TMP_CMD_FILE"
