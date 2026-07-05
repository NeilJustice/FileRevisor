#pragma once

namespace ZenUnit
{
   template<>
   struct Equalizer<FileRevisorArgs>
   {
      static void AssertEqual(
         const FileRevisorArgs& expectedArgs, const FileRevisorArgs& actualArgs);
   };

   FileRevisorArgs TestableRandomFileRevisorArgs(const ZenUnit::RandomGenerator* randomGenerator);
   template<>
   FileRevisorArgs Random();
}
