/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTreeOah___
#define ___mxmlTreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrDalSegnoAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrDalSegnoAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDalSegnoAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<string, msrDalSegno::msrDalSegnoKind>&
                          stringDalSegnoKindMapVariable);

    virtual ~msrDalSegnoAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<string, msrDalSegno::msrDalSegnoKind>&
                          getStringDalSegnoKindMapVariable ()
                              { return fStringDalSegnoKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<string, msrDalSegno::msrDalSegnoKind>&
                          fStringDalSegnoKindMapVariable;
};
typedef SMARTP<msrDalSegnoAtom> S_msrDalSegnoAtom;
EXP ostream& operator<< (ostream& os, const S_msrDalSegnoAtom& elt);

//______________________________________________________________________________
class msrReplaceClefAtom : public oahValuedAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrReplaceClefAtom> create (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<msrClefKind, msrClefKind>&
                          clefKindClefKindMapVariable);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrReplaceClefAtom (
      string              shortName,
      string              longName,
      string              description,
      string              valueSpecification,
      string              variableName,
      map<msrClefKind, msrClefKind>&
                          clefKindClefKindMapVariable);

    virtual ~msrReplaceClefAtom ();

  public:

    // set and get
    // ------------------------------------------------------

    const map<msrClefKind, msrClefKind>&
                          getClefKindClefKindMapVariable ()
                              { return fClefKindClefKindMapVariable; }

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    void                  handleValue (
                            string   theString,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortNamedOptionString () const;
    string                asActualLongNamedOptionString () const;

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------

    map<msrClefKind, msrClefKind>&
                          fClefKindClefKindMapVariable;
};
typedef SMARTP<msrReplaceClefAtom> S_msrReplaceClefAtom;
EXP ostream& operator<< (ostream& os, const S_msrReplaceClefAtom& elt);

//______________________________________________________________________________
class mxmlTreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlTreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<mxmlTreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxmlTreeOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlTreeOah (
      S_oahHandler handlerUpLink);

    virtual ~mxmlTreeOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllMusicXMLTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMxmlTreeTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMxmlTreeHeaderOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeClefsKeysTimesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeMeasuresOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeWordsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeDynamicsAndWedgesOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeCombinedOptionsOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMxmlTreeLoopOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printMxmlTreeOahHelp ();

    void                  printMxmlTreeOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // header
    // --------------------------------------

    bool                  fUseFilenameAsWorkTitle;

    bool                  fUseLyricistsAsPoets; // lyricist is unknown to LilyPond

    // clefs, keys, times
    // --------------------------------------

    map<msrClefKind, msrClefKind>
                          fReplaceClefMapVariable;

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

    S_oahBooleanAtom      fIgnoreRedundantClefsAtom;
    S_oahBooleanAtom      fIgnoreRedundantKeysAtom;
    S_oahBooleanAtom      fIgnoreRedundantTimesAtom;

    // measures
    // --------------------------------------

    map<string,int>       fAddEmptyMeasuresStringToIntMap;

    // words
    // --------------------------------------

    // convert words to dal segno
    map<string, msrDalSegno::msrDalSegnoKind>
                          fConvertWordsToDalSegno;

    // dynamics and wedges
    // --------------------------------------

    bool                  fAllDynamicsBelow;
    bool                  fAllWedgesBelow;

    // combined options, cubase
    // --------------------------------------

    bool                  fCubase;

    // loop to do JMI
    // --------------------------------------
    bool                  fLoopToMusicXML;

#ifdef TRACE_OAH
    // specific trace JMI move to traceOah
    // --------------------------------------

    // visitors
    bool                  fTraceMusicXMLTreeVisitors;

    // encoding
    bool                  fTraceEncoding;

    // divisions
    bool                  fTraceDivisions;

    // backup
    bool                  fTraceBackup;

    // forward
    bool                  fTraceForward;
#endif
};
typedef SMARTP<mxmlTreeOah> S_mxmlTreeOah;
EXP ostream& operator<< (ostream& os, const S_mxmlTreeOah& elt);

EXP extern S_mxmlTreeOah gMxmlTreeOah;
EXP extern S_mxmlTreeOah gMxmlTreeOahUserChoices;
EXP extern S_mxmlTreeOah gMxmlTreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMxmlTreeOahHandling (
  S_oahHandler handler);


}


#endif
