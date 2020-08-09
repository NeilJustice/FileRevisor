#pragma once
#include "libFileRevisor/ValueTypes/RenameResult.h"

namespace ZenUnit
{
   template<>
   struct Equalizer<RenameResult>
   {
      static void AssertEqual(const RenameResult& expectedRenameResult, const RenameResult& actualRenameResult);
   };

   template<>
   RenameResult Random<RenameResult>();

   template<>
   class Printer<RenameResult>
   {
   public:
      static void Print(ostream& os, const RenameResult& fileRenameResult);
   };
}

RenameResult TestableRenameResultRandom(const ZenUnit::RandomGenerator* randomGenerator);
