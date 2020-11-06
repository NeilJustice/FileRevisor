#include "pch.h"
#include "libFileRevisor/ValueTypes/RenameResult.h"

TESTS(RenameResultTests)
AFACT(DefaultConstructor_SetsFieldsToDefaults)
AFACT(ThreeArgConstructor_SetsFields)
FACTS(DidRenameFileOrDirectoryFieldIsTrue_ReturnsTrueIfDidRenameFileOrDirectoryFieldIsTrue)
EVIDENCE

TEST(DefaultConstructor_SetsFieldsToDefaults)
{
   RenameResult renameResult;
   IS_FALSE(renameResult.didRenameFileOrDirectory);
   IS_DEFAULT_VALUE(renameResult.originalFileOrDirectoryPath);
   IS_DEFAULT_VALUE(renameResult.renamedFileOrDirectoryPath);
}

TEST(ThreeArgConstructor_SetsFields)
{
   const bool didRenameFileOrDirectory = ZenUnit::Random<bool>();
   const fs::path originalPath = ZenUnit::Random<fs::path>();
   const fs::path renamedFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   //
   const RenameResult renameResult(didRenameFileOrDirectory, originalPath, renamedFileOrDirectoryPath);
   //
   ARE_EQUAL(didRenameFileOrDirectory, renameResult.didRenameFileOrDirectory);
   ARE_EQUAL(originalPath, renameResult.originalFileOrDirectoryPath);
   ARE_EQUAL(renamedFileOrDirectoryPath, renameResult.renamedFileOrDirectoryPath);
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
