Set-PSDebug -Trace 1

$env:PYTHONPATH="FileRevisorDevOpsPython"
python.exe -u FileRevisorDevOpsPython\FileRevisorDevOpsPython\BuildAndInstallCPlusPlusProgram.py `
   --solution-name=FileRevisor `
   --cmake-generator="Visual Studio 16 2019" `
   --cmake-build-type=Release `
   --tests-project-name=libFileRevisorTests `
   --cmake-definitions="-DCMAKE_INSTALL_PREFIX=C:\" `
   --install

Set-PSDebug -Trace 0
exit $LastExitCode
