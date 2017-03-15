#include "catch.hpp"
#include "leviathan.h"
#include "Testhelper.h"

// - INI format with # and ; as allowed comment chars
// - inline comments are forbidden
// - blank lines, leading and trailing spaces allowed
// - string values must have ""
// - enum values are allowed (only /[\w_]/)
// - boolean values are `true` and `false`
// - float values may not have leading zeroes, and must have a `.` in them
// - test many erroneous config files, class should (log and exit | self repair)???
// - if no file exists, one is written with defaults.
// - hint: can we use regexp in the algo?

TEST_CASE( "Configuration: read from file" ) {
    const irr::io::path configFileName = "testconfigfile.ini";

    // preconditions: create contents and write to file

    SECTION( "comments are ignored" ) {}
    SECTION( "unneccessary white spaces are ignored" ) {}

    SECTION( "all relevant values can be read" ) {}
    SECTION( "it has default values" ) {}
    SECTION( "it uses some of the values in the graphic engine" ) {}

    // SECTION( "it can read user defined sections" ) {}
}

TEST_CASE( "Configuration: write to file" ) {
    const irr::io::path configFileName = "testconfigfile.ini";

    SECTION( "it writes back all sections and values" ) {}

    // SECTION( "it writes back the user defined sections that were read before" ) {}
}
