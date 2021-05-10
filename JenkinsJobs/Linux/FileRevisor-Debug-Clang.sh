#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=.
python -u FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --no-install
