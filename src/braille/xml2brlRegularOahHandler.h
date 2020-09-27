/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlRegularOahHandler___
#define ___xml2brlRegularOahHandler___

#include "xml2brlInsiderOahHandler.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2brlRegularOahHandler : public oahHandler
/*
  An xml2brl regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2brlInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlRegularOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2brlRegularOahHandler> createFromInsiderHandler (
      string                     executableName,
      string                     handlerHeader,
      S_xml2brlInsiderOahHandler insiderHandler,
      ostream&                   os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlRegularOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    xml2brlRegularOahHandler (
      string                     executableName,
      string                     handlerHeader,
      S_xml2brlInsiderOahHandler insiderHandler,
      ostream&                   os);

    virtual ~xml2brlRegularOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2brlRegularOahHandler (
                            S_xml2brlInsiderOahHandler insiderHandler);

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

    void                  createPresentationRegularGroup ();

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

    void                  createStringsRegularGroup ();

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();

    void                  createFiguredBassesRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  populateRegularViewHandlerFromInsiderHandler (
                            S_xml2brlInsiderOahHandler insiderHandler);

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
typedef SMARTP<xml2brlRegularOahHandler> S_xml2brlRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlRegularOahHandler& elt);

/* JMI
//______________________________________________________________________________
class xml2brlRegularOahGroup : public oahGroup
{
  public:

    static SMARTP<xml2brlRegularOahGroup> create (
      S_oahHandler handlerUpLink);

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeXml2brlRegularOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlRegularOahGroup (
      S_oahHandler handlerUpLink);

    virtual ~xml2brlRegularOahGroup ();

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

    void                  printXml2brlRegularOahGroupHelp ();

    void                  printXml2brlRegularOahGroupValues (int fieldWidth);

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
typedef SMARTP<xml2brlRegularOahGroup> S_xml2brlRegularOahGroup;
EXP ostream& operator<< (ostream& os, const S_xml2brlRegularOahGroup& elt);

// JMI EXP extern S_xml2brlRegularOahGroup gGlobalXml2brlRegularOahGroup;

//______________________________________________________________________________
void initializeXml2brlRegularOahHandling (
  S_oahHandler handler);
*/


}


#endif
