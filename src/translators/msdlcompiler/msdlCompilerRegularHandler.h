/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlCompilerRegularHandler___
#define ___msdlCompilerRegularHandler___

#include "oahRegularHandlers.h"

#include "msdlCompilerInsiderHandler.h"

namespace MusicXML2
{
//_______________________________________________________________________________
class EXP msdlCompilerRegularHandler : public oahRegularHandler
/*
  An msdlCompiler regular OAH handler can rely on the existence of an insider handler,
  whose creation initialized gGlobalXml2gmnInsiderOahGroup.
  The latter contains all the options values gathered from the user.
*/
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCompilerRegularHandler> create (
                            string              executableName,
                            string              handlerHeader,
                            S_msdlCompilerInsiderHandler
                                                insiderOahHandler,
                            generatorOutputKind theGeneratorOutputKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerRegularHandler (
                            string              executableName,
                            string              handlerHeader,
                            S_msdlCompilerInsiderHandler
                                                insiderOahHandler,
                            generatorOutputKind theGeneratorOutputKind);

    virtual               ~msdlCompilerRegularHandler ();

  public:

    // set and get
    // ------------------------------------------------------

    // generated output kind
    void                  setGeneratorOutputKind (generatorOutputKind value)
                              { fGeneratorOutputKind = value; }
    generatorOutputKind   getGeneratorOutputKind () const
                              { return fGeneratorOutputKind; }

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArgumentsFromArgcAndArgv () const override;

    string                fetchOutputFileNameFromTheOptions () const override
                              {
                                return
                                  fInsiderHandler->
                                    fetchOutputFileNameFromTheOptions ();
                              }

    // quiet mode
    void                  enforceHandlerQuietness () override
                              {
                                fInsiderHandler->
                                  checkHandlerOptionsConsistency ();
                              }

    // consistency check
    void                  checkHandlerOptionsConsistency () override
                              {
                                fInsiderHandler->
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

    void                  createGenerateCodeRegularGroup ();

    void                  createOutputRegularGroup ();

    void                  createGuidoRegularGroup ();

    void                  createBrailleRegularGroup ();

    void                  createFilesRegularGroup ();

    void                  createOahRegularGroup ();

    void                  createWarningAndErrorsRegularGroup ();

    void                  createPresentationRegularGroup ();

    void                  createPartsRegularGroup ();
    void                  createStavesRegularGroup ();
    void                  createVoicesRegularGroup ();

    void                  createClefsRegularGroup ();
    void                  createKeysRegularGroup ();
    void                  createTimesRegularGroup ();

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

    void                  createHarmoniesRegularGroup ();
    void                  createFiguredBassesRegularGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    // generated output kind
    generatorOutputKind     fGeneratorOutputKind;
};
typedef SMARTP<msdlCompilerRegularHandler> S_msdlCompilerRegularHandler;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerRegularHandler& elt);


}


#endif
