#pragma once

namespace ZenUnit
{
   template<>
   struct Equalizer<FileRevisorArgs>
   {
      static void AssertEqual(const FileRevisorArgs& expectedFileRevisorArgs, const FileRevisorArgs& actualFileRevisorArgs);
   };

   FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator);
   template<>
   FileRevisorArgs Random();
}
