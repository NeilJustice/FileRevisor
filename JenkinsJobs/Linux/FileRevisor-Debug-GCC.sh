#!/bin/bash
set -ev

export CXX=/usr/bin/g++
export PYTHONPATH=FileRevisorPyUtils
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --no-install
