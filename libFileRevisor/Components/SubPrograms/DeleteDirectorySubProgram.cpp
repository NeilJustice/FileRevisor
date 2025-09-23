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
   , _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<_oneExtraArgMemberForEacher_DeleteDirectoryType>())
   , _parallelTwoArgMemberFunctionForEacher_DeleteDirectory(make_unique<_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType>())
   , _tryCatchCaller_DeleteDirectory(make_unique<_tryCatchCaller_DeleteDirectoryType>())
{
}

DeleteDirectorySubProgram::~DeleteDirectorySubProgram()
{
}

int DeleteDirectorySubProgram::Run(const FileRevisorArgs& args) const
{
   const bool targetDirectoryExists = p_fileSystem->FileOrDirectoryExists(args.targetFolderPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage = String::ConcatStrings("Directory does not exist: ", args.targetFolderPath.string());
      p_console->ProgramNameThreadIdWriteLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelFolderPathsInTargetDirectory = p_fileSystem->GetStringFolderPathsInDirectory(args.targetFolderPath, false);
   if (args.parallel)
   {
      const string deletingInParallelMessage = String::ConcatStrings("Deleting in parallel all files in directory: ", args.targetFolderPath.string());
      p_console->ProgramNameThreadIdWriteLine(deletingInParallelMessage);
      _parallelTwoArgMemberFunctionForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory, this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory, args);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelFolderPathsInTargetDirectory, this, &DeleteDirectorySubProgram::DeleteDirectory, args);
   }
   p_fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet);
   _caller_DeleteTargetDirectoryIfNotCurrentDirectory->CallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory, args);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   p_fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, args);
}

void DeleteDirectorySubProgram::DeleteTargetDirectoryIfNotCurrentDirectory(const FileRevisorArgs& args) const
{
   const fs::path currentFolderPath = p_fileSystem->CurrentFolderPath();
   if (currentFolderPath != args.targetFolderPath)
   {
      p_fileSystem->DeleteFileOrDirectory(args.targetFolderPath, args.skipFilesInUse, args.dryrun, args.quiet);
   }
}

void DeleteDirectorySubProgram::TryCatchCallDeleteDirectory(const string& directoryPath, const FileRevisorArgs& args) const
{
   _tryCatchCaller_DeleteDirectory->TryCatchCallConstMemberFunction(
      this, &DeleteDirectorySubProgram::DeleteDirectory, directoryPath, args,
      &DeleteDirectorySubProgram::ParallelExceptionHandler);
}

void DeleteDirectorySubProgram::ParallelExceptionHandler(string_view exceptionClassNameAndMessage) const
{
   const string errorMessage = "Error: " + string(exceptionClassNameAndMessage);
   p_console->ProgramNameThreadIdWriteLineColor(errorMessage, Color::Red);
}
