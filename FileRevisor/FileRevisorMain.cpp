#include "libFileRevisor/Compiler/IfWindowsIgnoreTheseWarningsGlobally.h" // IWYU pragma: keep
#include <functional>
#include <memory>
#include <string>
using namespace std;
#include "libFileRevisor/Components/FileRevisor/FileRevisorProgram.h"

// Testing Args
// ============
// delete-directory --target=C:\FileRevisorTesting\directory1 --contents-only
// delete-directory --target=C:\FileRevisorTesting\directory1 --contents-only --parallel

// rename-files --target=C:\FileRevisorTesting --from=123 --to=abc --recurse --verbose

// rename-directories --target=C:\FileRevisorTesting --from=\w\w\w --to=new_dir_name --verbose

// replace-text --target=C:\FileRevisorTesting --from=abc --to=123
// replace-text --target=C:\FileRevisorTesting --from=abc --to=123 --verbose
// replace-text --target=C:\FileRevisorTesting --from=abc --to=123 --recurse --verbose

// Performance Test Args
// =====================
// delete-directory --target=X:\PerformanceTestingInputFiles\FileRevisor\PerformanceTest --parallel --quiet

// VTune Hotspots Args
// ===================
// delete-directory --target=X:\PerformanceTestingInputFiles\FileRevisor\VTuneHotspots --quiet

// VSDiagnostics Args
// ==================
// delete-directory --target=X:\PerformanceTestingInputFiles\FileRevisor\VSDiagnostics --quiet

int main(int argc, char* argv[])
{
   FileRevisorProgram fileRevisorProgram;
   int exitCode = fileRevisorProgram.Main(argc, argv);
   return exitCode;
}
