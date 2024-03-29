#include "pch.h"
#include "libFileRevisorTests/Components/Utility/Docopt/ZenUnit/docoptvalueRandom.h"

TESTS(docoptvalueRandomTests)
AFACT(Random_ReturnsdocoptvalueWithAllNonDefaultFields)
EVIDENCE

TEST(Random_ReturnsdocoptvalueWithAllNonDefaultFields)
{
   const docopt::Value randomDocoptValue = ZenUnit::Random<docopt::Value>();
   IS_TRUE(randomDocoptValue.IsLong() || randomDocoptValue.IsString());
}

RUN_TESTS(docoptvalueRandomTests)
