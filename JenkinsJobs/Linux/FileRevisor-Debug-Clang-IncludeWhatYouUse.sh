#!/usr/bin/bash
set -ev

export CXX=clang++
LinuxCPlusPlusBuilder build-cpp-program \
   --solution-name=FileRevisor \
   --configuration=Debug \
   --cmake-definitions="-DIncludeWhatYouUseMode=ON" \
   --install=false \
   --standard-output-file="/code/IncludeWhatYouUseReports/FileRevisor_iwyu_report.txt"
