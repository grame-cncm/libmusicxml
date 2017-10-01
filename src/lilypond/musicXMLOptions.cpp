/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, set::precision, ...

#include "version.h"
#include "utilities.h"

#include "musicXMLOptions.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________

S_musicXMLOptions gMusicXMLOptions;
S_musicXMLOptions gMusicXMLOptionsUserChoices;
S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

S_musicXMLOptions musicXMLOptions::create ()
{
  musicXMLOptions* o = new musicXMLOptions();
  assert(o!=0);
  return o;
}

musicXMLOptions::musicXMLOptions ()
  : msrOptionsGroup (
    "MusicXML",
    "hmxml", "helpMusicXML",
R"()"
    )
{
  initializeMusicXMLOptions (false);
}

musicXMLOptions::~musicXMLOptions ()
{}

void musicXMLOptions::initializeMusicXMLOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  // variables
  
  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  // options

  S_msrOptionsSubGroup traceAndDisplaySubGroup =
    msrOptionsSubGroup::create (
      "Trace and display",
      "hmxmltd", "helpMusicXMLTraceAndDisplay",
R"()"
    );

  appendOptionsSubGroup (traceAndDisplaySubGroup);
      
  traceAndDisplaySubGroup->
    appendOptionsItem (
      msrOptionsBooleanItem::create (
        "tmxtvisits", "traceMusicXMLTreeVisitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
        "traceMusicXMLTreeVisitors",
        fTraceMusicXMLTreeVisitors));

  // other
  // --------------------------------------

  // variables
  
  fIgnoreMusicXMLErrors = boolOptionsInitialValue;
  fLoopToMusicXML = boolOptionsInitialValue;

  // options

  S_msrOptionsSubGroup otherSubGroup =
    msrOptionsSubGroup::create (
      "Other",
      "hmxmlo", "helpMusicXMLOther",
R"()"
    );

  appendOptionsSubGroup (otherSubGroup);

  otherSubGroup->
    appendOptionsItem (
      msrOptionsBooleanItem::create (
        "ime", "ignoreMusicXMLErrors",
R"(Don't stop the translation after issuing a MusicXML error message.)",
        "ignoreMusicXMLErrors",
        fIgnoreMusicXMLErrors));

  otherSubGroup->
    appendOptionsItem (
      msrOptionsBooleanItem::create (
        "loop", "loopToMusicXML",
R"(Close the loop, generating a MusicXML file from the MSR. 
The file name receives a '_loop' suffix. Currently under development.)",
        "loopToMusicXML",
        fLoopToMusicXML));
}

S_musicXMLOptions musicXMLOptions::createCloneWithDetailedTrace ()
{
  S_musicXMLOptions
    clone =
      musicXMLOptions::create ();

  // trace and display
  // --------------------------------------

  clone->fTraceMusicXMLTreeVisitors =
    true;

  // other
  // --------------------------------------

  clone->fIgnoreMusicXMLErrors =
    fIgnoreMusicXMLErrors;
    
  clone->fLoopToMusicXML =
    fLoopToMusicXML;

  return clone;
}  

void musicXMLOptions::printMusicXMLOptionsValues (int fieldWidth)
{  
  cerr << idtr <<
    "The MusicXML options are:" <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
      booleanAsString (fTraceMusicXMLTreeVisitors) <<
      endl;

  idtr--;
      
  // other
  // --------------------------------------

  cerr <<
    idtr << "Other:" <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      setw (fieldWidth) << "ignoreMusicXMLErrors" << " : " <<
      booleanAsString (fIgnoreMusicXMLErrors) <<
      endl <<
      
    idtr <<
      setw (fieldWidth) << "loopToMusicXML" << " : " <<
      booleanAsString (fLoopToMusicXML) <<
      endl;

  idtr--;


  idtr--;
}

ostream& operator<< (ostream& os, const S_musicXMLOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicXMLOptions ()
{
  // MusicXML options
  
  gMusicXMLOptionsUserChoices = musicXMLOptions::create ();
  assert(gMusicXMLOptionsUserChoices != 0);

  gMusicXMLOptions =
    gMusicXMLOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gMusicXMLOptionsWithDetailedTrace =
    gMusicXMLOptions->
      createCloneWithDetailedTrace ();
}


}
