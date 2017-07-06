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

#include "utilities.h"

#include "musicXMLOptions.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

//_______________________________________________________________________________
S_msrMusicXMLOptions gMusicXMLOptions;
S_msrMusicXMLOptions gMusicXMLOptionsUserChoices;
S_msrMusicXMLOptions gMusicXMLOptionsWithDetailedTrace;

S_msrMusicXMLOptions msrMusicXMLOptions::create ()
{
  msrMusicXMLOptions* o = new msrMusicXMLOptions();
  assert(o!=0);
  return o;
}

msrMusicXMLOptions::msrMusicXMLOptions ()
{
  initializeMusicXMLOptions (false);
}

msrMusicXMLOptions::~msrMusicXMLOptions ()
{}

void msrMusicXMLOptions::initializeMusicXMLOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;

  // other
  // --------------------------------------

  fIgnoreMusicXMLErrors = boolOptionsInitialValue;
  fLoopToMusicXML = boolOptionsInitialValue;
}

S_msrMusicXMLOptions msrMusicXMLOptions::createCloneWithDetailedTrace ()
{
  S_msrMusicXMLOptions
    clone =
      msrMusicXMLOptions::create ();

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

void msrMusicXMLOptions::printMusicXMLOptionsHelp ()
{
  idtr++;

  cerr <<
    idtr << "MusicXML:" <<
    endl <<
    idtr << "--------" <<
    endl <<
    endl;

  idtr++;

  // trace and display
  // --------------------------------------
  
  cerr <<
    idtr << "Trace and display:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_ ", --" _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Write a trace of the MusicXML tree visiting activity to standard error." <<
      endl <<
    endl;

  idtr--;
      
  // other
  // --------------------------------------
  
  cerr <<
    idtr << "Other:" <<
    endl <<
    endl;

  idtr++;

  cerr <<
    idtr <<
      "--" _INGORE_MUSICXML_ERRORS_SHORT_NAME_ ", --" _INGORE_MUSICXML_ERRORS_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Don't stop the translation after issuing a MusicXML error message." <<
      endl <<
    endl <<
      
    idtr <<
      "--" _LOOP_TO_MUSICXML_SHORT_NAME_ ", --" _LOOP_TO_MUSICXML_LONG_NAME_ <<
      endl <<
    idtr << tab << tab << tab <<
      "Close the loop, generating a MusicXML file from the MSR. " << endl <<
    idtr << tab << tab << tab <<
      "The file name receives a '_loop' suffix. Currently under development." <<
      endl <<
    endl;

  idtr--;
  
  idtr--;

  
  idtr--;
}

void msrMusicXMLOptions::printMusicXMLOptionsValues (int fieldWidth)
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
      setw(fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
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
      setw(fieldWidth) << "ignoreMusicXMLErrors" << " : " <<
      booleanAsString (fIgnoreMusicXMLErrors) <<
      endl <<
      
    idtr <<
      setw(fieldWidth) << "loopToMusicXML" << " : " <<
      booleanAsString (fLoopToMusicXML) <<
      endl;

  idtr--;


  idtr--;
}


}
