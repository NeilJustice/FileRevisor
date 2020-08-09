#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"

TESTS(FileRevisorArgsEqualizerAndRandomTests)
AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
AFACT(ZenUnitRandom_FileRevisorArgs_ReturnsRandomFileRevisorArgs)
AFACT(TestableRandomFileRevisorArgs_ReturnsFileRevisorArgsWithAllRandomFields)
EVIDENCE

TEST(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
{
   SETUP_EQUALIZER_TEST(FileRevisorArgs);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, commandLine, ZenUnit::Random<string>());
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, programMode, ProgramMode::ReplaceTextInTextFiles);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, fromRegexPattern, ZenUnit::Random<string>());
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, toRegexPattern, ZenUnit::Random<string>());
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, targetDirectoryPath, ZenUnit::Random<fs::path>());
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, recursive, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, parallel, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, skipFilesInUse, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, preview, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, minimal, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(FileRevisorArgs, verbose, true);
}

TEST(ZenUnitRandom_FileRevisorArgs_ReturnsRandomFileRevisorArgs)
{
   const FileRevisorArgs randomFileRevisorArgs = ZenUnit::Random<FileRevisorArgs>();
}

TEST(TestableRandomFileRevisorArgs_ReturnsFileRevisorArgsWithAllRandomFields)
{
   MetalMock::RandomGeneratorMock randomGeneratorMock;

   const string commandLine = ZenUnit::Random<string>();
   const string fromRegexPattern = ZenUnit::Random<string>();
   const string toRegexPattern = ZenUnit::Random<string>();
   const string justFilesSubstrings = ZenUnit::Random<string>();
   randomGeneratorMock.StringMock.ReturnValues(commandLine, fromRegexPattern, toRegexPattern, justFilesSubstrings);

   const int programModeInt = randomGeneratorMock.EnumMock.ReturnRandom();
   const ProgramMode programMode = static_cast<ProgramMode>(programModeInt);

   const fs::path targetDirectory = randomGeneratorMock.PathMock.ReturnRandom();

   const bool recursive = ZenUnit::Random<bool>();
   const bool parallel = ZenUnit::Random<bool>();
   const bool skipFilesInUse = ZenUnit::Random<bool>();
   const bool preview = ZenUnit::Random<bool>();
   const bool minimal = ZenUnit::Random<bool>();
   const bool verbose = ZenUnit::Random<bool>();
   randomGeneratorMock.BoolMock.ReturnValues(
      recursive, parallel, skipFilesInUse, preview, minimal, verbose);
   //
   const FileRevisorArgs randomFileRevisorArgs = TestableRandomFileRevisorArgs(&randomGeneratorMock);
   //
   METALMOCK(randomGeneratorMock.StringMock.CalledNTimes(3));
   METALMOCK(randomGeneratorMock.EnumMock.CalledOnceWith(static_cast<int>(ProgramMode::MaxValue)));
   METALMOCK(randomGeneratorMock.PathMock.CalledOnce());
   METALMOCK(randomGeneratorMock.BoolMock.CalledNTimes(6));
   FileRevisorArgs expectedRandomFileRevisorArgs;
   expectedRandomFileRevisorArgs.commandLine = commandLine;
   expectedRandomFileRevisorArgs.programMode = programMode;
   expectedRandomFileRevisorArgs.fromRegexPattern = fromRegexPattern;
   expectedRandomFileRevisorArgs.toRegexPattern = toRegexPattern;
   expectedRandomFileRevisorArgs.targetDirectoryPath = targetDirectory;
   expectedRandomFileRevisorArgs.recursive = recursive;
   expectedRandomFileRevisorArgs.parallel = parallel;
   expectedRandomFileRevisorArgs.skipFilesInUse = skipFilesInUse;
   expectedRandomFileRevisorArgs.preview = preview;
   expectedRandomFileRevisorArgs.minimal = minimal;
   expectedRandomFileRevisorArgs.verbose = verbose;
   ARE_EQUAL(expectedRandomFileRevisorArgs, randomFileRevisorArgs);
}

RUN_TESTS(FileRevisorArgsEqualizerAndRandomTests)
