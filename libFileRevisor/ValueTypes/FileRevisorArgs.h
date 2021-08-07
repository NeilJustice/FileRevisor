#pragma once
#include "libFileRevisor/Enums/ProgramMode.h"

struct FileRevisorArgs
{
   static const string CommandLineUsage;
   string commandLine;
   ProgramMode programMode = ProgramMode::Unset;
   fs::path targetDirectoryPath;
   string fromRegexPattern;
   string toRegexPattern;
   bool recurse = false;
   bool parallel = false;
   bool skipFilesInUse = false;
   bool preview = false;
   bool minimal = false;
   bool verbose = false;
};

#ifdef _WIN32
#ifdef _DEBUG
static_assert(sizeof(FileRevisorArgs) == 176);
#else
static_assert(sizeof(FileRevisorArgs) == 144);
#endif
#else
static_assert(sizeof(FileRevisorArgs) == 152);
#endif