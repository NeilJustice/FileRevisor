#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

const string FileRevisorArgs::CommandLineUsage =
R"(FileRevisor v1.1.0 - Quickly delete directories, rename files or directories, or replace text in files.

Usage:
   filerevisor delete-directory --target=<TargetDirectoryPath> [--parallel] [--skip-files-in-use] [--dryrun] [--quiet]
   filerevisor rename-files --target=<TargetDirectoryPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor rename-directories --target=<TargetDirectoryPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor replace-text --target=<TargetDirectoryPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--dryrun] [--verbose]
)";
