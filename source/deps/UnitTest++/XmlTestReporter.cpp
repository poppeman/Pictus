#include "XmlTestReporter.h"

#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::ostringstream;
using std::ostream;

namespace {

void Replacechar(string& str, char const c, string const& replacement)
{
    for (size_t pos = str.find(c); pos != string::npos; pos = str.find(c, pos + 1))
        str.replace(pos, 1, replacement);
}

string XmlEscape(char const* value)
{
    string escaped = value;

    Replacechar(escaped, '&', "&amp;");
    Replacechar(escaped, '<', "&lt;");
    Replacechar(escaped, '>', "&gt;");
    Replacechar(escaped, '\'', "&apos;");
    Replacechar(escaped, '\"', "&quot;");
 
    return escaped;
}

string BuildFailureMessage(string const& file, int const line, string const& message)
{
    ostringstream failureMessage;
    failureMessage << file << "(" << line << ") : " << message;
    return failureMessage.str();
}

}

namespace UnitTest {

XmlTestReporter::XmlTestReporter(ostream& ostream)
    : m_ostream(ostream)
{
}

void XmlTestReporter::ReportSummary(int const totalTestCount, int const failedTestCount,
                                    int const failureCount, float const secondsElapsed)
{
    AddXmlElement(m_ostream, NULL);

    BeginResults(m_ostream, totalTestCount, failedTestCount, failureCount, secondsElapsed);

    DeferredTestResultList const& results = GetResults();
    for (DeferredTestResultList::const_iterator i = results.begin(); i != results.end(); ++i)
    {
        BeginTest(m_ostream, *i);

        if (i->failed)
            AddFailure(m_ostream, *i);

        EndTest(m_ostream, *i);
    }

    EndResults(m_ostream);
}

void XmlTestReporter::AddXmlElement(ostream& os, char const* encoding)
{
    os << "<?xml version=\"1.0\"";

    if (encoding != NULL)
        os << " encoding=\"" << encoding << "\"";

    os << "?>";
}

void XmlTestReporter::BeginResults(std::ostream& os, int const totalTestCount, int const failedTestCount, 
                                   int const failureCount, float const secondsElapsed)
{
   os << "<unittest-results"
       << " tests=\"" << totalTestCount << "\"" 
       << " failedtests=\"" << failedTestCount << "\"" 
       << " failures=\"" << failureCount << "\"" 
       << " time=\"" << secondsElapsed << "\""
       << ">";
}

void XmlTestReporter::EndResults(std::ostream& os)
{
    os << "</unittest-results>";
}

void XmlTestReporter::BeginTest(std::ostream& os, DeferredTestResult const& result)
{
    os << "<test"
        << " suite=\"" << result.suiteName << "\"" 
        << " name=\"" << result.testName << "\""
        << " time=\"" << result.timeElapsed << "\"";
}

void XmlTestReporter::EndTest(std::ostream& os, DeferredTestResult const& result)
{
    if (result.failed)
        os << "</test>";
    else
        os << "/>";
}

void XmlTestReporter::AddFailure(std::ostream& os, DeferredTestResult const& result)
{
    os << ">"; // close <test> element

    string const escapedMessage = XmlEscape(result.failureMessage);
    string const message = BuildFailureMessage(result.failureFile, result.failureLine, escapedMessage);

    os << "<failure"
        << " message=\"" << message << "\""
        << "/>";
}

}
