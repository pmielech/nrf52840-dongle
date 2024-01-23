#!/bin/bash
# compile proj
#west build --build-dir /Users/patryk/github/nrf52840-dongle/build /Users/patryk/github/nrf52840-dongle

#upload
# nrfjprog --program build/zephyr/zephyr.hex --sectorerase

# Run after builds scripts 
# ./hex-edit.sh