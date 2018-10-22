/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrIdentification___
#define ___msrIdentification___

#include "msrElements.h"

#include "msrBasicTypes.h"

#include "msrVarValAssocs.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrIdentification : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrIdentification> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrIdentification (
      int inputLineNumber);
      
    virtual ~msrIdentification ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setWorkNumber (
                            int    inputLineNumber,
                            string val);

    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    void                  setMovementNumber (
                            int    inputLineNumber,
                            string val);
          
    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);
          
    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    void                  setMiscellaneousField (
                            int    inputLineNumber,
                            string val);

    void                  setScoreInstrument (
                            int    inputLineNumber,
                            string val);

    S_msrVarValsListAssoc getRights () const
                              { return fRights; }
    
    S_msrVarValAssoc      getWorkNumber () const
                              { return fWorkNumber; }
    
    S_msrVarValAssoc      getWorkTitle () const
                              { return fWorkTitle; }
    
    S_msrVarValAssoc      getMovementNumber () const
                              { return fMovementNumber; }
    
    S_msrVarValAssoc      getMovementTitle () const
                              { return fMovementTitle; }
    
    S_msrVarValsListAssoc getComposers () const
                              { return fComposers; };
                    
    S_msrVarValsListAssoc getArrangers () const
                              { return fArrangers; };
                    
    S_msrVarValsListAssoc getLyricists () const
                              { return fLyricists; };
    
    S_msrVarValsListAssoc getPoets () const
                              { return fPoets; };
    
    S_msrVarValsListAssoc getTranslators () const
                              { return fTranslators; };
    
    S_msrVarValsListAssoc getSoftwares () const
                              { return fSoftwares; };
    
    S_msrVarValAssoc      getEncodingDate () const
                              { return fEncodingDate; }
    
    S_msrVarValAssoc      getScoreInstrumentAssoc () const
                              { return fScoreInstrumentAssoc; }

    // services
    // ------------------------------------------------------

    void                  addRights (
                            int    inputLineNumber,
                            string value);

    void                  addComposer (
                            int    inputLineNumber,
                            string value);

    void                  addArranger (
                            int    inputLineNumber,
                            string value);

    void                  addLyricist (
                            int    inputLineNumber,
                            string value);

    void                  addPoet (
                            int    inputLineNumber,
                            string value);

    void                  addTranslator (
                            int    inputLineNumber,
                            string value);

    void                  addSoftware (
                            int    inputLineNumber,
                            string value);

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

    S_msrVarValsListAssoc    fRights;

    S_msrVarValAssoc         fWorkNumber;
    S_msrVarValAssoc         fWorkTitle;
    
    S_msrVarValAssoc         fMovementNumber;
    S_msrVarValAssoc         fMovementTitle;
    
    S_msrVarValsListAssoc    fComposers;
    S_msrVarValsListAssoc    fArrangers;
    S_msrVarValsListAssoc    fLyricists;
    S_msrVarValsListAssoc    fPoets;
    S_msrVarValsListAssoc    fTranslators;
    
    S_msrVarValsListAssoc    fSoftwares;
    
    S_msrVarValAssoc         fEncodingDate;
    
    S_msrVarValAssoc         fScoreInstrumentAssoc;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);


} // namespace MusicXML2


#endif
