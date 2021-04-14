Set-PSDebug -Trace 1

cd FileRevisorPyUtils
$env:PYTHONPATH = '.'
python.exe FileRevisorPyUtils\CoverageLintInstallPythonLibrary.py `
	--project=FileRevisorPyUtils `
	--run-tests-with-coverage-python-file=FileRevisorPyUtilsTests/RunAllWithCoverage.py
cd ..

Set-PSDebug -Trace 0
exit $LastExitCode
