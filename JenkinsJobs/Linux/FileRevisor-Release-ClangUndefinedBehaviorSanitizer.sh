#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
export UBSAN_OPTIONS=halt_on_error=true
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangUndefinedBehaviorSanitizerMode=ON" \
   --no-install
