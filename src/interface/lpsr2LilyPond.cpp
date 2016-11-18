/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>

/*
#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"
*/
#include "versions.h"

#include "lpsr2LilyPond.h"

#include "lpsr2LilyPondVisitor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
void lpsr2LilyPond (
  const S_lpsrScore lpScore,
  S_msrOptions&     msrOpts,
  S_lpsrOptions&    lpsrOpts,
  ostream&          os) 
{  
  // build LPSR score from MSR score
  generateLilyPondCodeFromLpsrScore (
    lpScore, msrOpts, lpsrOpts, os);
}

//_______________________________________________________________________________
void generateLilyPondCodeFromLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOptions&     msrOpts,
  S_lpsrOptions&    lpsrOpts,
  ostream&          os)
{
  // browse the LPSR score with a lpsr2LilyPondVisitor
  if (msrOpts->fTrace)
    cerr << idtr <<
      "Pass 4: generating LilyPond code from the LPSR" << endl;
      // ------------------------------------------------------
  
  // create an lpsr2LilyPondVisitor
  lpsr2LilyPondVisitor visitor (msrOpts, lpsrOpts, os, lpScore);

  string separator =
    "%----------------------------------------";
  
  // build the LPSR score
  if (msrOpts->fTrace) os << separator << endl;
  visitor.generateLilyPondCodeFromLpsrScore ();
  if (msrOpts->fTrace) os << separator << endl;
}


}
