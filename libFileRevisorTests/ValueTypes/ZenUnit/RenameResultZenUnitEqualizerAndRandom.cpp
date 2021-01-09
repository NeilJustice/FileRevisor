#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<RenameResult>::AssertEqual(const RenameResult& expectedRenameResult, const RenameResult& actualRenameResult)
   {
      ARE_EQUAL(expectedRenameResult.didRenameFileOrDirectory, actualRenameResult.didRenameFileOrDirectory);
      ARE_EQUAL(expectedRenameResult.originalFileOrDirectoryPath, actualRenameResult.originalFileOrDirectoryPath);
      ARE_EQUAL(expectedRenameResult.renamedFileOrDirectoryPath, actualRenameResult.renamedFileOrDirectoryPath);
   }

   template<>
   RenameResult Random<RenameResult>()
   {
      ZenUnit::RandomGenerator randomGenerator;
      RenameResult randomRenameResult = ::TestableRenameResultRandom(&randomGenerator);
      return randomRenameResult;
   }
}

RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator)
{
   RenameResult randomRenameResult;
   randomRenameResult.didRenameFileOrDirectory = randomGenerator->Bool();
   randomRenameResult.originalFileOrDirectoryPath = randomGenerator->FilesystemPath();
   randomRenameResult.renamedFileOrDirectoryPath = randomGenerator->FilesystemPath();
   return randomRenameResult;
}
