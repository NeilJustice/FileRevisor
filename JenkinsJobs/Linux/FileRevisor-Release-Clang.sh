#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=/usr/local" \
   --install
