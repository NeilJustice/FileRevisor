#include "pch.h"
#include "libFileRevisor/Components/Exceptions/TryCatchCaller.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneExtraArgMemberForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<OneExtraArgMemberForEacherType>())
   , _parallelOneExtraArgMemberForEacher_DeleteDirectory(make_unique<ParallelOneExtraArgMemberForEacherType>())
{
}

DeleteDirectorySubProgram::~DeleteDirectorySubProgram()
{
}

int DeleteDirectorySubProgram::Run(const FileRevisorArgs& args) const
{
   const bool targetDirectoryExists = _protected_fileSystem->FileOrDirectoryExists(args.targetDirectoryPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage =
         "[FileRevisor] Directory does not exist: " + args.targetDirectoryPath.string();
      _protected_console->WriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelDirectoryPathsInTargetDirectory =
      _protected_fileSystem->GetStringDirectoryPathsInDirectory(args.targetDirectoryPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage =
         "[FileRevisor] Deleting in parallel all files in directory: " + args.targetDirectoryPath.string();
      _protected_console->WriteLine(deletingInParallelMessage);
      _parallelOneExtraArgMemberForEacher_DeleteDirectory->ParallelOneExtraArgMemberForEach(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->OneExtraArgMemberForEach(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   _protected_fileSystem->RemoveReadonlyFlagsFromTopLevelFilesInDirectoryIfWindows(args.targetDirectoryPath);
   _protected_fileSystem->RemoveAll(args.targetDirectoryPath);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::DeleteDirectory(
   const string& directoryPath, const FileRevisorArgs& args) const
{
   _protected_fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
}
