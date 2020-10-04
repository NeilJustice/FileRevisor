#include "libFileRevisor/Utilities/MSVCIgnoredWallWarnings.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example command lines:
// delete-directory --target=C:\FileRevisorTesting\dir1 --parallel
// rename-files --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// rename-directories --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// replace-text --target=C:\FileRevisorTesting\dir1 --from=from --to=to --recurse --preview --verbose
// replace-text --target=C:\Code\CppUtils --from="Zen::Mock" --to="Metal::Mock" --recurse --preview --verbose

//#include <fstream>

int main(int argc, char* argv[])
{
   //std::ofstream f1(R"(C:\FileRevisorTesting\Folder\Folder1\FileInUseInSubFolder.txt)");
   //std::ofstream f2(R"(C:\FileRevisorTesting\Folder\FileInUseInTopFolder.txt)");

   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
