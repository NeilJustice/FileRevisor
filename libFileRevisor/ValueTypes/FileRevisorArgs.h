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
