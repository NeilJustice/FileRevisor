#pragma once
#include "libFileRevisor/Enums/ProgramMode.h"

struct FileRevisorArgs
{
   static const string CommandLineUsage;
   string commandLine;
   ProgramMode programMode = ProgramMode::Unset;
   fs::path targetFolderPath;
   string fromRegexPattern;
   string toRegexPattern;
   bool recurse = false;
   bool parallel = false;
   bool skipFilesInUse = false;
   bool dryrun = false;
   bool quiet = false;
   bool verbose = false;
};

#ifdef _WIN32
#ifdef _DEBUG
static_assert(sizeof(FileRevisorArgs) == 176);
#else
static_assert(sizeof(FileRevisorArgs) == 144);
#endif
#elifdef __linux__
static_assert(sizeof(FileRevisorArgs) == 152);
#endif