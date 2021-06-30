#include "pch.h"
#include "libFileRevisor/Components/FileSystem/FileSystem.h"
#include "libFileRevisor/Components/SubPrograms/FileRevisorSubProgram.h"
#include "libFileRevisor/UtilityComponents/Strings/Pluralizer.h"

FileRevisorSubProgram::~FileRevisorSubProgram()
{
}

FileRevisorSubProgram::FileRevisorSubProgram()
   // Base Class Constant Components
   : _protected_console(make_unique<Console>())
   , _protected_fileSystem(make_unique<FileSystem>())
   , _protected_pluralizer(make_unique<Pluralizer>())
{
}

int FileRevisorSubProgram::Run(const FileRevisorArgs& /*args*/) const
{
   return 0;
}
