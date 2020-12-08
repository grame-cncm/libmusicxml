/*

  Copyright (C) 2003-2008  Grame
  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

  This file is provided as an example of the MusicXML Library use.
*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include <iostream>

#include "libxml2ly.h"
#include "libxml2brl.h"
#include "libxml2xml.h"

using namespace std;
using namespace MusicXML2;

//_______________________________________________________________________________
int main(int argc, char *argv[])
{
  cout << "libmusicxml version " << musicxmllibVersionStr() << endl;
  cout << "MusicXML to guido converter version " << musicxml2guidoVersionStr() << endl;
  cout << "MusicXML to LilyPond converter version " << musicxml2lilypondVersionStr() << endl;
  cout << "MusicXML to braille converter version " << musicxml2brailleVersionStr() << endl;
  cout << "MusicXML to MusicXML converter version " << musicxml2musicxmlVersionStr() << endl;
  return 0;
}
