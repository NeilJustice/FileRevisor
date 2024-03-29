#include "pch.h"
#include "libFileRevisor/StaticUtilities/Vector.h"

TESTS(VectorTests)
AFACT(FromArgcArgv_ReturnsArgsVector)
AFACT(Join_EmptyIntVector_ReturnsEmptyStringRegardlessOfSeparator)
AFACT(Join_1ElementIntVector_ReturnsFirstElementRegardlessOfSeparator)
FACTS(Join_2ElementIntVector_ReturnsElementsJoinedBySeparator)
EVIDENCE

TEST(FromArgcArgv_ReturnsArgsVector)
{
   const char* const chars1[] = { "ExePath" };
   const vector<string> expectedArgsA = { "ExePath" };
   VECTORS_ARE_EQUAL(expectedArgsA, Vector::FromArgcArgv(1, const_cast<char**>(chars1)));

   const char* const chars2[] = { "ExePath", "Arg1" };
   const vector<string> expectedArgsB = { "ExePath", "Arg1" };
   VECTORS_ARE_EQUAL(expectedArgsB, Vector::FromArgcArgv(2, const_cast<char**>(chars2)));

   const char* const chars3[] = { "ExePath", "Arg1", "Arg2" };
   const vector<string> expectedArgsC = { "ExePath", "Arg1", "Arg2" };
   VECTORS_ARE_EQUAL(expectedArgsC, Vector::FromArgcArgv(3, const_cast<char**>(chars3)));
}

TEST(Join_EmptyIntVector_ReturnsEmptyStringRegardlessOfSeparator)
{
   const vector<int> emptyIntVector = {};
   IS_EMPTY_STRING(Vector::Join(emptyIntVector, ' '));
   IS_EMPTY_STRING(Vector::Join(emptyIntVector, ','));
}

TEST(Join_1ElementIntVector_ReturnsFirstElementRegardlessOfSeparator)
{
   const vector<int> intVector = { 1 };
   ARE_EQUAL("1", Vector::Join(intVector, ' '));
   ARE_EQUAL("1", Vector::Join(intVector, ','));
}

TEST3X3(Join_2ElementIntVector_ReturnsElementsJoinedBySeparator,
   string_view expectedJoinedVector, const vector<int>& elements, char separator,
   "1 2", vector<int>{ 1, 2 }, ' ',
   "1,2", vector<int>{ 1, 2 }, ',',
   "1 2 3", vector<int>{ 1, 2, 3 }, ' ',
   "1,2,3", vector<int>{ 1, 2, 3 }, ',')
{
   const string joinedVector = Vector::Join(elements, separator);
   ARE_EQUAL(expectedJoinedVector, joinedVector);
}

RUN_TESTS(VectorTests)
