Outline of the necessary features
=====


Parsing
-----

1. Must be able to get within <div class="rightcontent"> to </div>
1. There may be other <div> tags within that parent containter. Keep these.
1. All links within should point to www.wadsworthlibrary.com - May use wget, study command switches


Implementation
-----

*Simplest Thing That Could Possibly Work*

Folder Structure:
    ./
        WebMigrate/
            ./old_site
                [WGET OUTPUT]

            ./finished
                [WEBMIGRATE OUTPUT]

            ./webmigrate (binary)


1. System call to wget mirroring website in a temporary directory - try to uniquely identify by date (no big deal though)
1. There should be another directory for successfully parsed input
1. After wget-ing everything, ensure that links point to http://wadsworthlibrary.com
1. Proceed to scan each file found for <div class="rightcontent">
1. If contained, grab only the text WITHIN THE DIVS (ignoring any </div> after successfully finding a <div> contained inside the <div class="rightcontainer">)
1. Output the selection to a new file in *./finished*

*Validation*

1. SEE PARSING NOTE #2 AGAIN
1. Ensure wget makes the links point to wadsworthlibrary.com and not relative paths (maybe recursive but no mirror?)


API Necessary
-----

*File IO*

1. unsure - Find current directory in Linux
1. ifstream? - Recursively scan through folders inside a folder
1. ofstream - Create directories
1. ofstream - Read/Write Files

*String IO*

1. Modified GetBetween function, built to adapt (div counter?) to <divs> contained within
1. This should make extensive use of string library's find, rfind, and instr

*wget-Wrapper*

1. Should write a simple engine for using wget with the switches needed to output it to a directory in a recursive, spidering kind of way
1. This can probe the sitemap (I think we have one) or even any basic page and spider it
1. May also use curl if wget isn't powerful enough


OTHER PROJECTS
=====

1. Hook CodeIgniter up to MSSQL and read the library hours as an exploratory step in CodeIgniter's adaptability to old database software
1. Maybe create a simple model just for that interaction to keep everything seperate
