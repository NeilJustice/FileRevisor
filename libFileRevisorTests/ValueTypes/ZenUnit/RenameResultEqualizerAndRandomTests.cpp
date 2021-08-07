#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultEqualizerAndRandom.h"

TESTS(RenameResultZenUnitEqualizerAndRandomTests)
AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
AFACT(TestableRenameResultRandom_ReturnsRenameResultWithAllRandomFields)
AFACT(ZenUnitRandomRenameResult_CodeCoverage)
EVIDENCE

TEST(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
{
   ZENUNIT_EQUALIZER_TEST_SETUP(RenameResult);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, didRenameFileOrDirectory, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, originalFileOrDirectoryPath, ZenUnit::Random<fs::path>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, renamedFileOrDirectoryPath, ZenUnit::Random<fs::path>());
}

TEST(TestableRenameResultRandom_ReturnsRenameResultWithAllRandomFields)
{
   ZenUnit::RandomGeneratorMock randomGeneratorMock;
   const bool didRenameFileOrDirectory = randomGeneratorMock.BoolMock.ReturnRandom();
   const fs::path originalPath = ZenUnit::Random<fs::path>();
   const fs::path renamedFileOrDirectoryPath = ZenUnit::Random<fs::path>();
   randomGeneratorMock.FilesystemPathMock.ReturnValues(originalPath, renamedFileOrDirectoryPath);
   //
   const RenameResult randomRenameResult = ZenUnit::TestableRenameResultRandom(&randomGeneratorMock);
   //
   METALMOCK(randomGeneratorMock.BoolMock.CalledOnce());
   METALMOCK(randomGeneratorMock.FilesystemPathMock.CalledNTimes(2));
   RenameResult expectedRandomRenameResult;
   expectedRandomRenameResult.didRenameFileOrDirectory = didRenameFileOrDirectory;
   expectedRandomRenameResult.originalFileOrDirectoryPath = originalPath;
   expectedRandomRenameResult.renamedFileOrDirectoryPath = renamedFileOrDirectoryPath;
   ARE_EQUAL(expectedRandomRenameResult, randomRenameResult);
}

TEST(ZenUnitRandomRenameResult_CodeCoverage)
{
   ZenUnit::Random<RenameResult>();
}

RUN_TESTS(RenameResultZenUnitEqualizerAndRandomTests)
