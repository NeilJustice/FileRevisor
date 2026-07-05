#include "pch.h"
#include "libFileRevisor/Components/FileSystem/DirectoryDeleter.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/FunctionCallers/TryCatchCallers/VoidOneArgTryCatchCaller.h"
#include "libFileRevisor/Components/Iteration/ForEach/OneArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/Iteration/ForEach/ParallelOneArgMemberFunctionForEacher.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"

DeleteDirectorySubProgram::DeleteDirectorySubProgram()
   // Function Callers
   : _oneExtraArgMemberForEacher_DeleteDirectory(make_unique<_oneExtraArgMemberForEacher_DeleteDirectoryType>())
   , _parallelTwoArgMemberFunctionForEacher_DeleteDirectory(make_unique<_parallelTwoArgMemberFunctionForEacher_DeleteDirectoryType>())
   , _tryCatchCaller_DeleteDirectory(make_unique<_tryCatchCaller_DeleteDirectoryType>())
   // Constant Components
   , _directoryDeleter(make_unique<DirectoryDeleter>())
{
}

DeleteDirectorySubProgram::~DeleteDirectorySubProgram()
{
}

// Actions

int DeleteDirectorySubProgram::Run() const
{
   const bool targetDirectoryExists = p_fileSystem->FileOrDirectoryExists(p_args.targetDirectoryPath);
   if (!targetDirectoryExists)
   {
      const string directoryDoesNotExistMessage = Utils::String::ConcatStrings(
         "Directory does not exist: ", p_args.targetDirectoryPath.string());
      p_console->WriteProgramNameThreadIdLine(directoryDoesNotExistMessage);
      return 0;
   }
   const vector<string> topLevelDirectoryPathsInTargetDirectory =
      p_fileSystem->GetStringFolderPathsInDirectory(p_args.targetDirectoryPath, false);
   if (p_args.parallel)
   {
      const string deletingInParallelMessage = Utils::String::ConcatStrings(
         "Deleting in parallel all files in directory: ", p_args.targetDirectoryPath.string());
      p_console->WriteProgramNameThreadIdLine(deletingInParallelMessage);
      _parallelTwoArgMemberFunctionForEacher_DeleteDirectory->ParallelCallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory,
         this, &DeleteDirectorySubProgram::TryCatchCallDeleteDirectory);
   }
   else
   {
      _oneExtraArgMemberForEacher_DeleteDirectory->CallConstMemberFunctionWithEachElement(
         topLevelDirectoryPathsInTargetDirectory,
         this, &DeleteDirectorySubProgram::DeleteDirectory);
   }
   p_fileSystem->DeleteTopLevelFilesAndEmptyDirectoriesInDirectory(
      p_args.targetDirectoryPath,
      p_args.skipFilesInUse,
      p_args.dryrun,
      p_args.quiet);
   _directoryDeleter->DeleteTargetDirectoryIfNotContentsOnlyAndNotCurrentDirectory(p_args);
   return 0;
}

// Private Functions

void DeleteDirectorySubProgram::DeleteDirectory(const string& directoryPath) const
{
   p_fileSystem->RecursivelyDeleteAllFilesInDirectory(directoryPath, p_args);
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
   p_console->WriteProgramNameThreadIdLineColor(errorMessage, Color::Red);
}
