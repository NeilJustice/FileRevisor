#!/usr/bin/bash
set -ev

export CXX=clang++
python -u /usr/local/bin/PyUtils/BuildAndInstallCPlusPlusStaticLibrary.py \
   --solution-name=FileRevisor \
   --cmake-build-type=Debug \
   --cmake-definitions="-DIncludeWhatYouUseMode=ON" \
   --tests-project-name=FileRevisorTests \
   --do-install=false

