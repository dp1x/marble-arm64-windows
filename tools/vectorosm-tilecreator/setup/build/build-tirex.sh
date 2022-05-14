#!/bin/bash
set -x

pushd
git clone https://github.com/openstreetmap/tirex.git
cd tirex
make deb
popd
