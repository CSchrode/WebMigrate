#include <unit++.h>
#include <sstream>

#ifndef WEBMIGRATE_H
#include "../../include/webmigrate.h"
#endif

using namespace unitpp;
using namespace std;
namespace
{
class UnitTest : public suite
{
    void testBasicParse()
    {
        ostringstream TestData;
        TestData << "<html>" << endl
        << "<head>" << endl
        << "<title></title>" << endl
        << "</head>" << endl
        << "<body>" << endl
        << "<div class=\"rightcontent\">" << endl
        << "Div Content" << endl
        << "</div>" << endl
        << "<body>" << endl
        << "</body>" << endl
        << "</html>" << endl;


        assert_eq("Basic Div Parse",
                  "Div Content",
                  WebMigrate::ParseDiv(TestData.str(),
                           "<div class=\"rightcontent\">",
                           "<div",
                           "</div>"
                          )
                 );
    }

    void testComplexParse()
    {
        ostringstream TestData;
        ostringstream CorrectData;

        TestData << "<html>" << endl
        << "<head>" << endl
        << "<title></title>" << endl
        << "</head>" << endl
        << "<body>" << endl
        << "<div class=\"rightcontent\">" << endl
        << "Div Content" << endl
        << "Div Content" << endl
        << "<div class=\"pagetitle\">" << endl
        << "This Is The Title!" << endl
        << "</div>" << endl
        << "Div Content" << endl
        << "Div Content" << endl
        << "</div>" << endl
        << "<body>" << endl
        << "</body>" << endl
        << "</html>" << endl;

        CorrectData << "Div Content" << endl
        << "Div Content" << endl
        << "<div class=\"pagetitle\">" << endl
        << "This Is The Title!" << endl
        << "</div>" << endl
        << "Div Content" << endl
        << "Div Content";


        assert_eq("Advanced Div Parse",
                  CorrectData.str(),
                  WebMigrate::ParseDiv(TestData.str(),
                           "<div class=\"rightcontent\">",
                           "<div",
                           "</div>"
                          )
                 );
    }

    void testParseTitle()
    {
        ostringstream TestData;
        TestData << "<html>" << endl
        << "<head>" << endl
        << "<title></title>" << endl
        << "</head>" << endl
        << "<body>" << endl
        << "<div class=\"rightcontent\">" << endl
        << "Div Content" << endl
        << "Div Content" << endl
        << "<div class=\"pagetitle\">" << endl
        << "This Is The Title!" << endl
        << "</div>" << endl
        << "Div Content" << endl
        << "Div Content" << endl
        << "</div>" << endl
        << "<body>" << endl
        << "</body>" << endl
        << "</html>" << endl;

        assert_eq("Title Div Parse",
                  "This Is The Title!",
                  WebMigrate::ParseDiv(TestData.str(),
                           "<div class=\"pagetitle\">",
                           "<div",
                           "</div>",
                           false
                          )
                 );
    }

    void testTitleSlug()
    {
        const string Title = "This Is The Title!";

        assert_eq("Title Slug", "this-is-the-title", WebMigrate::ToSlug(Title));
    }

    void testPageTitleToFile()
    {
        const string Title = "This Is The Title!";

        ofstream OutputFile(WebMigrate::ToSlug(Title), ios::trunc);
        OutputFile << "Important Text!" << endl;
        OutputFile.close();

        boost::filesystem::path file(WebMigrate::ToSlug(Title));

        //< Test existence of file
        assert_true("Couldn't convert Title to a file!", exists(file));

        remove(WebMigrate::ToSlug(Title).c_str());
    }

    void testParseDivToFile()
    {
        string FilePath = "/tmp/unit-test.html";

        string WgetCommandPrep = "wget http://www.wadsworthlibrary.com/main/services.cfm?page_id=60 -O " + FilePath;
        const char* WgetCommandExecute = WgetCommandPrep.c_str();

        //< Download
        int lRet = std::system(WgetCommandExecute);

        if(!lRet) {
            cerr << "wget failed to run. Aborting...";
            _exit(EXIT_FAILURE);
        }

        boost::filesystem::path file(FilePath);

        //< Test existence of file
        assert_true("ParseDivToFile didn't successfully download a page", exists(file));

        ostringstream OutputString;
        ifstream infile(FilePath);

        OutputString << infile.rdbuf();

        assert_false("ParseDivToFile didn't find data", OutputString.str() == "");

        string FoundTitle = WebMigrate::ParseDiv(OutputString.str(),
            "<div class=\"title\">",
            "<div",
            "</div>",
            false);

        if(FoundTitle.empty()) {
            FoundTitle = WebMigrate::ParseDiv(OutputString.str(),
                "<div class=\"page_title\">",
                "<div",
                "</div>",
                false
            );
        }


        if(FoundTitle.empty())
        {
            remove(FilePath.c_str());
        }

        assert_false("Didn't find title", FoundTitle == "");

        //assert_eq("Title isn't formatted correctly", "about-the-library", WebMigrate::ToSlug(FoundTitle));

        ofstream outfile(WebMigrate::ToSlug(FoundTitle) + ".php", ios::trunc);

        if(outfile.is_open()) {
            string FormatText = WebMigrate::GetBetween(OutputString.str(),
                        "<div class=\"rightcontent\">",
                        "<div class=\"footer\">"
                        );

            algorithm::ireplace_last(FormatText, "</div>", "");

            outfile << FormatText;
        }

        remove(FilePath.c_str());
    }

    void testWget()
    {
        string FilePath = "/tmp/unit-test.html";

        string WgetCommandPrep = "wget http://wadsworthlibrary.com -O " + FilePath;
        const char* WgetCommandExecute = WgetCommandPrep.c_str();

        //< Download
        int lRet = std::system(WgetCommandExecute);

        if(!lRet) {
            cerr << "wget failed to run. Aborting...";
            _exit(EXIT_FAILURE);
        }

        boost::filesystem::path file(FilePath);

        //< Test existence of file
        assert_true("WGET didn't successfully download a page", exists(file));

        remove(FilePath.c_str());
    }

public:
    UnitTest() : suite("UnitTest") {
        add("Parsing", testcase(this, "Basic Parse", &UnitTest::testBasicParse));
        add("Parsing", testcase(this, "Complex Parse", &UnitTest::testComplexParse));
        add("Parsing", testcase(this, "Title Parse", &UnitTest::testParseTitle));
        add("StringManip", testcase(this, "Title Slug", &UnitTest::testTitleSlug));
        add("File", testcase(this, "Title File IO", &UnitTest::testPageTitleToFile));
        add("Acceptence", testcase(this, "Acceptence Test", &UnitTest::testParseDivToFile));
        add("wget", testcase(this, "Basic_testWget", &UnitTest::testWget));
        suite::main().add("UnitTest", this);
    }
};
UnitTest* TestCase = new UnitTest();
}
