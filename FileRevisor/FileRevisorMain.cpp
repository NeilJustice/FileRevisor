#include "libFileRevisor/Utilities/MSVCIgnoredWallWarnings.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example command lines:
// delete-directory --target=C:\FileRevisorTesting\dir1 --parallel
// rename-files --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// rename-directories --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// replace-text --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// replace-text --target=C:\Code\CppUtils --from="Zen::Mock" --to="Metal::Mock" --recurse --preview --verbose

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
