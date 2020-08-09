#include "libFileRevisor/Utilities/MSVCIgnoredWallWarnings.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// delete-directory --target=C:\FileRevisorTesting\dir1 --parallel

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
