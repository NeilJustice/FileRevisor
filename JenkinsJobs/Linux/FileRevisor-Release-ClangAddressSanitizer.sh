#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=.
python -u FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangAddressSanitizerMode=ON -DFastLinuxReleaseBuildMode=ON" \
   --no-install
