#ifndef WEBMIGRATE_H
#define WEBMIGRATE_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/replace.hpp>

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
                    bool AppendWhenMultiple = true  ///< Used to differentiate title from content if false
                   );

/**
    Grabs the text between GetS and GetE using In as the source
    \return Text between GetS and GetE
*/
    string GetBetween( string In, string GetS, string GetE );
    string ToSlug(string TextToConvert);
    bool ParseDivToFile(string DelimitedFile);
};

#endif // WEBMIGRATE_H

