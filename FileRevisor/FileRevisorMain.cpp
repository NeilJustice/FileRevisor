#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example FileRevisor command line arguments:
// delete-directory --target=C:\FileRevisorTesting\directory1 --parallel
// rename-files --target=C:\FileRevisorTesting --from=file --to=abc --recurse --dryrun --verbose
// rename-directories --target=C:\FileRevisorTesting --from=dir1 --to=new_dir_name --dryrun --verbose
// replace-text --target=D:\FileRevisorTesting --from=100 --to=200 --recurse --verbose

#include <fstream>

int main(int argc, char* argv[])
{
   std::ofstream f(R"(D:\FileRevisorTesting\a.txt)");

   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
