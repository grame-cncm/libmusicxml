#include "msdlDriver.h"

#include "msdlParser.h"

using namespace std;


msdlDriver::msdlDriver ()
  : fTraceScanning (false),
    fTraceParsing  (false),
    fResult (0.0)
{
  fVariablesMap ["one"] = 1;
  fVariablesMap ["two"] = 2;

  fVariablesMap ["pi"] = 3.14159265358979323846;
  fVariablesMap ["e"]  = 2.71828182845904523536;
}

double msdlDriver::getVariableValue (string variableName) const
{
  double result = 0.0;

  map<string, double>::const_iterator it =
    fVariablesMap.find (variableName);

  if (it == fVariablesMap.end ()) {
    cout <<
      "Sorry, variable \"" << variableName << "\" is unknown" <<
      endl;

    exit (1);
  }
  else {
    result = (*it).second;
  }

  return result;
}

int msdlDriver::parse (const string &f)
{
  fFile = f;

  fLocation.initialize (& fFile);

  scanBegin ();

  msdl::parser parse (*this);

  parse.set_debug_level (fTraceParsing);

  int result = parse ();

  scanEnd ();

  return result;
}
