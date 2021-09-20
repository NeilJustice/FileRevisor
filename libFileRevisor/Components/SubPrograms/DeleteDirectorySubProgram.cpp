#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/NonVoidOneArgTryCatchCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/TwoArgMemberFunctionForEacher.h"
#include "libFileRevisor/UtilityComponents/Iteration/ForEach/ParallelTwoArgMemberFunctionForEacher.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _caller_DeleteTargetDirectoryIfNotCurrentDirectory(make_unique<_caller_DeleteTargetDirectoryIfNotCurrentDirectoryType>())
   , _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<TwoArgMemberFunctionForEacherType>())
   , _parallelTwoArgMemberFunctionForEacher_DeleteDirectory(make_unique<ParallelTwoArgMemberFunctionForEacherType>())
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
      _parallelTwoArgMemberFunctionForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   _fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(args.targetDirectoryPath, args.skipFilesInUse, args.dryrun, args.quiet);
   _caller_DeleteTargetDirectoryIfNotCurrentDirectory->CallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
}

void DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory(const FileRevisorArgs& args) const
{
   const fs::path currentDirectoryPath = _fileSystem->CurrentDirectoryPath();
   if (currentDirectoryPath != args.targetDirectoryPath)
   {
      _fileSystem->DeleteFileOrDirectory(args.targetDirectoryPath, args.skipFilesInUse, args.dryrun, args.quiet);
   }
}

void DeleteDirectorySubProgram::TryCatchCallDeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _voidTwoArgTryCatchCaller->TryCatchCallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath, args, &DeleteDirectorySubProgram::ParallelExceptionHandler);
}

void DeleteDirectorySubProgram::ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const
{
   const string errorMessage = "Error: " + string(exceptionClassNameAndMessage);
   _console->ThreadIdWriteLineColor(errorMessage, Color::Red);
}
