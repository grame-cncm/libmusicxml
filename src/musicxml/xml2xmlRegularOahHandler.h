/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlRegularOahHandler___
#define ___xml2xmlRegularOahHandler___

#include "xml2xmlInsiderOahHandler.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2xmlRegularOahHandler : public oahHandler
/*
  An xml2xml regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2xmlInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlRegularOahHandler> create (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    static SMARTP<xml2xmlRegularOahHandler> createFromInsiderHandler (
      string                     executableName,
      string                     handlerHeader,
      S_xml2xmlInsiderOahHandler insiderHandler,
      ostream&                   os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlRegularOahHandler (
      string   executableName,
      string   handlerHeader,
      ostream& os);

    xml2xmlRegularOahHandler (
      string                     executableName,
      string                     handlerHeader,
      S_xml2xmlInsiderOahHandler insiderHandler,
      ostream&                   os);

    virtual ~xml2xmlRegularOahHandler ();

  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeXml2xmlRegularOahHandler (
                            S_xml2xmlInsiderOahHandler insiderHandler);

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

    void                  createTupletsRegularGroup ();

    void                  createLyricsRegularGroup ();

    void                  createStringsRegularGroup ();

    void                  createTablaturesRegularGroup ();

    void                  createHarmoniesRegularGroup ();

    void                  createFiguredBassesRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  populateRegularViewHandlerFromInsiderHandler (
                            S_xml2xmlInsiderOahHandler insiderHandler);

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
typedef SMARTP<xml2xmlRegularOahHandler> S_xml2xmlRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2xmlRegularOahHandler& elt);


}


#endif
