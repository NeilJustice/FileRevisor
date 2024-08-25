#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=.
export UBSAN_OPTIONS=halt_on_error=true
python -u FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangUndefinedBehaviorSanitizerMode=ON -DFastLinuxReleaseBuildMode=ON" \
   --no-install
