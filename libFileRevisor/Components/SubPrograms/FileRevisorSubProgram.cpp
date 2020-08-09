#include "pch.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"

FileRevisorSubProgram::FileRevisorSubProgram()
   : _protected_console(make_unique<Console>())
   , _protected_fileSystem(make_unique<FileSystem>())
   , _protected_pluralizer(make_unique<Pluralizer>())
{
}

FileRevisorSubProgram::~FileRevisorSubProgram()
{
}

int FileRevisorSubProgram::Run(const FileRevisorArgs& /*args*/) const
{
   return 0;
}
