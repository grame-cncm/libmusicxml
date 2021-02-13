#ifndef __msdlDriver__
#define __msdlDriver__

#include <string>
#include <map>

#include "msdlParser.h"

using namespace std;


// Give Flex the prototype of yylex we want ...
#define YY_DECL msdl::parser::symbol_type yylex (msdlDriver & drv)
// ... and declare it for the parser's sake.
YY_DECL;

// Conducting the whole scanning and parsing of MSDL
class msdlDriver
{
  public:

    // constructor
    msdlDriver ();

    // set and get
    void setVariableValue (string variableName, double& value)
      { fVariablesMap [variableName] = value; }

    double getVariableValue (string variableName) const;

    void setResult (double& value)
      { fResult = value; }

    double getResult () const
      { return fResult; }

    void setTraceScanning (bool value)
      { fTraceScanning = value; }

    bool getTraceScanning () const
      { return fTraceScanning; }

    void setTraceParsing (bool value)
      { fTraceParsing = value; }

    bool getTraceParsing () const
      { return fTraceParsing; }

    msdl::location& getLocation ()
      { return fLocation; }

    // Run the parser on file f.  Return 0 on success.
    int parse (const string& f);

  private:

    // The name of the file being parsed.
    string fFile;

    // Whether to generate scanner debug traces.
    bool fTraceScanning;

    // Whether to generate parser debug traces.
    bool fTraceParsing;

    // variables map
    map<string, double> fVariablesMap;

    // The token's location used by the scanner.
    msdl::location fLocation;

    // Handling the scanner.
    void scanBegin ();
    void scanEnd ();

    // result
    double fResult;
};

#endif // ! DRIVER_HH
