/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrUplinks___
#define ___msrUplinks___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


namespace MusicXML2 
{

//______________________________________________________________________________
template <typename T> class msrUplink : public smartable
{
  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrUplink (T value) : fUplinkvalue (value) {};
      
    virtual ~msrUplink () {};
  
  public:

    // set and get
    // ------------------------------------------------------

    T                     getUplinkValue () const
                              { return fUplinkvalue; }
                  
    void                  setUplinkValue (T value)
                              { fUplinkvalue = value; }
                                    
  private:

    // fields
    // ------------------------------------------------------

    T                     fUplinkvalue;
};

} // namespace MusicXML2


#endif
