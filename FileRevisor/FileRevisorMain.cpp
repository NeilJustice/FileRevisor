#include "libFileRevisor/Compiler/IfMSVCIgnoreTheseWarnings.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example FileRevisor command line arguments:
// delete-directory --target=C:\FileRevisorTesting\Folder --parallel
// rename-files --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --preview --verbose
// rename-directories --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --preview --verbose
// replace-text --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --preview --verbose

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
