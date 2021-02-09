Set-PSDebug -Trace 1

$env:PYTHONPATH = '.'
python.exe FileRevisorPyUtils\CoverageLintInstallPythonLibrary.py --project=PyUtils --run-tests-with-coverage-python-file=FileRevisorPyUtilsTests/RunAllWithCoverage.py

Set-PSDebug -Trace 0
exit $LastExitCode
