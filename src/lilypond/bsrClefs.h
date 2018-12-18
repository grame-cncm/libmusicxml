/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrClefs___
#define ___bsrClefs___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrClef : public bsrLineElement
{
  public:
          
    // data types
    // ------------------------------------------------------

    enum bsrClefKind {
        kClefKindNone,
        
        kClefKindGTreble,
        kClefKindFBass,
        kClefKindCAlto,
        kClefKindGSoprano, // first line, french violin
        kClefKindFBaritone,
        kClefKindCTenor,
        kClefKindGOttavaAlta,
        kClefKindGOttavaBassa,
        kClefKindModifiedBassForRightHandPart,
        kClefKindModifiedTrebleForLeftHandPart };
        
    static string clefKindAsString (
      bsrClefKind clefKind);
      
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrClef> create (
      int         inputLineNumber,
      bsrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrClef (
      int         inputLineNumber,
      bsrClefKind clefKind);
      
    virtual ~bsrClef ();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
    bsrClefKind           getClefKind () const
                              { return fClefKind; }

  public:

    // public services
    // ------------------------------------------------------
                  
    S_bsrCellsList        fetchCellsList () const
                              { return fClefCellsList; }

    int                   fetchCellsNumber () const;

  private:
  
    // private services
    // ------------------------------------------------------
                                                                      
    S_bsrCellsList        buildCellsList () const;
                  
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrClefKind           fClefKind;

    S_bsrCellsList        fClefCellsList;
};
typedef SMARTP<bsrClef> S_bsrClef;
EXP ostream& operator<< (ostream& os, const S_bsrClef& elt);


} // namespace MusicXML2


#endif
