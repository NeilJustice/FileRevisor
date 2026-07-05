#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/FileRevisorArgsEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<FileRevisorArgs>::AssertEqual(
      const FileRevisorArgs& expectedArgs, const FileRevisorArgs& actualArgs)
   {
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, commandLine);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, programMode);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, fromFileOrDirectoryName);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, toFileOrDirectoryName);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, targetDirectoryPath);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, contentsOnly);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, recurse);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, parallel);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, skipFilesInUse);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, dryrun);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, quiet);
      FIELDS_ARE_EQUAL(expectedArgs, actualArgs, verbose);
   }

   FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator)
   {
      FileRevisorArgs randomFileRevisorArgs;
      randomFileRevisorArgs.commandLine = randomGenerator->String();
      randomFileRevisorArgs.programMode = static_cast<ProgramMode>(randomGenerator->Enum(static_cast<int>(ProgramMode::MaxValue)));
      randomFileRevisorArgs.fromFileOrDirectoryName = randomGenerator->String();
      randomFileRevisorArgs.toFileOrDirectoryName = randomGenerator->String();
      randomFileRevisorArgs.targetDirectoryPath = randomGenerator->FilesystemPath();
      randomFileRevisorArgs.contentsOnly = randomGenerator->Bool();
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
      return TestableRandomFileRevisorArgs(ZenUnit::RandomGenerator::Instance());
   }
}
