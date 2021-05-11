#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultEqualizerAndRandom.h"

namespace ZenUnit
{
   void Equalizer<RenameResult>::AssertEqual(const RenameResult& expectedRenameResult, const RenameResult& actualRenameResult)
   {
      FIELDS_ARE_EQUAL(expectedRenameResult, actualRenameResult, didRenameFileOrDirectory);
      FIELDS_ARE_EQUAL(expectedRenameResult, actualRenameResult, originalFileOrDirectoryPath);
      FIELDS_ARE_EQUAL(expectedRenameResult, actualRenameResult, renamedFileOrDirectoryPath);
   }

   RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator)
   {
      RenameResult randomRenameResult;
      randomRenameResult.didRenameFileOrDirectory = randomGenerator->Bool();
      randomRenameResult.originalFileOrDirectoryPath = randomGenerator->FilesystemPath();
      randomRenameResult.renamedFileOrDirectoryPath = randomGenerator->FilesystemPath();
      return randomRenameResult;
   }

   template<>
   RenameResult Random<RenameResult>()
   {
      ZenUnit::RandomGenerator randomGenerator;
      RenameResult randomRenameResult = TestableRenameResultRandom(&randomGenerator);
      return randomRenameResult;
   }
}
