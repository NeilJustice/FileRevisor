#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/TwoArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelTwoArgMemberForEacher.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<TwoArgMemberForEacherType>())
   , _parallelTwoArgMemberForEacher_DeleteDirectory(make_unique<ParallelTwoArgMemberForEacherType>())
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
      const string directoryDoesNotExistMessage = String::ConcatStrings("Directory does not exist: ", args.targetDirectoryPath.string());
      _console->ThreadIdWriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelDirectoryPathsInTargetDirectory = _fileSystem->GetStringDirectoryPathsInDirectory(args.targetDirectoryPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage = String::ConcatStrings("Deleting in parallel all files in directory: ", args.targetDirectoryPath.string());
      _console->ThreadIdWriteLine(deletingInParallelMessage);
      _parallelTwoArgMemberForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   _fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(args.targetDirectoryPath, args.skipFilesInUse, args.dryrun);
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
