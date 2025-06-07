$env:PYTHONPATH="FileRevisorDevOpsPython"
python.exe -u FileRevisorDevOpsPython\FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-build-type=Debug `
   --tests-project=libFileRevisorTests `
   --cmake-definitions="" `
   --no-install
exit $LastExitCode
