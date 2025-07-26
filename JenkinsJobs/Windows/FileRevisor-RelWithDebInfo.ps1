$env:PYTHONPATH="FileRevisorDevOpsPython"
python.exe -u FileRevisorDevOpsPython\FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-build-type=RelWithDebInfo `
   --tests-project-name=libFileRevisorTests `
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=C:\" `
   --install
exit $LastExitCode
