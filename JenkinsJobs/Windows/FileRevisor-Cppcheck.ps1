$numberOfLogicalProcessors = (Get-CimInstance -ClassName Win32_ComputerSystem).NumberOfLogicalProcessors
cppcheck.exe `
   --enable=all `
   --cppcheck-build-dir=Cppcheck `
   --suppressions-list=CppcheckSuppressions.txt `
   -D_WIN32 `
   -DTEST `
   -DTESTS `
   -DMETALMOCK_NONVOID0_FREE `
   -DMETALMOCK_NONVOID1_FREE `
   -DMETALMOCK_NONVOID5_FREE `
   -DMETALMOCK_NONVOID2_STATIC `
   -DRUN_TEMPLATE_TESTS `
   -DTHEN_RUN_TEMPLATE_TESTS `
   -DAFACT `
   -DMETALMOCK_NONVOID0_CONST `
   -DMETALMOCK_VOID2_CONST `
   -DMETALMOCK_VOID1_CONST `
   -DMETALMOCK_NONVOID0_CONST `
   -DMETALMOCK_NONVOID0 `
   -DFACTS `
   -DMETALMOCK_VOID4_CONST `
   -DMETALMOCK_NONVOID1_CONST `
   -DMETALMOCK_NONVOID2_CONST `
   -DMETALMOCK_VOID2 `
   -DMETALMOCK_NONVOID2_FREE `
   -DMETALMOCK_VOID5_CONST `
   -I C:\include\ZenUnitAndMetalMock `
   -I libFileRevisor `
   -I . `
   -j $numberOfLogicalProcessors `
   --output-file=cppcheck_results.txt `
   --error-exitcode=1 `
   .
if ($LastExitCode -eq 1)
{
   Write-Host "Cppcheck failed with exit code 1"
   exit 1
}
