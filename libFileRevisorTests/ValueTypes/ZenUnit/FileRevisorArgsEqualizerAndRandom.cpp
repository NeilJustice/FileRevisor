#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<FileRevisorArgs>::AssertEqual(
      const FileRevisorArgs& expectedFileRevisorArgs, const FileRevisorArgs& actualFileRevisorArgs)
   {
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, commandLine);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, programMode);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, fromRegexPattern);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, toRegexPattern);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, targetFolderPath);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, recurse);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, parallel);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, skipFilesInUse);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, dryrun);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, quiet);
      FIELDS_ARE_EQUAL(expectedFileRevisorArgs, actualFileRevisorArgs, verbose);
   }

   FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator)
   {
      FileRevisorArgs randomFileRevisorArgs;
      randomFileRevisorArgs.commandLine = randomGenerator->String();
      randomFileRevisorArgs.programMode = static_cast<ProgramMode>(randomGenerator->Enum(static_cast<int>(ProgramMode::MaxValue)));
      randomFileRevisorArgs.fromRegexPattern = randomGenerator->String();
      randomFileRevisorArgs.toRegexPattern = randomGenerator->String();
      randomFileRevisorArgs.targetFolderPath = randomGenerator->FilesystemPath();
      randomFileRevisorArgs.recurse = randomGenerator->Bool();
      randomFileRevisorArgs.parallel = randomGenerator->Bool();
      randomFileRevisorArgs.skipFilesInUse = randomGenerator->Bool();
      randomFileRevisorArgs.dryrun = randomGenerator->Bool();
      randomFileRevisorArgs.quiet = randomGenerator->Bool();
      randomFileRevisorArgs.verbose = randomGenerator->Bool();
      return randomFileRevisorArgs;
   }

   template<>
   FileRevisorArgs Random()
   {
      ZenUnit::RandomGenerator randomGenerator;
      FileRevisorArgs randomFileRevisorArgs = TestableRandomFileRevisorArgs(&randomGenerator);
      return randomFileRevisorArgs;
   }
}
