#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example FileRevisor command line arguments:
// delete-directory --target=C:\FileRevisorTesting --skip-files-in-use
// rename-files --target=C:\FileRevisorTesting\Folder --from=from --to=to --recurse --dryrun --verbose
// rename-directories --target=C:\FileRevisorTesting --from=Folder1 --to=Folder2 --dryrun --verbose
// replace-text --target=C:\FileRevisorTesting --from=from --to=to --recurse --dryrun --verbose

#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

int main(int argc, char* argv[])
{
   ofstream f(R"(C:\FileRevisorTesting\file_in_use)");

   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
