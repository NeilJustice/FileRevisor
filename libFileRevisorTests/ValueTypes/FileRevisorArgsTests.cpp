#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

TESTS(FileRevisorArgsTests)
AFACT(CommandLineUsage_IsExpectedString)
AFACT(DefaultConstructor_SetsFieldsTo0)
EVIDENCE

TEST(CommandLineUsage_IsExpectedString)
{
   ARE_EQUAL(R"(FileRevisor v1.1.0 - Quickly delete directories, rename files or directories, or replace text in files.

Usage:
   filerevisor delete-directory --target=<TargetFolderPath> [--parallel] [--skip-files-in-use] [--dryrun] [--quiet]
   filerevisor rename-files --target=<TargetFolderPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor rename-directories --target=<TargetFolderPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--dryrun] [--verbose]
   filerevisor replace-text --target=<TargetFolderPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--dryrun] [--verbose]
)", FileRevisorArgs::CommandLineUsage);
}

TEST(DefaultConstructor_SetsFieldsTo0)
{
   const FileRevisorArgs defaultFileRevisorArgs;
   FileRevisorArgs expectedDefaultFileRevisorArgs;
   expectedDefaultFileRevisorArgs.commandLine = "";
   expectedDefaultFileRevisorArgs.programMode = ProgramMode::Unset;
   expectedDefaultFileRevisorArgs.targetFolderPath = fs::path();
   expectedDefaultFileRevisorArgs.fromRegexPattern = "";
   expectedDefaultFileRevisorArgs.toRegexPattern = "";
   expectedDefaultFileRevisorArgs.recurse = false;
   expectedDefaultFileRevisorArgs.parallel = false;
   expectedDefaultFileRevisorArgs.skipFilesInUse = false;
   expectedDefaultFileRevisorArgs.dryrun = false;
   expectedDefaultFileRevisorArgs.quiet = false;
   expectedDefaultFileRevisorArgs.verbose = false;
   ARE_EQUAL(expectedDefaultFileRevisorArgs, defaultFileRevisorArgs);
}

RUN_TESTS(FileRevisorArgsTests)
