#!/bin/bash
make clean
make
echo "===========Sequential Simulation==========="
./rainfall_pt 2 10 0.25 4 ./test/sample_4x4.in > ./result/4x4_pt.out
python3 ./test/check.py 4 ./result/4x4_pt.out ./test/sample_4x4.out
./rainfall_pt 8 50 0.5 4096 ./measurement_4096x4096.in
