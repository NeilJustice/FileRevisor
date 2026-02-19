#include "pch.h"
#include "libFileRevisorTests/Components/Docopt/ZenUnit/docoptvalueRandom.h"

TESTS(docoptvalueRandomTests)
AFACT(Random_ReturnsdocoptvalueWithAllNonDefaultFields)
EVIDENCE

TEST(Random_ReturnsdocoptvalueWithAllNonDefaultFields)
{
   const docopt::value randomDocoptValue = ZenUnit::Random<docopt::value>();
   IS_TRUE(randomDocoptValue.isLong() || randomDocoptValue.isString());
}

RUN_TESTS(docoptvalueRandomTests)
