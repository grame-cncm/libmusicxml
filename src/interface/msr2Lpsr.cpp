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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "msr2Lpsr.h"

#include "msr2LpsrVisitor.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
/*
 * The method that converts the file contents to LilyPond code
 * and  writes the result to the output stream
*/
S_lpsrScore msr2Lpsr (
  const S_msrScore mScore,
  S_msrOptions&    msrOpts,
  S_lpsrOptions&   lpsrOpts,
  ostream&         os) 
{  
  // build LPSR score from MSR score
  S_lpsrScore
    lpScore =
      buildLpsrScoreFromMsrScore (
        mScore, msrOpts, lpsrOpts, os);

  // display it
  if (lpsrOpts->fDisplayLPSR)
    displayLpsrScore (
      lpScore, msrOpts, lpsrOpts, os);

  return lpScore;
}

//_______________________________________________________________________________
S_lpsrScore buildLpsrScoreFromMsrScore (
  const S_msrScore mScore,
  S_msrOptions&    msrOpts,
  S_lpsrOptions&   lpsrOpts,
  ostream&         os)
{
  if (gGeneralOptions->fTrace) {
    string separator = "%----------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Pass 3: building a LPSR from the MSR" <<
      endl <<
      idtr << separator <<
      endl;
      // ------------------------------------------------------
  }
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      endl;
      // ------------------------------------------------------

  idtr++;
  
  // create an msr2LpsrVisitor
  msr2LpsrVisitor visitor (msrOpts, lpsrOpts, os, mScore);
      
  // build the LPSR score
  visitor.buildLpsrScoreFromMsrScore ();

  idtr--;

  return visitor.getLpsrScore ();
}

//_______________________________________________________________________________
void displayLpsrScore (
  const S_lpsrScore lpScore,
  S_msrOptions&     msrOpts,
  S_lpsrOptions&    lpsrOpts,
  ostream&          os)
{
  string separator = "%----------------------------------------";

  if (gGeneralOptions->fTrace) {
    string separator = "%----------------------------------------";
    
    cerr <<
      endl <<
      idtr << separator <<
      endl <<
      "Optional pass: outputting a view of the LPSR" <<
      endl <<
      idtr << separator <<
      endl;
      // ------------------------------------------------------
  }
  
  if (gGeneralOptions->fTrace) os << "%{" << endl;
  os << lpScore;
  if (gGeneralOptions->fTrace) os << "%}" << endl;
  
  os << separator << endl;
}


}
