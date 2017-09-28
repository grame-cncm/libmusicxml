/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicXMLOptions__
#define __musicXMLOptions__

#include "optionsHandling.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class EXP musicXMLOptions : public msrOptionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicXMLOptions> create ();
    
    SMARTP<musicXMLOptions>        createCloneWithDetailedTrace ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    musicXMLOptions();
  
    virtual ~musicXMLOptions();
 
  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkMusicXMLOptionsConsistency () const;

  public:

    // print
    // ------------------------------------------------------

    void                  printMusicXMLOptionsHelp ();

    void                  printMusicXMLOptionsValues (int fieldWidth);
    
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMusicXMLTreeVisitors;

    // other
    // --------------------------------------
  
    bool                  fIgnoreMusicXMLErrors;
    
    bool                  fLoopToMusicXML;
};
typedef SMARTP<musicXMLOptions> S_musicXMLOptions;
EXP ostream& operator<< (ostream& os, const S_musicXMLOptions& elt);

extern S_musicXMLOptions gMusicXMLOptions;
extern S_musicXMLOptions gMusicXMLOptionsUserChoices;
extern S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
void initializeMusicXMLOptions ();

/*! @} */

}


#endif
