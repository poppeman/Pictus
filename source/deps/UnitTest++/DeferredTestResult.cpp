#include "DeferredTestResult.h"

#include <cstdlib>

namespace UnitTest
{

DeferredTestResult::DeferredTestResult()
	: suiteName(NULL)
	, testName(NULL)
	, failureFile(NULL)
	, failureMessage(NULL)
	, failureLine(0)
	, timeElapsed(0.0f)
	, failed(false)
{
}

DeferredTestResult::DeferredTestResult(char const* suite, char const* test)
	: suiteName(suite)
	, testName(test)
	, failureFile(NULL)
	, failureMessage(NULL)
	, failureLine(0)
	, timeElapsed(0.0f)
	, failed(false)
{
}

}
