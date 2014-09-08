#ifndef UNITTEST_DEFERREDTESTRESULT_H
#define UNITTEST_DEFERREDTESTRESULT_H

namespace UnitTest
{

struct DeferredTestResult
{
	DeferredTestResult();
    DeferredTestResult(char const* suite, char const* test);

    char const* suiteName;
    char const* testName;
    char const* failureFile;
    char const* failureMessage;
    int failureLine;
    float timeElapsed;
	bool failed;
};

}

#endif //UNITTEST_DEFERREDTESTRESULT_H
