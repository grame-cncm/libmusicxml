/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrOptionsHandling__
#define __msrOptionsHandling__

#include "list"
#include "map"

#include "smartpointer.h"
#include "rational.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
class EXP xml2lilypondOptionsHandler : public msrOptionsHandler
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lilypondOptionsHandler> create (
      string optionHandlerShortName,
      string optionHandlerLongName,
      string optionHandlerDescription);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lilypondOptionsHandler (
      string optionHandlerShortName,
      string optionHandlerLongName,
      string optionHandlerDescription);
      
    virtual ~msrOptionsHandler();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeOptionsHandler ();

  public:
  
    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:
};
typedef SMARTP<msrOptionsHandler> S_msrOptionsHandler;
ostream& operator<< (ostream& os, const S_msrOptionsHandler& elt);


/*! @} */

}


#endif
