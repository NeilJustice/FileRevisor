cd FileRevisorDevOpsPython
$env:PYTHONPATH="."
python.exe -u "FileRevisorDevOpsPython\MypyFlake8PylintThenRunTestsWithCoverage.py" --run-tests-with-coverage-python-file="FileRevisorDevOpsPythonTests/RunAllWithCoverage.py"
exit $LastExitCode
