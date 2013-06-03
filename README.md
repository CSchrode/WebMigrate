Outline of the necessary features
=====


Parsing
-----

1. Must be able to get within <div class="rightcontent"> to </div>
1. There may be other <div> tags within that parent containter. Keep these.
1. All links within should point to www.wadsworthlibrary.com - May use wget, study command switches


Implementation
-----

[Simplest Thing That Could Possibly Work](http://c2.com/cgi/wiki?DoTheSimplestThingThatCouldPossiblyWork)

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
