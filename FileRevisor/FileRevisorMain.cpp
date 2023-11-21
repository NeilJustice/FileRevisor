#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>
using namespace std;
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Example FileRevisor command line arguments:
// delete-directory --target=C:\FileRevisorTesting\directory1 --parallel
// rename-files --target=C:\FileRevisorTesting --from=file --to=abc --recurse --dryrun --verbose
// rename-directories --target=C:\FileRevisorTesting --from=dir1 --to=new_dir_name --dryrun --verbose
// replace-text --target=D:\FileRevisorTesting --from=\d --to=2 --recurse --verbose

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   const int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
