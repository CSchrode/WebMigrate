#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <string>
#include <sstream>
#include "include/webmigrate.h"
using namespace std;

#ifndef DEVELOPMENT
int main(int argc, char **argv)
{
    WebMigrate::ParseDivToFile(argv[1]);
}
#endif
