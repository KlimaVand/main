#!/bin/bash
lcov --zerocounters --directory /home/beer/main/trunk/src
lcov --capture --initial --directory /home/beer/main/trunk/src --output-file output.info