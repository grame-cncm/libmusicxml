/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrStaves___
#define ___lpsrStaves___

#include <iostream>

#include "lpsrElements.h"

#include "msr.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrNewStaffgroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffgroupBlock> create (
      int inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffgroupBlock (
      int inputLineNumber);
      
    virtual ~lpsrNewStaffgroupBlock ();
  
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

    vector<S_msrElement> fNewStaffgroupElements;
};
typedef SMARTP<lpsrNewStaffgroupBlock> S_lpsrNewStaffgroupBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupBlock& elt);

//______________________________________________________________________________
class lpsrNewStaffTuningBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffTuningBlock> create (
      int              inputLineNumber,
      S_msrStaffTuning staffTuning);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffTuningBlock (
      int              inputLineNumber,
      S_msrStaffTuning staffTuning);
      
    virtual ~lpsrNewStaffTuningBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaffTuning      getStaffTuning () const
                              { return fStaffTuning; }

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

    S_msrStaffTuning fStaffTuning;
};
typedef SMARTP<lpsrNewStaffTuningBlock> S_lpsrNewStaffTuningBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffTuningBlock& elt);

//______________________________________________________________________________
class lpsrNewStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffBlock> create (
      int inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffBlock (
      int inputLineNumber);
      
    virtual ~lpsrNewStaffBlock ();
  
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

    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewStaffBlock> S_lpsrNewStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffBlock& elt);

//______________________________________________________________________________
class lpsrStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffBlock> create (
      S_msrStaff staff);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrStaffBlock (
      S_msrStaff staff);
      
    virtual ~lpsrStaffBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaff () const
                              { return fStaff; }

    const list<S_msrElement>&
                          getStaffBlockElements () const
                              { return fStaffBlockElements; }

    void                  setStaffBlockInstrumentName (
                            string instrumentName)
                              {
                                fStaffBlockInstrumentName =
                                  instrumentName;
                              }

    string                getStaffBlockInstrumentName () const
                              { return fStaffBlockInstrumentName; }

    void                  setStaffBlockShortInstrumentName (
                            string shortInstrumentName)
                              {
                                fStaffBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    string                getStaffBlockShortInstrumentName () const
                              { return fStaffBlockShortInstrumentName; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceUseToStaffBlock (
                            S_msrVoice voice);

    void                  appendLyricsUseToStaffBlock (
                            S_msrStanza stanza);

    void                  appendElementToStaffBlock (
                            S_msrElement elem)
                              { fStaffBlockElements.push_back (elem); }
                  
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

    S_msrStaff            fStaff;

    list<S_msrElement>    fStaffBlockElements;

    string                fStaffBlockInstrumentName;
    string                fStaffBlockShortInstrumentName;
};
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrStaffBlock& elt);


}


#endif
