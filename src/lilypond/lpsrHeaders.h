/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrHeaders___
#define ___lpsrHeaders___

#include "lpsrVarValAssocs.h"

#include "lilypondOptions.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrHeader : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrHeader> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrHeader (
      int inputLineNumber);
      
    virtual ~lpsrHeader ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // MusicXML informations

    void                  setWorkNumber (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getWorkNumber () const
                              { return fWorkNumber; }
    
    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getWorkTitle () const
                              { return fWorkTitle; }
    
    void                  setMovementNumber (
                            int    inputLineNumber,
                            string vall);

    S_lpsrVarValAssoc     getMovementNumber () const
                              { return fMovementNumber; }
    
    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getMovementTitle () const
                              { return fMovementTitle; }
    
    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getEncodingDate () const
                              { return fEncodingDate; }

    void                  setScoreInstrument (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getScoreInstrument () const
                              { return fScoreInstrument; }

    void                  setMiscellaneousField (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getMiscellaneousField () const
                              { return fMiscellaneousField; }

    void                  setComposers (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getComposers () const
                              { return fComposers; };
    
    void                  setArrangers (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getArrangers () const
                              { return fArrangers; };
    
    void                  setLyricists (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getLyricists () const
                              { return fLyricists; };
    
    void                  setPoets (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getPoets () const
                              { return fPoets; };
    
    void                  setTranslators (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getTranslators () const
                              { return fTranslators; };
    
    void                  setRights (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getRights () const
                              { return fRights; }
    
    void                  setSoftwares (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValsListAssoc
                          getSoftwares () const
                              { return fSoftwares; };
    
    // LilyPond informations

    // centered
    
    void                  setLilypondDedication (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondDedication () const
                              { return fLilypondDedication; }

    void                  setLilypondPiece (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondPiece () const
                              { return fLilypondPiece; }

    void                  setLilypondOpus (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondOpus () const
                              { return fLilypondOpus; }

    void                  setLilypondTitle (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondTitle () const
                              { return fLilypondTitle; }

    void                  setLilypondSubTitle (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondSubTitle () const
                              { return fLilypondSubTitle; }

    void                  setLilypondSubSubTitle (
                            int    inputLineNumber,
                            string val);
    
    S_lpsrVarValAssoc     getLilypondSubSubTitle () const
                              { return fLilypondSubSubTitle; }
                              
    // evenly spread on one line
    // "instrument" also appears on following pages
                                  
    void                  setLilypondInstrument (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondInstrument () const
                              { return fLilypondInstrument; }
    
    // at opposite ends of the same line

    void                  setLilypondMeter (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondMeter () const
                              { return fLilypondMeter; }

    // centered at the bottom of the first page
    
    void                  setLilypondCopyright (
                            int    inputLineNumber,
                            string val);
    
    S_lpsrVarValAssoc     getLilypondCopyright () const
                              { return fLilypondCopyright; }
                              
    // centered at the bottom of the last page
    
    void                  setLilypondTagline (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc     getLilypondTagline () const
                              { return fLilypondTagline; }
                              
  public:

    // public services
    // ------------------------------------------------------

  public:

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

    int                   maxLilypondVariablesNamesLength ();

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

    // MusicXML informations
    
    S_lpsrVarValAssoc     fWorkNumber;
    S_lpsrVarValAssoc     fWorkTitle;
    S_lpsrVarValAssoc     fMovementNumber;
    S_lpsrVarValAssoc     fMovementTitle;
    
    S_lpsrVarValsListAssoc
                          fRights;
                          
    S_lpsrVarValsListAssoc
                          fComposers;
    S_lpsrVarValsListAssoc
                          fArrangers;
    S_lpsrVarValsListAssoc
                          fLyricists;
    S_lpsrVarValsListAssoc
                          fPoets;
    S_lpsrVarValsListAssoc
                          fTranslators;
    
    S_lpsrVarValsListAssoc
                          fSoftwares;
    
    S_lpsrVarValAssoc     fEncodingDate;
    
    S_lpsrVarValAssoc     fScoreInstrument;

    S_lpsrVarValAssoc     fMiscellaneousField;

    // Lilypond informations

    // centered
    S_lpsrVarValAssoc     fLilypondDedication;
    
    S_lpsrVarValAssoc     fLilypondPiece;
    S_lpsrVarValAssoc     fLilypondOpus;
    
    S_lpsrVarValAssoc     fLilypondTitle;
    S_lpsrVarValAssoc     fLilypondSubTitle;
    S_lpsrVarValAssoc     fLilypondSubSubTitle;
    
    // evenly spread on one line
    // "instrument" also appears on following pages
    S_lpsrVarValAssoc     fLilypondInstrument;
    
    // at opposite ends of the same line
    S_lpsrVarValAssoc     fLilypondMeter;
    
    // centered at the bottom of the first page
    S_lpsrVarValAssoc     fLilypondCopyright;
    
    // centered at the bottom of the last page
    S_lpsrVarValAssoc     fLilypondTagline;    
};
typedef SMARTP<lpsrHeader> S_lpsrHeader;
EXP ostream& operator<< (ostream& os, const S_lpsrHeader& elt);


}


#endif
