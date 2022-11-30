#!/bin/bash
make clean
make rainfall_seq
echo "===========Sequential Simulation==========="
# ./rainfall_seq 8 30 0.25 128 ./test/sample_128x128.in > ./result/seq_128x128.out
# python3 ./test/check.py 128 ./test/sample_128x128.out ./result/seq_128x128.out
./rainfall_seq 8 30 0.75 512 ./test/sample_512x512.in > ./result/seq_512x512.out
python3 ./test/check.py 512 ./test/sample_512x512.out ./result/seq_512x512.out

# ./rainfall_seq 8 50 0.5 4096 ./test/measurement_4096x4096.in > ./result/seq_4096x4096.out
# python3 ./test/check.py 4096 ./test/measurement_4096x4096.out ./result/seq_4096x4096.out

