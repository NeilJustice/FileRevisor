#!/bin/bash
set -ev

export CXX=/usr/bin/g++
export PYTHONPATH=FileRevisorPyUtils
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=/usr/local" \
   --install
