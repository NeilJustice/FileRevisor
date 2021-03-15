#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorPyUtils
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangThreadSanitizerMode=ON" \
   --no-install
