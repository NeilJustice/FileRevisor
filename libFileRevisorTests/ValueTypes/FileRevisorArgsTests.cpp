#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

TESTS(FileRevisorArgsTests)
AFACT(CommandLineUsage_IsExpectedString)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
EVIDENCE

TEST(CommandLineUsage_IsExpectedString)
{
   ARE_EQUAL(R"(FileRevisor v1.1.0
Deletes directories, renames files or directories, or replaces text in files.

Usage:
   filerevisor delete-directory
      --target=<TargetDirectory>
      [--contents-only]
      [--parallel]
      [--skip-files-in-use]
      [--dryrun]
      [--quiet]
   filerevisor rename-files
      --target=<TargetDirectory>
      --from=<FileNameSubstring>
      --to=<FileNameSubstring>
      [--recurse]
      [--dryrun]
      [--verbose]
   filerevisor rename-directories
      --target=<TargetDirectory>
      --from=<DirectoryNameSubstring>
      --to=<DirectoryNameSubstring>
      [--recurse]
      [--dryrun]
      [--verbose]
   filerevisor replace-text
      --target=<TargetDirectory>
      --from=<TextSubstring>
      --to=<ReplacementTextSubstring>
      [--recurse]
      [--dryrun]
      [--verbose]
)", FileRevisorArgs::CommandLineUsage);
}

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const FileRevisorArgs defaultFileRevisorArgs;
   FileRevisorArgs expectedDefaultFileRevisorArgs;
   expectedDefaultFileRevisorArgs.commandLine = "";
   expectedDefaultFileRevisorArgs.programMode = ProgramMode::Unset;
   expectedDefaultFileRevisorArgs.targetDirectoryPath = fs::path();
   expectedDefaultFileRevisorArgs.fromFileOrDirectoryName = "";
   expectedDefaultFileRevisorArgs.toFileOrDirectoryName = "";
   expectedDefaultFileRevisorArgs.contentsOnly = false;
   expectedDefaultFileRevisorArgs.recurse = false;
   expectedDefaultFileRevisorArgs.parallel = false;
   expectedDefaultFileRevisorArgs.skipFilesInUse = false;
   expectedDefaultFileRevisorArgs.dryrun = false;
   expectedDefaultFileRevisorArgs.quiet = false;
   expectedDefaultFileRevisorArgs.verbose = false;
   ARE_EQUAL(expectedDefaultFileRevisorArgs, defaultFileRevisorArgs);
}

RUN_TESTS(FileRevisorArgsTests)
