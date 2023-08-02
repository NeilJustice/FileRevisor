#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/Components/Utility/Strings/Pluralizer.h"

FileRevisorSubProgram::~FileRevisorSubProgram()
{
}

FileRevisorSubProgram::FileRevisorSubProgram()
   // Base Class Constant Components
   : _console(make_unique<Console>())
   , _fileSystem(make_unique<FileSystem>())
   , _pluralizer(make_unique<Pluralizer>())
{
}

int FileRevisorSubProgram::Run(const FileRevisorArgs& /*args*/) const
{
   return 0;
}
