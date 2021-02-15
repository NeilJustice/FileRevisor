Set-PSDebug -Trace 1

$env:PYTHONPATH="FileRevisorPyUtils"
python.exe -u FileRevisorPyUtils\FileRevisorPyUtils\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 16 2019" `
   --cmake-build-type=Debug `
   --tests-project=libFileRevisorTests `
   --no-install

Set-PSDebug -Trace 0
exit $LastExitCode
