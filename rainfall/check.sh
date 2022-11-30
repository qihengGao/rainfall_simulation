#!/bin/bash
make clean
make
echo "===========Sequential Simulation==========="
./rainfall_pt 2 10 0.25 4 ./test/sample_4x4.in
