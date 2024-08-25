#!/bin/bash
set -ev

export PYTHONPATH=.
python FileRevisorDevOpsPython/MypyFlake8PylintThenRunTestsWithCoverage.py --run-tests-with-coverage-python-file=FileRevisorDevOpsPythonTests/RunAllWithCoverage.py
