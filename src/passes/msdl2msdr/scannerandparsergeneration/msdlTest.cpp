#include <iostream>

#include "msdlDriver.h"

using namespace std;


int
main (int argc, char *argv[])
{
  int result = 0;

  msdlDriver drv;

  if (argc == 1) { // no input file nor options specified
    if (! drv.parse ("-")) { // fake "-" as file name to read from stdin
      cout << drv.getResult () << '\n';
    }
  }

  else {
    // check the arguments
    for (int i = 1; i < argc; ++i) {
      if (argv [i] == string ("-h")) {
        cout <<
          "Usage: " << argv [0] << " ['-p'|'-s'] fileName*" <<
          endl <<
          "  -p: parse" <<
          endl <<
          "  -s: scan" <<
          endl;
      }

      else if (argv [i] == string ("-s"))
        drv.setTraceScanning (true);

      else if (argv [i] == string ("-p"))
        drv.setTraceParsing (true);

      else if (! drv.parse (argv [i]))
        cout << drv.getResult () << '\n';

      else {
        result = 1;
      }
    } // for
  }

  return result;
}
