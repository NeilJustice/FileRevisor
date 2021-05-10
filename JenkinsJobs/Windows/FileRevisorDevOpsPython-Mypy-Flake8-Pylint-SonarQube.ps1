Set-PSDebug -Trace 1

$env:PYTHONPATH="."
python.exe FileRevisorDevOpsPython\MypyFlake8PylintThenRunTestsWithCoverage.py --run-tests-with-coverage-python-file=FileRevisorDevOpsPythonTests/RunAllWithCoverage.py

Set-PSDebug -Trace 0
exit $LastExitCode
