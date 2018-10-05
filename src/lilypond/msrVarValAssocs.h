/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrVarValAssocs___
#define ___msrVarValAssocs___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrVarValAssoc : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrVarValAssocKind {
      kWorkNumber, kWorkTitle,
      kMovementNumber, kMovementTitle,
      kEncodingDate,
      kScoreInstrument,
      kMiscellaneousField };

    static string varValAssocKindAsString (
      msrVarValAssocKind varValAssocKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVarValAssoc> create (
      int                inputLineNumber,
      msrVarValAssocKind varValAssocKind,
      string             value);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVarValAssoc (
      int                inputLineNumber,
      msrVarValAssocKind varValAssocKind,
      string             value);
      
    virtual ~msrVarValAssoc ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrVarValAssocKind    getVarValAssocKind () const
                              { return fVarValAssocKind; }
                              
    void                  setVariableValue (string value);

    string                getVariableValue () const
                              { return fVariableValue; }

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

    string                varValAssocKindAsString () const
                              {
                                return
                                  varValAssocKindAsString (
                                    fVarValAssocKind);
                              }
                              
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrVarValAssocKind    fVarValAssocKind;
    
    string                fVariableValue;
};
typedef SMARTP<msrVarValAssoc> S_msrVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt);

//______________________________________________________________________________
class msrVarValsListAssoc : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrVarValsListAssocKind {
      kRights,
      kComposer, kArranger, kLyricist, kPoet, kTranslator,
      kSoftware };

    static string varValsListAssocKindAsString (
      msrVarValsListAssocKind varValsListAssocKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVarValsListAssoc> create (
      int                     inputLineNumber,
      msrVarValsListAssocKind varValsListAssocKind);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVarValsListAssoc (
      int                     inputLineNumber,
      msrVarValsListAssocKind varValsListAssocKind);
      
    virtual ~msrVarValsListAssoc ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrVarValsListAssocKind
                          getVarValsListAssocKind () const
                              { return fVarValsListAssocKind; }
                              
    const list<string>&   getVariableValuesList ()
                              { return fVariableValuesList; }
    
    // services
    // ------------------------------------------------------

    void                  addAssocVariableValue (string value)
                              {
                                fVariableValuesList.push_back (value);
                              }

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                varValsListAssocKindAsString () const
                              {
                                return
                                  varValsListAssocKindAsString (
                                    fVarValsListAssocKind);
                              }
                              
    string                varValsListAssocValuesAsString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrVarValsListAssocKind
                          fVarValsListAssocKind;
                          
    list<string>          fVariableValuesList;
};
typedef SMARTP<msrVarValsListAssoc> S_msrVarValsListAssoc;
EXP ostream& operator<< (ostream& os, const S_msrVarValsListAssoc& elt);


} // namespace MusicXML2


#endif
