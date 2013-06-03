#include "../include/webmigrate.h"

using namespace boost;
using namespace std;

namespace WebMigrate
{

/**
    Grabs content between ParseBegin and ParseEnd
    \remarks Allows nesting by specifying a trigger [ParseTrigger] to ignore the next ParseEnd
    \return String - From ParseBegin to ParseEnd (filtering out these strings)
*/
string ParseDiv(string ParseText,               ///< Full input text
                string ParseBegin,              ///< Text which enables parsing
                string ParseTrigger,            ///< Increments ParseCounter
                string ParseEnd,                ///< Decrements ParseCounter
                bool AppendWhenMultiple         ///< Used to differentiate title from content if false
               )
{
    /// Initialize a PARSECOUNTER_NUM to ZERO
    int ParseCounter = 1;
    bool ParseEnabled = false;

    string OutputString;
    string line;

    stringstream NewText;

    NewText << ParseText;

    /// Read Input Text
    while(getline(NewText, line)) {
        iterator_range<string::iterator> range = algorithm::ifind_first(line, ParseBegin);

        /// If FINDME_STRING is found, turn the PARSEBEGIN_BOOL on
        if(!range.empty() && ParseEnabled == false)
        {
            ParseEnabled = true;
        }

        /// Only functions when Parse is enabled and div is actually closed
        if(ParseEnabled)
        {
            /// Any PARSEINCREMENT_STRING should increment the PARSECOUNTER_NUM by 1
            range = algorithm::ifind_first(line, ParseTrigger);
            if(!range.empty())
            {
                ParseCounter++;
            }

            /// Lines read while PARSEBEGIN_BOOL is TRUE should be placed into an OUTPUTBUFFER_OSSTREAM
            if(ParseCounter > 1) {
                OutputString += line + "\n";
            }

            /// Any PARSEDECREMENT_STRING should decrement the PARSECOUNTER_NUM by 1
            range = algorithm::ifind_first(line, ParseEnd);
            if(!range.empty())
            {
                ParseCounter--;

                /// IF 1-time-check break out of the While
                if(AppendWhenMultiple == false)
                {
                    break;
                }
            }
        }
    }

    algorithm::ireplace_all(OutputString, ParseBegin, "");
    algorithm::ireplace_all(OutputString, "<br />", "");
    algorithm::ireplace_last(OutputString, ParseEnd, "");
    trim(OutputString);

    /// Return the OUTPUTBUFFER_OSSTREAM as a STRING
    return OutputString;
}

/**
    Grabs the text between GetS and GetE using In as the source
    \return Text between GetS and GetE
*/

string GetBetween( string In, string GetS, string GetE )
{
    // Start after the first Get string
    size_t spos = ( In.find( GetS ) + GetS.length() );

    // End at the beginning of the second Get string
    size_t epos = In.rfind( GetE );

    // Get everything in between the two strings
    return In.substr( spos, epos - spos );
}

string ToSlug(string TextToConvert)
{
    string OutputString = "";
    string::iterator StringLoop;
    for(StringLoop = TextToConvert.begin(); StringLoop != TextToConvert.end(); StringLoop++)
    {
        char CurrentChar = *StringLoop;
        if(isalpha(CurrentChar) || isdigit(CurrentChar)) {
            OutputString += CurrentChar;
        }

        if(isspace(CurrentChar))
        {
            OutputString += '-';
        }
    }

    std::transform(OutputString.begin(), OutputString.end(), OutputString.begin(), ::tolower);
    return OutputString;
}

bool ParseDivToFile(string DelimitedFile)
{
    /// Read Input Text
    ifstream LinkList(DelimitedFile.c_str());
    stringstream NewText;
    string line;
    iterator_range<string::iterator> range;

    NewText << LinkList.rdbuf();

    while(getline(NewText, line)) {
        range = algorithm::ifind_first(line, "http://");

        if(range.empty())
        {
           continue;
        }

        string FilePath = "/tmp/unit-test.html";

        string WgetCommandPrep = "wget " + line + " -O " + FilePath;
        const char* WgetCommandExecute = WgetCommandPrep.c_str();

        //< Download
        int lRet = std::system(WgetCommandExecute);

        if(!lRet) {
            cerr << "wget failed to run. Aborting...";
            _exit(EXIT_FAILURE);
        }

        filesystem::path file(FilePath);

        ostringstream OutputString;
        ifstream infile(FilePath);

        OutputString << infile.rdbuf();

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

    return true;
}
};

