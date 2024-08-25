#pragma once

namespace ZenUnit
{
   template<>
   struct Equalizer<RenameResult>
   {
      static void AssertEqual(const RenameResult& expectedRenameResult, const RenameResult& actualRenameResult);
   };

   RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator);
   template<>
   RenameResult Random<RenameResult>();
}
