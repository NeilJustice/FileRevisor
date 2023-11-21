#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

const string FileRevisorArgs::CommandLineUsage =
R"(FileRevisor v1.1.0 - Quickly delete directories, rename files or directories, or replace text in files.

Usage:
   filerevisor delete-directory --target=<TargetFolderPath> [--parallel] [--skip-files-in-use] [--dryrun] [--quiet]
   filerevisor rename-files --target=<TargetFolderPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor rename-directories --target=<TargetFolderPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor replace-text --target=<TargetFolderPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--dryrun] [--verbose]
)";
