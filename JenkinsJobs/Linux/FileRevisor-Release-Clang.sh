#!/bin/bash
set -eu

export CXX=clang++
python -u PyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-architecture=x64 \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=/usr/local" \
   --install
