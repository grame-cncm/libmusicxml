/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrPages___
#define ___bsrPages___

#include "bsrPageElements.h"

#include "bsrSpaces.h"

#include "bsrPageHeadings.h"
#include "bsrMusicHeadings.h"

#include "bsrFootNotes.h"

#include "bsrLines.h"

#include "bsrNumbers.h"

#include "bsrClefs.h"
#include "bsrKeys.h"
#include "bsrTimes.h"

#include "bsrNotes.h"

#include "bsrOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrPageElement;
typedef SMARTP<bsrPageElement> S_bsrPageElement;

class bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

//______________________________________________________________________________
class bsrPage : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrPage> create (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);
    
    SMARTP<bsrPage> createPageNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrPage (
      int inputLineNumber,
      int printPageNumber,
      int linesPerPage);
      
    virtual ~bsrPage ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPrintPageNumber () const
                              { return fPrintPageNumber; }

    int                   getBraillePageNumber () const
                              { return fBraillePageNumber; }
                              
    int                   getLinesPerPage () const
                              { return fLinesPerPage; }
                              
  public:

    // public services
    // ------------------------------------------------------

    void                  appendPageHeadingToPage (S_bsrPageHeading pageHeading)
                              { fPageElementsList.push_back (pageHeading); }
                              
    void                  appendMusicHeadingToPage (S_bsrMusicHeading musicHeading)
                              { fPageElementsList.push_back (musicHeading); }
                              
    void                  appendFootNotesToPage (S_bsrFootNotes footNotes)
                              { fPageElementsList.push_back (footNotes); }
                              
    void                  appendLineToPage (S_bsrLine line)
                              { fPageElementsList.push_back (line); }

    int                   fetchLineContentsNumber ();
                              
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

    virtual std::string   asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fPrintPageNumber;    
    int                   fBraillePageNumber;

    int                   fLinesPerPage;    

    list<S_bsrPageElement>
                          fPageElementsList;
};
typedef SMARTP<bsrPage> S_bsrPage;
EXP ostream& operator<< (ostream& os, const S_bsrPage& elt);


}


#endif
