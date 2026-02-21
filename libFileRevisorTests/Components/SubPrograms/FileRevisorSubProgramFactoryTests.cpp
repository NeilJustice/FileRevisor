#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

TESTS(FileRevisorSubProgramFactoryTests)
AFACT(NewSubProgram_ValidProgramMode_ReturnsSubProgram)
FACTS(NewSubProgram_ProgramModeIsInvalid_ThrowsInvalidArgument)
EVIDENCE

FileRevisorSubProgramFactory _fileRevisorSubProgramFactory;

TEST(NewSubProgram_ValidProgramMode_ReturnsSubProgram)
{
   POINTEE_IS_EXACT_TYPE(RenameFilesSubProgram, _fileRevisorSubProgramFactory.NewSubProgram(ProgramMode::RenameFiles));
   POINTEE_IS_EXACT_TYPE(RenameDirectoriesSubProgram, _fileRevisorSubProgramFactory.NewSubProgram(ProgramMode::RenameDirectories));
   POINTEE_IS_EXACT_TYPE(ReplaceTextInTextFilesSubProgram, _fileRevisorSubProgramFactory.NewSubProgram(ProgramMode::ReplaceTextInTextFiles));
   POINTEE_IS_EXACT_TYPE(DeleteDirectorySubProgram, _fileRevisorSubProgramFactory.NewSubProgram(ProgramMode::DeleteDirectory));
}

TEST1X1(NewSubProgram_ProgramModeIsInvalid_ThrowsInvalidArgument,
   ProgramMode invalidProgramMode,
   ProgramMode::Unset,
   ProgramMode::MaxValue)
{
   const string expectedExceptionmessage = Utils::String::ConcatValues(
      "FileRevisorSubProgramFactory::NewSubProgram called with invalid ProgramMode: ", static_cast<int>(invalidProgramMode));
   THROWS_EXCEPTION(shared_ptr<FileRevisorSubProgram> fileRevisorSubProgram = _fileRevisorSubProgramFactory.NewSubProgram(invalidProgramMode),
      invalid_argument, expectedExceptionmessage);
}

RUN_TESTS(FileRevisorSubProgramFactoryTests)
