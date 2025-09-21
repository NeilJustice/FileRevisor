#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelTwoArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/TwoArgMemberFunctionForEacher.h"

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
   const bool targetDirectoryExists = _fileSystem->FileOrDirectoryExists(args.targetFolderPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage = String::ConcatStrings("Directory does not exist: ", args.targetFolderPath.string());
      _console->ProgramNameThreadIdWriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelFolderPathsInTargetDirectory = _fileSystem->GetStringFolderPathsInDirectory(args.targetFolderPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage = String::ConcatStrings("Deleting in parallel all files in directory: ", args.targetFolderPath.string());
      _console->ProgramNameThreadIdWriteLine(deletingInParallelMessage);
      _parallelTwoArgMemberFunctionForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory, this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   _fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet);
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
   const fs::path currentFolderPath = _fileSystem->CurrentFolderPath();
   if (currentFolderPath != args.targetFolderPath)
   {
      _fileSystem->DeleteFileOrDirectory(args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet);
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
   _console->ProgramNameThreadIdWriteLineColor(errorMessage, Color::Red);
}
