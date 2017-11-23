#!/bin/bash
lcov --no-checksum --directory /home/beer/main/trunk/src --capture --output-file output.info
mkdir output
cp output.info output
cd output
genhtml  output.info
rm output.info
