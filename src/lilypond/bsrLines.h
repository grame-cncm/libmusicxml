/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrLines___
#define ___bsrLines___

#include "bsrPageElements.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"

#include "bsrLinesDeclarations.h"

#include "bsrOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLine : public bsrPageElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrLine> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrLine (
      int inputLineNumber);
      
    virtual ~bsrLine ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintLineNumber () const
                              { return fPrintLineNumber; }

    int                   getBrailleLineNumber () const
                              { return fBrailleLineNumber; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendClefToLine (S_bsrClef clef)
                              { fLineElementsList.push_back (clef); }
                              
    void                  appendKeyToLine (S_bsrKey key)
                              { fLineElementsList.push_back (key); }
                              
    void                  appendTimeToLine (S_bsrTime time)
                              { fLineElementsList.push_back (time); }
                              
    void                  appendElementToLine (S_bsrLineElement lineElement)
                              { fLineElementsList.push_back (lineElement); }
                              
  public:


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

    int                   fPrintLineNumber;    
    int                   fBrailleLineNumber;

    list<S_bsrLineElement>
                          fLineElementsList;
};
typedef SMARTP<bsrLine> S_bsrLine;
EXP ostream& operator<< (ostream& os, const S_bsrLine& elt);


}


#endif
