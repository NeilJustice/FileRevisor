Set-PSDebug -Trace 1

$env:PYTHONPATH="."
python.exe -u FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 16 2019" `
   --cmake-build-type=Debug `
   --tests-project=libFileRevisorTests `
   --cmake-definitions="-DFastWindowsDebugBuildMode=ON" `
   --no-install

Set-PSDebug -Trace 0
exit $LastExitCode
