#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/OneExtraArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelOneExtraArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<OneExtraArgMemberForEacherType>())
   , _parallelOneExtraArgMemberForEacher_DeleteDirectory(make_unique<ParallelOneExtraArgMemberForEacherType>())
   , _voidTwoArgTryCatchCaller(make_unique<_voidTwoArgTryCatchCallerType>())
{
}

DeleteDirectorySubProgram::~DeleteDirectorySubProgram()
{
}

int DeleteDirectorySubProgram::Run(const FileRevisorArgs& args) const
{
   const bool targetDirectoryExists = _fileSystem->FileOrDirectoryExists(args.targetDirectoryPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage = String::ConcatStrings("[FileRevisor] Directory does not exist: ", args.targetDirectoryPath.string());
      _console->WriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelDirectoryPathsInTargetDirectory = _fileSystem->GetStringDirectoryPathsInDirectory(args.targetDirectoryPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage = String::ConcatStrings(
         "[FileRevisor] Deleting in parallel all files in directory: ", args.targetDirectoryPath.string());
      _console->WriteLine(deletingInParallelMessage);
      _parallelOneExtraArgMemberForEacher_DeleteDirectory->ParallelOneExtraArgMemberForEach(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->OneExtraArgMemberForEach(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   _fileSystem->DeleteDirectoryIfNotDryRun(args.targetDirectoryPath, args.dryrun);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::TryCatchCallDeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _voidTwoArgTryCatchCaller->TryCatchCallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath, args, &DeleteDirectorySubProgram::ParallelExceptionHandler);
}

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
}

void DeleteDirectorySubProgram::ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const
{
   const string errorMessage = "Error: " + string(exceptionClassNameAndMessage);
   _console->ThreadIdWriteLineColor(errorMessage, Color::Red);
}
