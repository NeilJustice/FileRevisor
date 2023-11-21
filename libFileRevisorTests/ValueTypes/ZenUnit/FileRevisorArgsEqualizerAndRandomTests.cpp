#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"

TESTS(FileRevisorArgsEqualizerAndRandomTests)
AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
AFACT(TestableRandomFileRevisorArgs_ReturnsFileRevisorArgsWithAllRandomFields)
AFACT(ZenUnitRandomFileRevisorArgs_CodeCoverage)
EVIDENCE

TEST(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
{
   ZENUNIT_EQUALIZER_TEST_SETUP(FileRevisorArgs);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, commandLine, ZenUnit::Random<string>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, programMode, ProgramMode::ReplaceTextInTextFiles);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, fromRegexPattern, ZenUnit::Random<string>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, toRegexPattern, ZenUnit::Random<string>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, targetFolderPath, ZenUnit::Random<fs::path>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, recurse, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, parallel, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, skipFilesInUse, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, dryrun, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, quiet, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, verbose, true);
}

TEST(TestableRandomFileRevisorArgs_ReturnsFileRevisorArgsWithAllRandomFields)
{
   ZenUnit::RandomGeneratorMock randomGeneratorMock;

   const string commandLine = ZenUnit::Random<string>();
   const string fromRegexPattern = ZenUnit::Random<string>();
   const string toRegexPattern = ZenUnit::Random<string>();
   const string justFilesSubstrings = ZenUnit::Random<string>();
   randomGeneratorMock.StringMock.ReturnValues(commandLine, fromRegexPattern, toRegexPattern, justFilesSubstrings);

   const int programModeInt = randomGeneratorMock.EnumMock.ReturnRandom();
   const ProgramMode programMode = static_cast<ProgramMode>(programModeInt);

   const fs::path targetDirectory = randomGeneratorMock.FilesystemPathMock.ReturnRandom();

   const bool recurse = ZenUnit::Random<bool>();
   const bool parallel = ZenUnit::Random<bool>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool dryrun = ZenUnit::Random<bool>();
   const bool quiet = ZenUnit::Random<bool>();
   const bool verbose = ZenUnit::Random<bool>();
   randomGeneratorMock.BoolMock.ReturnValues(recurse, parallel, skipFilesInUse, dryrun, quiet, verbose);
   //
   const FileRevisorArgs randomFileRevisorArgs = TestableRandomFileRevisorArgs(&randomGeneratorMock);
   //
   METALMOCK(randomGeneratorMock.StringMock.CalledNTimes(3));
   METALMOCK(randomGeneratorMock.EnumMock.CalledOnceWith(static_cast<int>(ProgramMode::MaxValue)));
   METALMOCK(randomGeneratorMock.FilesystemPathMock.CalledOnce());
   METALMOCK(randomGeneratorMock.BoolMock.CalledNTimes(6));
   FileRevisorArgs expectedRandomFileRevisorArgs;
   expectedRandomFileRevisorArgs.commandLine = commandLine;
   expectedRandomFileRevisorArgs.programMode = programMode;
   expectedRandomFileRevisorArgs.fromRegexPattern = fromRegexPattern;
   expectedRandomFileRevisorArgs.toRegexPattern = toRegexPattern;
   expectedRandomFileRevisorArgs.targetFolderPath = targetDirectory;
   expectedRandomFileRevisorArgs.recurse = recurse;
   expectedRandomFileRevisorArgs.parallel = parallel;
   expectedRandomFileRevisorArgs.skipFilesInUse = skipFilesInUse;
   expectedRandomFileRevisorArgs.dryrun = dryrun;
   expectedRandomFileRevisorArgs.quiet = quiet;
   expectedRandomFileRevisorArgs.verbose = verbose;
   ARE_EQUAL(expectedRandomFileRevisorArgs, randomFileRevisorArgs);
}

TEST(ZenUnitRandomFileRevisorArgs_CodeCoverage)
{
   ZenUnit::Random<FileRevisorArgs>();
}

RUN_TESTS(FileRevisorArgsEqualizerAndRandomTests)
