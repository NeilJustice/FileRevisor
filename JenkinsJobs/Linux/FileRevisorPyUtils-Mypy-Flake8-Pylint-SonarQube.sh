#!/bin/bash
set -ev

cd FileRevisorPyUtils
export PYTHONPATH='.'
python FileRevisorPyUtils/MypyFlake8PylintThenRunTestsWithCoverage.py --run-tests-with-coverage-python-file=FileRevisorPyUtilsTests/RunAllWithCoverage.py
cd ..
