/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <regex>

using namespace std;


//_______________________________________________________________________________
void checkRegex ()
{
  string theString = "4/3";
  
  // theString contains the fraction:
  // decipher it to extract numerator and denominator values
  
  string regularExpression (
    "[[:space:]]*([[:digit:]]+)[[:space:]]*"
    "/"
    "[[:space:]]*([[:digit:]]+)[[:space:]]*");
    
  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  if (sm.size ()) {
    fOptionsHandlerLogIOstream <<
      "There are " << sm.size () << " matches" <<
      " for rational string '" << theString <<
      "' with regex '" << regularExpression <<
      "'" <<
      endl;
 
    for (unsigned i = 0; i < sm.size (); ++i) {
      cout <<
        "[" << sm [i] << "] ";
    } // for
    
    cout <<
      endl;
  }
 
  else {
    stringstream s;

    s <<
      "-delayedOrnamentFraction argument '" << theString <<
      "' is ill-formed";
              
    exit (4);
  }
}

//_______________________________________________________________________________
int main (int argc, char *argv[]) 
{
  checkRegex ();
}
