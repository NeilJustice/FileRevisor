#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/DeleteDirectorySubProgram.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgramFactory.h"
#include "libFileRevisor/Components/SubPrograms/RenameDirectoriesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/RenameFilesSubProgram.h"
#include "libFileRevisor/Components/SubPrograms/ReplaceTextInTextFilesSubProgram.h"

TESTS(FileRevisorSubProgramFactoryTests)
AFACT(NewFileRevisorSubProgram_ProgramModeIsRenameFiles_ReturnsRenameFilesSubProgram)
AFACT(NewFileRevisorSubProgram_ProgramModeIsRenameDirectories_ReturnsRenameDirectoriesSubProgram)
AFACT(NewFileRevisorSubProgram_ProgramModeIsReplaceFileText_ReturnsReplaceFileTextSubProgram)
AFACT(NewFileRevisorSubProgram_ProgramModeIsDeleteDirectory_ReturnsDeleteDirectorySubProgram)
FACTS(NewFileRevisorSubProgram_ProgramModeIsInvalid_ThrowsInvalidArgument)
EVIDENCE

FileRevisorSubProgramFactory _fileRevisorSubProgramFactory;

TEST(NewFileRevisorSubProgram_ProgramModeIsRenameFiles_ReturnsRenameFilesSubProgram)
{
   const shared_ptr<FileRevisorSubProgram> renameFilesSubProgram = _fileRevisorSubProgramFactory.NewFileRevisorSubProgram(ProgramMode::RenameFiles);
   POINTEE_IS_EXACT_TYPE(RenameFilesSubProgram, renameFilesSubProgram);
}

TEST(NewFileRevisorSubProgram_ProgramModeIsRenameDirectories_ReturnsRenameDirectoriesSubProgram)
{
   const shared_ptr<FileRevisorSubProgram> renameDirectoriesSubProgram = _fileRevisorSubProgramFactory.NewFileRevisorSubProgram(ProgramMode::RenameDirectories);
   POINTEE_IS_EXACT_TYPE(RenameDirectoriesSubProgram, renameDirectoriesSubProgram);
}

TEST(NewFileRevisorSubProgram_ProgramModeIsReplaceFileText_ReturnsReplaceFileTextSubProgram)
{
   const shared_ptr<FileRevisorSubProgram> replaceTextInFilesSubProgram = _fileRevisorSubProgramFactory.NewFileRevisorSubProgram(ProgramMode::ReplaceTextInTextFiles);
   POINTEE_IS_EXACT_TYPE(ReplaceTextInTextFilesSubProgram, replaceTextInFilesSubProgram);
}

TEST(NewFileRevisorSubProgram_ProgramModeIsDeleteDirectory_ReturnsDeleteDirectorySubProgram)
{
   const shared_ptr<FileRevisorSubProgram> deleteDirectorySubProgram = _fileRevisorSubProgramFactory.NewFileRevisorSubProgram(ProgramMode::DeleteDirectory);
   POINTEE_IS_EXACT_TYPE(DeleteDirectorySubProgram, deleteDirectorySubProgram);
}

TEST1X1(NewFileRevisorSubProgram_ProgramModeIsInvalid_ThrowsInvalidArgument,
   ProgramMode invalidProgramMode,
   ProgramMode::Unset,
   ProgramMode::MaxValue)
{
   THROWS_EXCEPTION(shared_ptr<FileRevisorSubProgram> fileRevisorSubProgram = _fileRevisorSubProgramFactory.NewFileRevisorSubProgram(invalidProgramMode),
      invalid_argument, "[FileRevisor] Invalid ProgramMode: " + to_string(static_cast<int>(invalidProgramMode)));
}

RUN_TESTS(FileRevisorSubProgramFactoryTests)
