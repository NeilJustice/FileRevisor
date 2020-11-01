#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitFunctions.h"

TESTS(RenameResultZenUnitFunctionsTests)
AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
AFACT(ZenUnitRandom_CodeCoverage)
AFACT(TestableRenameResultRandom_ReturnsRenameResultWithAllRandomFields)
AFACT(ZenUnitPrinter_WritesExpectedStringRepresentation)
EVIDENCE

TEST(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
{
   SETUP_EQUALIZER_TEST(RenameResult);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, didRenameFileOrDirectory, true);
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, originalFileOrDirectoryPath, ZenUnit::Random<fs::path>());
   EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, renamedFileOrDirectoryPath, ZenUnit::Random<fs::path>());
}

TEST(ZenUnitRandom_CodeCoverage)
{
   const RenameResult randomRenameResult = ZenUnit::Random<RenameResult>();
}

TEST(TestableRenameResultRandom_ReturnsRenameResultWithAllRandomFields)
{
   ZenUnit::RandomGeneratorMock randomGeneratorMock;
   const bool didRenameFileOrDirectory = randomGeneratorMock.BoolMock.ReturnRandom();
   const fs::path originalPath = ZenUnit::Random<fs::path>();
   const fs::path renamedFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   randomGeneratorMock.FilesystemPathMock.ReturnValues(originalPath, renamedFileOrDirectoryPath);
   //
   const RenameResult randomRenameResult = ::TestableRenameResultRandom(&randomGeneratorMock);
   //
   METALMOCK(randomGeneratorMock.BoolMock.CalledOnce());
   METALMOCK(randomGeneratorMock.FilesystemPathMock.CalledNTimes(2));
   RenameResult expectedRandomRenameResult;
   expectedRandomRenameResult.didRenameFileOrDirectory = didRenameFileOrDirectory;
   expectedRandomRenameResult.originalFileOrDirectoryPath = originalPath;
   expectedRandomRenameResult.renamedFileOrDirectoryPath = renamedFileOrDirectoryPath;
   ARE_EQUAL(expectedRandomRenameResult, randomRenameResult);
}

TEST(ZenUnitPrinter_WritesExpectedStringRepresentation)
{
   ostringstream oss;
   const RenameResult renameResult = ZenUnit::Random<RenameResult>();
   //
   ZenUnit::Printer<RenameResult>::Print(oss, renameResult);
   //
   ostringstream expectedPrintMaker;
   expectedPrintMaker << "RenameResult(didRenameFile=" << boolalpha << renameResult.didRenameFileOrDirectory
      << ", originalFilePath=" << renameResult.originalFileOrDirectoryPath
      << ", renamedFilePath=" << renameResult.renamedFileOrDirectoryPath << ")";
   const string expectedPrint = expectedPrintMaker.str();
   const string actualPrint = oss.str();
   ARE_EQUAL(expectedPrint, actualPrint);
}

RUN_TESTS(RenameResultZenUnitFunctionsTests)
