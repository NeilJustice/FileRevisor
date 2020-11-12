#pragma once

namespace ZenUnit
{
   template<>
   struct Equalizer<RenameResult>
   {
      static void AssertEqual(const RenameResult& expectedRenameResult, const RenameResult& actualRenameResult);
   };

   template<>
   RenameResult Random<RenameResult>();
}

RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator);
