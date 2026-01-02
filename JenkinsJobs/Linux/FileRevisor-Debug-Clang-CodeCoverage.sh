#!/bin/bash
set -ev

export CXX=clang++
export PYTHONPATH=FileRevisorDevOpsPython
python -u FileRevisorDevOpsPython/FileRevisorDevOpsPython/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangCodeCoverageMode=ON" \
   --no-install

LinuxCodeCoverageRunner measure-cplusplus-code-coverage --solution=FileRevisor --tests-project=libFileRevisorTests
