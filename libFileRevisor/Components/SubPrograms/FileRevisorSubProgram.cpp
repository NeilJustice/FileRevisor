#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/Components/Strings/Pluralizer.h"

FileRevisorSubProgram::~FileRevisorSubProgram()
{
}

FileRevisorSubProgram::FileRevisorSubProgram()
   // Base Class Constant Components
   : p_console(make_unique<Console>())
   , p_fileSystem(make_unique<FileSystem>())
   , p_pluralizer(make_unique<Pluralizer>())
{
}

void FileRevisorSubProgram::Initialize(const FileRevisorArgs& args)
{
   p_args = args;
}

// Actions

int FileRevisorSubProgram::Run() const
{
   return 0;
}
