#!/bin/bash
set -eu

export CXX=/usr/bin/g++
python -u FileRevisorPyUtils/FileRevisorPyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --no-install
