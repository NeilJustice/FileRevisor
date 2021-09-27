param($fastWindowsDebugBuildMode = "ON")

$env:PYTHONPATH="."
python.exe -u FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 16 2019" `
   --cmake-build-type=Debug `
   --tests-project=libFileRevisorTests `
   --cmake-definitions="-DFastWindowsDebugBuildMode=$fastWindowsDebugBuildMode" `
   --no-install
exit $LastExitCode
