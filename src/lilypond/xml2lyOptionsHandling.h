/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lyOptionsHandling__
#define __xml2lyOptionsHandling__

#include "exports.h"
#include "optionsHandling.h"


namespace MusicXML2 
{

//_______________________________________________________________________________
class EXP xml2lyOptionsHandler : public msrOptionsHandler
{
  public:
  
    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOptionsHandler> create (
      indentedOstream& ios);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOptionsHandler (
      indentedOstream& ios);
      
    virtual ~xml2lyOptionsHandler();

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
typedef SMARTP<xml2lyOptionsHandler> S_xml2lyOptionsHandler;
ostream& operator<< (ostream& os, const S_xml2lyOptionsHandler& elt);


}


#endif
