/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lilypondOptionsHandling__
#define __xml2lilypondOptionsHandling__

#include "optionsHandling.h"

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
      indentedOstream& ios);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lilypondOptionsHandler (
      indentedOstream& ios);
      
    virtual ~xml2lilypondOptionsHandler();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeOptionsHandler ();

  public:
  
    // services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();
    
    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lilypondOptionsHandler> S_xml2lilypondOptionsHandler;
ostream& operator<< (ostream& os, const S_xml2lilypondOptionsHandler& elt);


/*! @} */

}


#endif
