#include "pch.h"
#if defined __linux__
#include "libFileRevisor/Components/FileSystem/DirectoryIterator.h"
#include "libFileRevisor/Components/FileSystem/FileOpenerCloser.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/UtilityComponents/FunctionCallers/Member/VoidTwoArgMemberFunctionCaller.h"
#include "libFileRevisor/UtilityComponents/Iteration/Counter/PredicateCounter.h"
#include "libFileRevisor/UtilityComponents/Iteration/Math/OneExtraArgMemberFunctionAccumulator.h"
#include "libFileRevisor/UtilityComponents/Iteration/Transform/OneExtraArgMemberFunctionTransformer.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"
#include "libFileRevisor/UtilityComponents/Strings/Regexer.h"
#endif
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

shared_ptr<FileRevisorSubProgram> FileRevisorSubProgramFactory::NewFileRevisorSubProgram(ProgramMode programMode) const
{
   switch (programMode)
   {
   case ProgramMode::RenameFiles:
   {
      return make_shared<RenameFilesSubProgram>();
   }
   case ProgramMode::RenameDirectories:
   {
      return make_shared<RenameDirectoriesSubProgram>();
   }
   case ProgramMode::ReplaceTextInTextFiles:
   {
      return make_shared<ReplaceTextInTextFilesSubProgram>();
   }
   case ProgramMode::DeleteDirectory:
   {
      return make_shared<DeleteDirectorySubProgram>();
   }
   default: throw invalid_argument("[FileRevisor] Invalid ProgramMode: " + to_string(static_cast<int>(programMode)));
   }
}
