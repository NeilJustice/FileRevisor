#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example FileRevisor command line arguments:
// delete-directory --target=C:\FileRevisorTesting --parallel
// rename-files --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --dryrun --verbose
// rename-directories --target=C:\FileRevisorTesting --from=Folder1 --to=Folder2 --dryrun --verbose
// replace-text --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --dryrun --verbose

//#include <fstream>
//using namespace std;

int main(int argc, char* argv[])
{
   //ifstream f(R"(C:\FileRevisorTesting\directory1\file1.bin)");

   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
