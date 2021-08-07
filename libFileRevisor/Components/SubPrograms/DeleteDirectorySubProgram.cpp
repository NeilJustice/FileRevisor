#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/TryCatchCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/OneExtraArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"

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
      const string directoryDoesNotExistMessage = String::ConcatStrings("[FileRevisor] Directory does not exist: ", args.targetDirectoryPath.string());
      _protected_console->WriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelDirectoryPathsInTargetDirectory =
      _protected_fileSystem->GetStringDirectoryPathsInDirectory(args.targetDirectoryPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage = String::ConcatStrings(
         "[FileRevisor] Deleting in parallel all files in directory: ", args.targetDirectoryPath.string());
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

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _protected_fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
}
