#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

TESTS(FileRevisorArgsTests)
AFACT(CommandLineUsage_IsExpectedString)
AFACT(DefaultConstructor_SetsFieldsTo0)
EVIDENCE

TEST(CommandLineUsage_IsExpectedString)
{
   ARE_EQUAL(R"(FileRevisor v0.11.0
Quickly deletes directories, renames files, renames directories, or replaces text in files.

Usage:
   filerevisor delete-directory --target=<TargetDirectoryPath> [--parallel] [--skip-files-in-use] [--minimal]
   filerevisor rename-files --target=<TargetDirectoryPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor rename-directories --target=<TargetDirectoryPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor replace-text --target=<TargetDirectoryPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--preview] [--verbose]
)", FileRevisorArgs::CommandLineUsage);
}

TEST(DefaultConstructor_SetsFieldsTo0)
{
   const FileRevisorArgs defaultFileRevisorArgs;
   FileRevisorArgs expectedDefaultFileRevisorArgs;
   expectedDefaultFileRevisorArgs.commandLine = "";
   expectedDefaultFileRevisorArgs.programMode = ProgramMode::Unset;
   expectedDefaultFileRevisorArgs.targetDirectoryPath = fs::path();
   expectedDefaultFileRevisorArgs.fromRegexPattern = "";
   expectedDefaultFileRevisorArgs.toRegexPattern = "";
   expectedDefaultFileRevisorArgs.recurse = false;
   expectedDefaultFileRevisorArgs.parallel = false;
   expectedDefaultFileRevisorArgs.skipFilesInUse = false;
   expectedDefaultFileRevisorArgs.preview = false;
   expectedDefaultFileRevisorArgs.minimal = false;
   expectedDefaultFileRevisorArgs.verbose = false;
   ARE_EQUAL(expectedDefaultFileRevisorArgs, defaultFileRevisorArgs);
}

RUN_TESTS(FileRevisorArgsTests)
