Set-PSDebug -Trace 1

$env:PYTHONPATH="."
python.exe -u FileRevisorPyUtils\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 16 2019" `
   --cmake-build-type=Debug `
   --tests-project-name=libFileRevisorTests `
   --no-install

Set-PSDebug -Trace 0
exit $LastExitCode
