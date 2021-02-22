#!/bin/bash
set -eu

export CXX=clang++
export UBSAN_OPTIONS=halt_on_error=true
export PYTHONPATH=FileRevisorPyUtils
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangSanitizersMode_AddressAndUndefined=ON" \
   --no-install
