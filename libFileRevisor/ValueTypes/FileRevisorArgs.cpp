#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

const string FileRevisorArgs::CommandLineUsage =
R"(FileRevisor v0.10.0
Quickly deletes directories in parallel,
bulk renames files or directories,
or replaces text in files that matches a regular expression.
https://github.com/NeilJustice/FileRevisor

Usage:
   filerevisor delete-directory --target=<TargetDirectoryPath> [--parallel] [--skip-files-in-use] [--minimal]
   filerevisor rename-files --target=<TargetDirectoryPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor rename-directories --target=<TargetDirectoryPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor replace-text --target=<TargetDirectoryPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--preview] [--verbose]
)";
