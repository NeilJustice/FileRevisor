#!/bin/bash
set -eu

cd FileRevisorPyUtils
export PYTHONPATH='.'
python FileRevisorPyUtils/CoverageLintInstallPythonLibrary.py --project=FileRevisorPyUtils --run-tests-with-coverage-python-file=FileRevisorPyUtilsTests/RunAllWithCoverage.py
cd ..

