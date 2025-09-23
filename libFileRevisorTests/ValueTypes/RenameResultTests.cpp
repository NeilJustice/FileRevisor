#include "pch.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

TESTS(RenameResultTests)
AFACT(DefaultConstructor_SetsFieldsToDefaultValues)
AFACT(ThreeArgConstructor_SetsFields)
FACTS(DidRenameFileOrDirectoryFieldIsTrue_ReturnsTrueIfDidRenameFileOrDirectoryFieldIsTrue)
EVIDENCE

TEST(DefaultConstructor_SetsFieldsToDefaultValues)
{
   const RenameResult defaultRenameResult;
   RenameResult expectedDefaultRenameResult;
   expectedDefaultRenameResult.didRenameFileOrDirectory = false;
   expectedDefaultRenameResult.originalFileOrFolderPath = fs::path();
   expectedDefaultRenameResult.renamedFileOrFolderPath = fs::path();
   ARE_EQUAL(expectedDefaultRenameResult, defaultRenameResult);
}

TEST(ThreeArgConstructor_SetsFields)
{
   const bool didRenameFileOrDirectory = ZenUnit::Random<bool>();
   const fs::path originalPath = ZenUnit::Random<fs::path>();
   const fs::path renamedFileOrFolderPath = ZenUnit::Random<fs::path>();
   //
   const RenameResult renameResult(didRenameFileOrDirectory, originalPath, renamedFileOrFolderPath);
   //
   ARE_EQUAL(didRenameFileOrDirectory, renameResult.didRenameFileOrDirectory);
   ARE_EQUAL(originalPath, renameResult.originalFileOrFolderPath);
   ARE_EQUAL(renamedFileOrFolderPath, renameResult.renamedFileOrFolderPath);
}

TEST2X2(DidRenameFileOrDirectoryFieldIsTrue_ReturnsTrueIfDidRenameFileOrDirectoryFieldIsTrue,
   bool didRenameFileOrDirectoryFieldValue, bool expectedReturnValue,
   false, false,
   true, true)
{
   RenameResult renameResult = ZenUnit::Random<RenameResult>();
   renameResult.didRenameFileOrDirectory = didRenameFileOrDirectoryFieldValue;
   //
   bool didRenameFileOrDirectoryFieldIsTrue = RenameResult::DidRenameFileOrDirectoryFieldIsTrue(renameResult);
   //
   ARE_EQUAL(expectedReturnValue, didRenameFileOrDirectoryFieldIsTrue);
}

RUN_TESTS(RenameResultTests)
