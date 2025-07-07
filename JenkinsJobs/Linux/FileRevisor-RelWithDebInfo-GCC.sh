#!/bin/bash
set -ev

export CXX=/usr/bin/g++
export PYTHONPATH=FileRevisorDevOpsPython
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-build-type=RelWithDebInfo \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=/usr/local" \
   --install
