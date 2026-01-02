#!/usr/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-build-type=Debug \
   --cmake-definitions="-DIncludeWhatYouUseMode=ON" \
   --tests-project-name=FileRevisorTests \
   --no-install

