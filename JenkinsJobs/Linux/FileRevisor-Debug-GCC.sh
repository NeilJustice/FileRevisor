#!/bin/bash
set -ev

export CXX=/usr/bin/g++
export PYTHONPATH=.
python -u FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DFastLinuxDebugBuildMode=ON" \
   --no-install
