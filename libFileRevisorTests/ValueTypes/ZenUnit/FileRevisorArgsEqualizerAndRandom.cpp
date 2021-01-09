#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<FileRevisorArgs>::AssertEqual(
      const FileRevisorArgs& expectedFileRevisorArgs, const FileRevisorArgs& actualFileRevisorArgs)
   {
      ARE_EQUAL(expectedFileRevisorArgs.commandLine, actualFileRevisorArgs.commandLine);
      ARE_EQUAL(expectedFileRevisorArgs.programMode, actualFileRevisorArgs.programMode);
      ARE_EQUAL(expectedFileRevisorArgs.fromRegexPattern, actualFileRevisorArgs.fromRegexPattern);
      ARE_EQUAL(expectedFileRevisorArgs.toRegexPattern, actualFileRevisorArgs.toRegexPattern);
      ARE_EQUAL(expectedFileRevisorArgs.targetDirectoryPath, actualFileRevisorArgs.targetDirectoryPath);
      ARE_EQUAL(expectedFileRevisorArgs.recurse, actualFileRevisorArgs.recurse);
      ARE_EQUAL(expectedFileRevisorArgs.parallel, actualFileRevisorArgs.parallel);
      ARE_EQUAL(expectedFileRevisorArgs.skipFilesInUse, actualFileRevisorArgs.skipFilesInUse);
      ARE_EQUAL(expectedFileRevisorArgs.preview, actualFileRevisorArgs.preview);
      ARE_EQUAL(expectedFileRevisorArgs.minimal, actualFileRevisorArgs.minimal);
      ARE_EQUAL(expectedFileRevisorArgs.verbose, actualFileRevisorArgs.verbose);
   }

   template<>
   FileRevisorArgs Random()
   {
      ZenUnit::RandomGenerator randomGenerator;
      FileRevisorArgs randomFileRevisorArgs = TestableRandomFileRevisorArgs(&randomGenerator);
      return randomFileRevisorArgs;
   }
}

FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator)
{
   FileRevisorArgs randomFileRevisorArgs;
   randomFileRevisorArgs.commandLine = randomGenerator->String();
   randomFileRevisorArgs.programMode = static_cast<ProgramMode>(randomGenerator->Enum(static_cast<int>(ProgramMode::MaxValue)));
   randomFileRevisorArgs.fromRegexPattern = randomGenerator->String();
   randomFileRevisorArgs.toRegexPattern = randomGenerator->String();
   randomFileRevisorArgs.targetDirectoryPath = randomGenerator->FilesystemPath();
   randomFileRevisorArgs.recurse = randomGenerator->Bool();
   randomFileRevisorArgs.parallel = randomGenerator->Bool();
   randomFileRevisorArgs.skipFilesInUse = randomGenerator->Bool();
   randomFileRevisorArgs.preview = randomGenerator->Bool();
   randomFileRevisorArgs.minimal = randomGenerator->Bool();
   randomFileRevisorArgs.verbose = randomGenerator->Bool();
   return randomFileRevisorArgs;
}
