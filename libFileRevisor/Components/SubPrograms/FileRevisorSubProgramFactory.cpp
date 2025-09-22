#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

shared_ptr<FileRevisorSubProgram> FileRevisorSubProgramFactory::NewSubProgram(ProgramMode programMode) const
{
   switch (programMode)
   {
   case ProgramMode::RenameFiles: return make_shared<RenameFilesSubProgram>();
   case ProgramMode::RenameDirectories: return make_shared<RenameDirectoriesSubProgram>();
   case ProgramMode::ReplaceTextInTextFiles: return make_shared<ReplaceTextInTextFilesSubProgram>();
   case ProgramMode::DeleteDirectory: return make_shared<DeleteDirectorySubProgram>();
   default:
   {
      const string exceptionmessage = String::ConcatValues(
         "FileRevisorSubProgramFactory::NewSubProgram called with invalid ProgramMode: ", static_cast<int>(programMode));
      throw invalid_argument(exceptionmessage);
   }
   }
}
