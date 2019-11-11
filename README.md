pbrt-parser-perf: Performance comparisons of PBRT parsers
=========================================================

This project provides a command line app which parses 
[PBRT v3 files](https://pbrt.org/fileformat-v3.html) using several different
parsing libraries and produces a report about how long each parser took. The
performance report can be formatted as Markdown or CSV.

The parsers currently being tested are:
* [pbrt-parser](https://github.com/ingowald/pbrt-parser)
* [minipbrt](https://github.com/vilya/minipbrt)

See RESULTS.md for one (fairly large) set of results from running this tool.

The author of this tool is also the author of the `minipbrt` library. Every
effort has been made to keep the performance comparisons as fair and unbiased
as possible, but the usage of minipbrt may be better optimised simply due to
better familiarity. Any improvements to the code will be gratefully received!

