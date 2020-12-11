#!/bin/bash
set -eu

export CXX=clang++
export UBSAN_OPTIONS=halt_on_error=true
python -u PyUtils/BuildAndInstallCPlusPlusProgram.py \
   --solution-name=FileRevisor \
   --cmake-generator=Ninja \
   --cmake-architecture=x64 \
   --cmake-build-type=Release \
   --tests-project-name=libFileRevisorTests \
   --cmake-definitions="-DClangSanitizersMode_AddressAndUndefined=ON" \
   --no-install
