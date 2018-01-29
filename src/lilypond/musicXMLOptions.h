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
#include "exports.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class musicXMLOptions : public optionsGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicXMLOptions> create (
      S_optionsHandler optionsHandler);
    
    SMARTP<musicXMLOptions>        createCloneWithDetailedTrace ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    musicXMLOptions (
      S_optionsHandler optionsHandler);
  
    virtual ~musicXMLOptions ();
 
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

    // fields
    // ------------------------------------------------------

    // trace and display
    
    bool                  fTraceMusicXMLTreeVisitors;

    // work

    bool                  fUseFilenameAsWorkTitle;

    // clefs, keys, times

    bool                  fIgnoreRedundantClefs;
    bool                  fIgnoreRedundantKeys;
    bool                  fIgnoreRedundantTimes;

    // tuplets

    // JMI ??? bracket yes/no ???
                      
    // cubase

    bool                  fCubase;

    // to do
    
    bool                  fLoopToMusicXML;
};
typedef SMARTP<musicXMLOptions> S_musicXMLOptions;
EXP ostream& operator<< (ostream& os, const S_musicXMLOptions& elt);

EXP extern S_musicXMLOptions gMusicXMLOptions;
EXP extern S_musicXMLOptions gMusicXMLOptionsUserChoices;
EXP extern S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

//______________________________________________________________________________
void initializeMusicXMLOptionsHandling (
  S_optionsHandler optionsHandler);


}


#endif
