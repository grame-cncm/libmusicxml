/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyTwoViewOahHandler___
#define ___xml2lyTwoViewOahHandler___

// JMI #include "xml2lyOahTypes.h"

#include "oahTwoViewHandlers.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
class EXP xml2lyTwoViewOahHandler : public oahTwoViewHandler
{
  public:

    // creation
    // ------------------------------------------------------
    static SMARTP<xml2lyTwoViewOahHandler> create (
      string   executableName,
      ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyTwoViewOahHandler (
      string   executableName,
      ostream& os);

    virtual ~xml2lyTwoViewOahHandler ();

  protected:

    // initialization
    // ------------------------------------------------------

    void                  createThePrefixes ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  checkOptionsAndArguments ();

    // quiet mode
// JMI
    // consistency check
    void                  enforceTwoViewHandlerQuietness ();

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

    void                  printXml2lyOahHelp ();

    void                  printXml2lyOahValues (int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    // handlers creation
    void                  createTheTwoHandlers (
                            ostream& os) override;

    // alternate handler groups creation
    void                  createAlternateHandlerGroups (
                            ostream& os);

    void                  createOahAlternateGroup (
                            ostream& os);

    void                  createInformationsAlternateGroup (
                            ostream& os);

    void                  createWarningAndErrorsAlternateGroup (
                            ostream& os);

    void                  createFilesAlternateGroup (
                            ostream& os);

    void                  createInputAlternateGroup (
                            ostream& os);

    void                  createPartsAlternateGroup (
                            ostream& os);

    void                  createStavesAlternateGroup (
                            ostream& os);

    void                  createVoicesAlternateGroup (
                            ostream& os);

    void                  createTranspositionsAlternateGroup (
                            ostream& os);

    void                  createClefsAlternateGroup (
                            ostream& os);

    void                  createKeysAlternateGroup (
                            ostream& os);

    void                  createTimesAlternateGroup (
                            ostream& os);

    void                  createRepeatsAlternateGroup (
                            ostream& os);

    void                  createMeasuresAlternateGroup (
                            ostream& os);

    void                  createRestsAlternateGroup (
                            ostream& os);

    void                  createNotesAlternateGroup (
                            ostream& os);

    void                  creatBeamsAlternateGroup (
                            ostream& os);

    void                  createArticulationsAlternateGroup (
                            ostream& os);

    void                  createOrnamentsAlternateGroup (
                            ostream& os);

    void                  createGraceNotesAlternateGroup (
                            ostream& os);

    void                  createChordsAlternateGroup (
                            ostream& os);

    void                  createTiesAlternateGroup (
                            ostream& os);

    void                  createSlursAlternateGroup (
                            ostream& os);

    void                  createLigaturesAlternateGroup (
                            ostream& os);

    void                  createDynamicsAlternateGroup (
                            ostream& os);

    void                  createWedgesAlternateGroup (
                            ostream& os);

    void                  createTupletsAlternateGroup (
                            ostream& os);

    void                  createLyricsAlternateGroup (
                            ostream& os);

    void                  createStringsAlternateGroup (
                            ostream& os);

    void                  createTablaturesAlternateGroup (
                            ostream& os);

    void                  createHarmoniesAlternateGroup (
                            ostream& os);

    void                  createFiguredBassesAlternateGroup (
                            ostream& os);

    void                  createHeaderAlternateGroup (
                            ostream& os);

    void                  createPaperAlternateGroup (
                            ostream& os);

    void                  createLayoutAlternateGroup (
                            ostream& os);

    void                  createOutputAlternateGroup (
                            ostream& os);

    void                  createMidiAlternateGroup (
                            ostream& os);
};
typedef SMARTP<xml2lyTwoViewOahHandler> S_xml2lyTwoViewOahHandler;
EXP ostream& operator<< (ostream& os, const S_xml2lyTwoViewOahHandler& elt);


}


#endif
