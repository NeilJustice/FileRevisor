#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
export UBSAN_OPTIONS="silence_unsigned_overflow=1"
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangUndefinedBehaviorSanitizerMode=ON" \
   --no-install
