#pragma once

namespace ZenUnit
{
   template<>
   struct Equalizer<FileRevisorArgs>
   {
      static void AssertEqual(const FileRevisorArgs& expectedFileRevisorArgs, const FileRevisorArgs& actualFileRevisorArgs);
   };

   template<>
   FileRevisorArgs Random();
};

FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator);
