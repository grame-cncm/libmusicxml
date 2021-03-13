/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___Mikrokosmos3WanderingGenerator___
#define ___Mikrokosmos3WanderingGenerator___

#include "smartpointer.h"

#include "msr.h"

#include "multiGenerationOah.h"

#include "msrGenerationBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP Mikrokosmos3WanderingGenerator : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

      static SMARTP<Mikrokosmos3WanderingGenerator> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          Mikrokosmos3WanderingGenerator ();

    virtual               ~Mikrokosmos3WanderingGenerator ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMikrokosmos3WanderingGenerator ();

  public:

    // public services
    // ------------------------------------------------------

    S_msrScore            createAndPopulateTheScore (
                            msrGenerationAPIKind generationAPIKind);

  private:

    // private services
    // ------------------------------------------------------

    // the score
    S_msrScore            createTheScore (
                            msrGenerationAPIKind generationAPIKind);

    // the staves
    S_msrStaff            createStaffInPart (
                            int staffNumber,
                            S_msrPart part);

    S_msrStaff            createAndPopulateLowerStaffInPart (
                            S_msrPart            part,
                            msrGenerationAPIKind generationAPIKind);

    S_msrStaff            createAndPopulateUpperStaffInPart (
                            S_msrPart            part,
                            msrGenerationAPIKind generationAPIKind);

    // the part
    S_msrPart             createPartInScore (S_msrScore theMsrScore);

    // the voices
    S_msrVoice            createRegularVoiceInStaff (
                            int          inputLineNumber,
                            int          voiceNumber,
                            S_msrStaff   staff);

    // the measures
    S_msrMeasure          createMeasureInVoice (
                            int        inputLineNumber,
                            string     measureNumber,
                            S_msrVoice voice);

    // populating the upper voice
    void                  populateUpperVoice1WithTheFunctionsAPI (
                            S_msrVoice upperVoice1);

    void                  populateUpperVoice1WithTheStringsAPI (
                            S_msrVoice upperVoice1);

    // populating the lower voices
    void                  populateLowerVoice1WithTheFunctionsAPI (
                            S_msrVoice lowerVoice1);

    void                  populateLowerVoice1WithTheStringsAPI (
                            S_msrVoice lowerVoice1);

    void                  populateLowerVoice2WithTheFunctionsAPI (
                            S_msrVoice lowerVoice2);

    void                  populateLowerVoice2WithTheStringsAPI (
                            S_msrVoice lowerVoice2);
};
typedef SMARTP<Mikrokosmos3WanderingGenerator> S_Mikrokosmos3WanderingGenerator;
// JMI EXP ostream& operator<< (ostream& os, const S_Mikrokosmos3WanderingGenerator& elt);


}


#endif
