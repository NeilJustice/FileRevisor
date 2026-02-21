#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidOneArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/Member/VoidZeroArgMemberFunctionCaller.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidOneArgTryCatchCaller.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidTwoArgTryCatchCaller.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/TwoArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _caller_DeleteTargetDirectoryIfNotCurrentDirectory(make_unique<_caller_DeleteTargetDirectoryIfNotCurrentDirectoryType>())
   , _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<_oneExtraArgMemberForEacher_DeleteDirectoryType>())
   , _parallelTwoArgMemberFunctionForEacher_DeleteDirectory(make_unique<_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType>())
   , _tryCatchCaller_DeleteDirectory(make_unique<_tryCatchCaller_DeleteDirectoryType>())
{
}

DeleteDirectorySubProgram::~DeleteDirectorySubProgram()
{
}

// Actions

int DeleteDirectorySubProgram::Run() const
{
   const bool targetDirectoryExists = p_fileSystem->FileOrDirectoryExists(p_args.targetFolderPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage = Utils::String::ConcatStrings(
         "Directory does not exist: ", p_args.targetFolderPath.string());
      p_console->ProgramNameThreadIdWriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelFolderPathsInTargetDirectory = p_fileSystem->GetStringFolderPathsInDirectory(p_args.targetFolderPath, false);
   if (p_args.parallel)
   {
      const string deletingInParallelMessage = Utils::String::ConcatStrings(
         "Deleting in parallel all files in directory: ", p_args.targetFolderPath.string());
      p_console->ProgramNameThreadIdWriteLine(deletingInParallelMessage);
      _parallelTwoArgMemberFunctionForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory,
         this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory,
         this, &DeleteDirectorySubProgram::DeleteDirectory);
   }
   p_fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(
      p_args.targetFolderPath,
      p_args.skipFilesInUse,
      p_args.dryrun,
      p_args.quiet);
   _caller_DeleteTargetDirectoryIfNotCurrentDirectory->CallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath) const
{
   p_fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, p_args);
}

void DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory() const
{
   const fs::path currentFolderPath = p_fileSystem->CurrentFolderPath();
   if (currentFolderPath != p_args.targetFolderPath)
   {
      p_fileSystem->DeleteFileOrDirectory(p_args.targetFolderPath, p_args.skipFilesInUse, p_args.dryrun, p_args.quiet);
   }
}

void DeleteDirectorySubProgram::TryCatchCallDeleteDirectory(const string& directoryPath) const
{
   _tryCatchCaller_DeleteDirectory->TryCatchCallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath,
      &DeleteDirectorySubProgram::ParallelExceptionHandler);
}

void DeleteDirectorySubProgram::ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const
{
   const string errorMessage = "Error: " + string(exceptionClassNameAndMessage);
   p_console->ProgramNameThreadIdWriteLineColor(errorMessage, Color::Red);
}
