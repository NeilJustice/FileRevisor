#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-build-type=RelWithDebInfo \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DFastLinuxReleaseBuildMode=ON" \
   --install
