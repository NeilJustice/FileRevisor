#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitFunctions.h"

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
      const RenameResult randomRenameResult = ::TestableRenameResultRandom(&randomGenerator);
      return randomRenameResult;
   }

   void Printer<RenameResult>::Print(ostream& os, const RenameResult& fileRenameResult)
   {
      os << "RenameResult(didRenameFile=" << boolalpha << fileRenameResult.didRenameFileOrDirectory
         << ", originalFilePath=" << fileRenameResult.originalFileOrDirectoryPath
         << ", renamedFilePath=" << fileRenameResult.renamedFileOrDirectoryPath << ")";
   }
}

RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator)
{
   RenameResult randomRenameResult;
   randomRenameResult.didRenameFileOrDirectory = randomGenerator->Bool();
   randomRenameResult.originalFileOrDirectoryPath = randomGenerator->Path();
   randomRenameResult.renamedFileOrDirectoryPath = randomGenerator->Path();
   return randomRenameResult;
}
