$env:PYTHONPATH="."
python.exe -u FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 17 2022" `
   --cmake-build-type=Debug `
   --tests-project=libFileRevisorTests `
   --cmake-definitions="" `
   --no-install
exit $LastExitCode
