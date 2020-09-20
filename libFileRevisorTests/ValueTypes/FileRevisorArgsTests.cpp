#include "pch.h"
#include "libFileRevisor/ValueTypes/FileRevisorArgs.h"

TESTS(FileRevisorArgsTests)
AFACT(CommandLineUsage_IsExpectedString)
AFACT(DefaultConstructor_SetsFieldsTo0)
EVIDENCE

TEST(CommandLineUsage_IsExpectedString)
{
   ARE_EQUAL(R"(FileRevisor v0.9.6
https://github.com/NeilJustice/FileRevisor

Usage:
   filerevisor delete-directory --target=<TargetDirectoryPath> [--parallel] [--skip-files-in-use] [--minimal]
   filerevisor rename-files --target=<TargetDirectoryPath> --from=<FileNameRegex> --to=<FileNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor rename-directories --target=<TargetDirectoryPath> --from=<DirectoryNameRegex> --to=<DirectoryNameRegex> [--recurse] [--preview] [--verbose]
   filerevisor replace-text --target=<TargetDirectoryPath> --from=<FileTextRegexPattern> --to=<ReplacementFileTextRegexPattern> [--recurse] [--preview] [--verbose]
)", FileRevisorArgs::CommandLineUsage);
}

TEST(DefaultConstructor_SetsFieldsTo0)
{
   const FileRevisorArgs args;
   ARE_EQUAL("", args.commandLine);
   ARE_EQUAL(ProgramMode::Unset, args.programMode);
   ARE_EQUAL(fs::path(), args.targetDirectoryPath);
   ARE_EQUAL("", args.fromRegexPattern);
   ARE_EQUAL("", args.toRegexPattern);
   IS_FALSE(args.recurse);
   IS_FALSE(args.parallel);
   IS_FALSE(args.skipFilesInUse);
   IS_FALSE(args.preview);
   IS_FALSE(args.minimal);
   IS_FALSE(args.verbose);
}

RUN_TESTS(FileRevisorArgsTests)
