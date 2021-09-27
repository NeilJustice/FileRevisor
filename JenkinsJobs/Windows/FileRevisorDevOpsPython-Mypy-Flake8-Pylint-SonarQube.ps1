$env:PYTHONPATH="."
python.exe FileRevisorDevOpsPython\MypyFlake8PylintThenRunTestsWithCoverage.py --run-tests-with-coverage-python-file=FileRevisorDevOpsPythonTests/RunAllWithCoverage.py
exit $LastExitCode
