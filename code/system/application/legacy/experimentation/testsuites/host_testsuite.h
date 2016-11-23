#include "cxxtest/TestSuite.h"

class Host_TestSuite : public CxxTest::TestSuite {
public:
    void testSomething() {
        TS_ASSERT(true);
    }
};
