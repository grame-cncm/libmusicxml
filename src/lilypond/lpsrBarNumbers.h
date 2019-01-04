/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrBarNumbers___
#define ___lpsrBarNumbers___

#include <iostream>

#include "lpsrElements.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrBarNumberCheck : public lpsrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarNumberCheck> create (
      int inputLineNumber,
      int nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarNumberCheck(
      int inputLineNumber,
      int nextBarNumber);
      
    virtual ~lpsrBarNumberCheck ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

//______________________________________________________________________________
class lpsrBarCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarCommand> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarCommand (
      int inputLineNumber);
      
    virtual ~lpsrBarCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrBarCommand& elt);


}


#endif
