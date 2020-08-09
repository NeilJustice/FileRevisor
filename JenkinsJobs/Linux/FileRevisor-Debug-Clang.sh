#!/bin/bash
set -eu

export CXX=clang++
python -u PyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-architecture=x64 \
   --cmake-build-type=Debug \
   --tests-project-name=libFileRevisorTests \
   --no-install
