#!/bin/bash
set -x

cd
git clone https://github.com/openstreetmap/tirex.git
cd tirex
make deb

# extract the relevant output
cd
pwd
cp *.deb /output/
