#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

const string FileRevisorArgs::CommandLineUsage =
R"(FileRevisor v0.9.6
Quickly deletes directories in parallel, renames files and directories, and replaces text in files.
https://github.com/NeilJustice/FileRevisor

Usage:
   filerevisor delete-directory --target=<TargetDirectoryPath> [--parallel] [--skip-files-in-use] [--minimal]
   filerevisor rename-files --target=<TargetDirectoryPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor rename-directories --target=<TargetDirectoryPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor replace-text --target=<TargetDirectoryPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--preview] [--verbose]
)";
