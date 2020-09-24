/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyRegularOahHandler___
#define ___xml2lyRegularOahHandler___

#include "xml2lyInsiderOahHandler.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2lyRegularOahHandler : public oahHandler
/*
  An xml2ly regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2lyInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyRegularOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2lyRegularOahHandler> createFromInsiderHandler (
      string                    executableName,
      string                    handlerHeader,
      S_xml2lyInsiderOahHandler insiderHandler,
      ostream&                  os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyRegularOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    xml2lyRegularOahHandler (
      string                    executableName,
      string                    handlerHeader,
      S_xml2lyInsiderOahHandler insiderHandler,
      ostream&                  os);

    virtual ~xml2lyRegularOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2lyRegularOahHandler (
                            S_xml2lyInsiderOahHandler insiderHandler);

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments () override;

    // quiet mode
    void                  enforceHandlerQuietness () override;

    // consistency check
    virtual void          checkHandlerOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation
    void                  createRegularHandlerGroups ();

    void                  createOahRegularGroup ();

    void                  createInformationsRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createInputRegularGroup ();

    void                  createPartsRegularGroup ();

    void                  createStavesRegularGroup ();

    void                  createVoicesRegularGroup ();

    void                  createTranspositionsRegularGroup ();

    void                  createClefsRegularGroup ();

    void                  createKeysRegularGroup ();

    void                  createTimesRegularGroup ();

    void                  createRepeatsRegularGroup ();

    void                  createMeasuresRegularGroup ();

    void                  createRestsRegularGroup ();

    void                  createNotesRegularGroup ();

    void                  creatBeamsRegularGroup ();

    void                  createArticulationsRegularGroup ();

    void                  createOrnamentsRegularGroup ();

    void                  createGraceNotesRegularGroup ();

    void                  createChordsRegularGroup ();

    void                  createTiesRegularGroup ();

    void                  createSlursRegularGroup ();

    void                  createLigaturesRegularGroup ();

    void                  createDynamicsRegularGroup ();

    void                  createWedgesRegularGroup ();

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createStringsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();

    void                  createFiguredBassesRegularGroup ();

    void                  createHeaderRegularGroup ();

    void                  createPaperRegularGroup ();

    void                  createLayoutRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  createMidiRegularGroup ();

    void                  populateRegularViewHandlerFromInsiderHandler (
                            S_xml2lyInsiderOahHandler insiderHandler);

    void                  handleSubGroupMapping (S_oahSubGroup subGroup);
    void                  handleAtomMapping (S_oahAtom atom);

    void                  checkMappingsUse ();

  private:

    // fields
    // ------------------------------------------------------

    // the mappings
    map<string, S_oahGroup>
                          fSubGroupNamesToRegularGroupsMap;
    map<string, S_oahSubGroup>
                          fAtomNamesToRegularSubGroupsMap;

    // atoms not present in the 'regular' view are put aside
    S_oahGroup            fPutAsideInRegularViewGroup;
    S_oahSubGroup         fPutAsideInRegularViewSubGroup;
    list<string>          fPutAsideInRegularViewAtomNamesList;
};
typedef SMARTP<xml2lyRegularOahHandler> S_xml2lyRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyRegularOahHandler& elt);

//______________________________________________________________________________
class xml2lyRegularOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2lyRegularOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2lyRegularOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyRegularOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2lyRegularOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    S_oahBooleanAtom      getAutoOutputFileNameAtom () const
                              { return fAutoOutputFileNameAtom; }

    S_oahStringAtom       getOutputFileNameStringAtom () const
                              { return fOutputFileNameStringAtom; }

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printXml2lyRegularOahGroupHelp ();

    void                  printXml2lyRegularOahGroupValues (int fieldWidth);

  public:

    // output file
    // --------------------------------------

    // we store the atoms for the needs of checkOptionsAndArguments()
    S_oahBooleanAtom      fAutoOutputFileNameAtom;

    S_oahStringAtom       fOutputFileNameStringAtom;

    // loop back to MusicXML
    // --------------------------------------
    bool                  fLoopBackToMusicXML;

    // exit after some passes
    // --------------------------------------

    bool                  fExit2a;
    bool                  fExit2b;

  private:

    // LilyPond output file name
    // --------------------------------------

    // these private variables are the ones accessible through the atoms
    bool                  fAutoOutputFileName;

    string                fLilyPondOutputFileName;
};
typedef SMARTP<xml2lyRegularOahGroup> S_xml2lyRegularOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2lyRegularOahGroup& elt);

// JMI EXP extern S_xml2lyRegularOahGroup gGlobalXml2lyRegularOahGroup;

/* JMI
//______________________________________________________________________________
void initializeXml2lyRegularOahHandling (
  S_oahHandler handler);
*/


}


#endif
