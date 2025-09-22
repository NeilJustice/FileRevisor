#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h" // IWYU pragma: keep
#include <functional>
#include <memory>
#include <string>
using namespace std;
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Testing command line arguments
// ==============================
// delete-directory --target=C:\FileRevisorTesting\directory1 --parallel

// rename-files --target=C:\FileRevisorTesting --from=file --to=abc --recurse --dryrun --verbose

// rename-directories --target=C:\FileRevisorTesting --from=dir1 --to=new_dir_name --dryrun --verbose

// replace-text --target=D:\FileRevisorTesting --from=abc --to=123
// replace-text --target=D:\FileRevisorTesting --from=abc --to=123 --verbose
// replace-text --target=D:\FileRevisorTesting --from=abc --to=123 --recurse --verbose

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
