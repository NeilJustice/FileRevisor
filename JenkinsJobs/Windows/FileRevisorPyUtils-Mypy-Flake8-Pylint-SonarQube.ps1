Set-PSDebug -Trace 1

cd FileRevisorPyUtils
$env:PYTHONPATH = '.'
python.exe FileRevisorPyUtils\MypyFlake8PylintThenRunTestsWithCoverage.py --run-tests-with-coverage-python-file=FileRevisorPyUtilsTests/RunAllWithCoverage.py
cd ..

Set-PSDebug -Trace 0
exit $LastExitCode
