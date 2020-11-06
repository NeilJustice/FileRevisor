#include "pch.h"
#include "libFileRevisorTests/ValueTypes/ZenUnit/RenameResultZenUnitEqualizerAndRandom.h"

TESTS(RenameResultZenUnitEqualizerAndRandomTests)
AFACT(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
AFACT(ZenUnitRandom_ReturnsRenameResultWithAllNonDefaultValueFields)
AFACT(TestableRenameResultRandom_ReturnsRenameResultWithAllRandomFields)
EVIDENCE

TEST(ZenUnitEqualizer_ThrowsIfAnyFieldsNotEqual)
{
   ZENUNIT_EQUALIZER_TEST_SETUP(RenameResult);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, didRenameFileOrDirectory, true);
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, originalFileOrDirectoryPath, ZenUnit::Random<fs::path>());
   ZENUNIT_EQUALIZER_THROWS_WHEN_FIELD_NOT_EQUAL(RenameResult, renamedFileOrDirectoryPath, ZenUnit::Random<fs::path>());
}

TEST(ZenUnitRandom_ReturnsRenameResultWithAllNonDefaultValueFields)
{
   const RenameResult randomRenameResult = ZenUnit::Random<RenameResult>();
   // randomRenameResult.didRenameFileOrDirectory is a bool that ZenUnit::RandomGenerator::Bool() can sometimes set to false
   IS_NOT_DEFAULT_VALUE(randomRenameResult.originalFileOrDirectoryPath);
   IS_NOT_DEFAULT_VALUE(randomRenameResult.renamedFileOrDirectoryPath);
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

RUN_TESTS(RenameResultZenUnitEqualizerAndRandomTests)
