$env:PYTHONPATH="."
python.exe -u FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 17 2022" `
   --cmake-build-type=RelWithDebInfo `
   --tests-project-name=libFileRevisorTests `
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=C:\" `
   --install
exit $LastExitCode
