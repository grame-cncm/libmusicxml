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

#include "xml2brlInsiderOahHandler.h"


namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2brlRegularOahHandler : public oahRegularOahHandler
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
      string      executableName,
      string      executableAboutInformation,
      string      handlerHeader,
      S_xml2brlInsiderOahHandler
                  insiderOahHandler);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlRegularOahHandler (
      string      executableName,
      string      executableAboutInformation,
      string      handlerHeader,
      S_xml2brlInsiderOahHandler
                  insiderOahHandler);

    virtual ~xml2brlRegularOahHandler ();

  public:

    // public services
    // ------------------------------------------------------

    string                fetchOutputFileNameFromTheOptions () const override
                              {
                                return
                                  fInsiderOahHandler->
                                    fetchOutputFileNameFromTheOptions ();
                              }

    // quiet mode
    void                  enforceHandlerQuietness () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    void                  checkHandlerOptionsConsistency () override
                              {
                                fInsiderOahHandler->
                                  checkHandlerOptionsConsistency ();
                              }

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // regular handler groups creation

    void                  createRegularHandlerGroups () override;

    // the groups themselves

    void                  createInformationsRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createInputRegularGroup ();

    void                  createPresentationRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createTranspositionsRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

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

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2brlRegularOahHandler> S_xml2brlRegularOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2brlRegularOahHandler& elt);


}


#endif
