#!/bin/bash
set -ev

export CXX=clang++
LinuxCPlusPlusBuilder build-cpp-solution \
   --solution-name=FileRevisor \
   --configuration=Debug \
   --cmake-definitions="-DFastLinuxDebugBuildMode=ON" \
   --install=false

LinuxCodeCoverageRunner measure-cplusplus-code-coverage \
   --solution=FileRevisor \
   --tests-project=libFileRevisorTests
