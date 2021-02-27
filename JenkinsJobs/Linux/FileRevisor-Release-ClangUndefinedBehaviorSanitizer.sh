#!/bin/bash
set -eu

export CXX=clang++
export PYTHONPATH=FileRevisorPyUtils
export UBSAN_OPTIONS=halt_on_error=true
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangUndefinedBehaviorSanitizerMode=ON" \
   --no-install
