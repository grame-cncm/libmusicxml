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

#include "mxmlOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________

S_musicXMLOptions gMusicXMLOptions;
S_musicXMLOptions gMusicXMLOptionsUserChoices;
S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

S_musicXMLOptions musicXMLOptions::create (
  S_msrOptionsHandler optionsHandler)
{
  musicXMLOptions* o = new musicXMLOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

musicXMLOptions::musicXMLOptions (
  S_msrOptionsHandler optionsHandler)
  : msrOptionsGroup (
    "MusicXML",
    "hmxml", "helpMusicXML",
R"(These options control the way MusicXML data is translated.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroup (this);
  }

  // initialize it
  initializeMusicXMLOptions (false);
}

musicXMLOptions::~musicXMLOptions ()
{}

void musicXMLOptions::initializeMusicXMLOptions (
  bool boolOptionsInitialValue)
{
  // trace and display
  // --------------------------------------

  {
    // variables
    
    fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup
      traceAndDisplaySubGroup =
        msrOptionsSubGroup::create (
          "Trace and display",
          "hmxmltd", "helpMusicXMLTraceAndDisplay",
R"()",
          msrOptionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (traceAndDisplaySubGroup);
        
    traceAndDisplaySubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "tmxmltv", "traceMusicXMLTreeVisitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
          "traceMusicXMLTreeVisitors",
          fTraceMusicXMLTreeVisitors));
  }
  
  
  // error handling
  // --------------------------------------

  {
    // variables
    
    fIgnoreMusicXMLErrors = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup
      errorHandlingSubGroup =
        msrOptionsSubGroup::create (
          "Error handling",
          "hmxmleh", "helpMusicXMLErrorHandling",
R"()",
          msrOptionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (errorHandlingSubGroup);
  
    errorHandlingSubGroup->
      appendOptionsItem (
        msrOptionsBooleanItem::create (
          "ime", "ignoreMusicXMLErrors",
R"(Don't stop the translation after issuing a MusicXML error message.)",
          "ignoreMusicXMLErrors",
          fIgnoreMusicXMLErrors));
  }

  
  // clefs, keys, times
  // --------------------------------------

  S_msrOptionsBooleanItem
    ignoreRedundantClefsItem,
    ignoreRedundantKeysItem,
    ignoreRedundantTimesItem;
    
  {
    // variables
    
    fIgnoreRedundantClefs = boolOptionsInitialValue;
    fIgnoreRedundantKeys  = boolOptionsInitialValue;
    fIgnoreRedundantTimes = boolOptionsInitialValue;
    
    fLoopToMusicXML = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup
      clefsKeysTimesSubGroup =
        msrOptionsSubGroup::create (
          "Clefs, keys, times",
          "hmxmlckt", "helpMusicXMLClefsKeysTimes",
R"()",
          msrOptionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (clefsKeysTimesSubGroup);
  

    ignoreRedundantClefsItem =
        msrOptionsBooleanItem::create (
          "irc", "ignoreRedundantClefs",
R"(Ignore clefs that are the same as the current one.)",
          "ignoreRedundantClefs",
          fIgnoreRedundantClefs);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantClefsItem);

  
    ignoreRedundantKeysItem =
        msrOptionsBooleanItem::create (
          "irk", "ignoreRedundantKeys",
R"(Ignore keys that are the same as the current one.)",
          "ignoreRedundantKeys",
          fIgnoreRedundantKeys);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantKeysItem);


    ignoreRedundantTimesItem =
        msrOptionsBooleanItem::create (
          "irt", "ignoreRedundantTimes",
R"(Ignore times that are the same as the current one.)",
          "ignoreRedundantTimes",
          fIgnoreRedundantTimes);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantTimesItem);


    // '-loop' is hidden...
    S_msrOptionsBooleanItem
      loopOptionsBooleanItem =
        msrOptionsBooleanItem::create (
          "loop", "loopToMusicXML",
R"(Close the loop, generating a MusicXML file from the MSR. 
The file name receives a '_loop' suffix. Currently under development.)",
          "loopToMusicXML",
          fLoopToMusicXML);
    loopOptionsBooleanItem->
      setOptionsElementIsHidden ();
      
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        loopOptionsBooleanItem);
  }
  

  // combined options
  // --------------------------------------

  {
    // variables
    
    fIgnoreRedundantClefs = boolOptionsInitialValue;
    fIgnoreRedundantKeys  = boolOptionsInitialValue;
    fIgnoreRedundantTimes = boolOptionsInitialValue;
    
    fLoopToMusicXML = boolOptionsInitialValue;
  
    // options
  
    S_msrOptionsSubGroup
      combinedOptionsSubGroup =
        msrOptionsSubGroup::create (
          "Combined options",
          "hmxmlco", "helpMusicXMLCombinedOptions",
R"()",
          msrOptionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (combinedOptionsSubGroup);


    // create a combined item
    S_msrOptionsCombinedItemsItem
      cubaseCombinedItemsItem =
        msrOptionsCombinedItemsItem::create (
          "cubase", "",
R"(Useful settings for MusicXML data exported from Cubase.)");

    combinedOptionsSubGroup->
      appendOptionsItem (
        cubaseCombinedItemsItem);
 
    // populate the combined items
    cubaseCombinedItemsItem->
      appendOptionsItemToCombinedItemsList (
        ignoreRedundantClefsItem);
    cubaseCombinedItemsItem->
      appendOptionsItemToCombinedItemsList (
        ignoreRedundantKeysItem);
    cubaseCombinedItemsItem->
      appendOptionsItemToCombinedItemsList (
        ignoreRedundantTimesItem);
  }
}

S_musicXMLOptions musicXMLOptions::createCloneWithDetailedTrace ()
{
  S_musicXMLOptions
    clone =
      musicXMLOptions::create (0);
      // 0 not to have it inserted twice in teh option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // trace and display
  // --------------------------------------

  clone->fTraceMusicXMLTreeVisitors =
    true;


  // error handling
  // --------------------------------------

  clone->fIgnoreMusicXMLErrors =
    fIgnoreMusicXMLErrors;

    
  // clefs, keys, times
  // --------------------------------------
    
  clone->fIgnoreRedundantClefs =
    fIgnoreRedundantClefs;

  clone->fIgnoreRedundantKeys =
    fIgnoreRedundantKeys;

  clone->fIgnoreRedundantTimes =
    fIgnoreRedundantTimes;

  clone->fLoopToMusicXML =
    fLoopToMusicXML;

  return clone;
}  

void musicXMLOptions::printMusicXMLOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

  // trace and display
  // --------------------------------------
  
  gLogIOstream <<
    "Trace and display:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;
      
  // error handling
  // --------------------------------------

  gLogIOstream <<
    "Other:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "ignoreMusicXMLErrors" << " : " <<
    booleanAsString (fIgnoreMusicXMLErrors) <<
    endl <<
    
  gIndenter--;

  // clefs, keys, times
  // --------------------------------------

  gLogIOstream <<
    "Clefs, keys, times:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
    setw (fieldWidth) << "ignoreRedundantClefs" << " : " <<
    booleanAsString (fIgnoreRedundantClefs) <<
    endl <<
    
    setw (fieldWidth) << "ignoreRedundantKeys" << " : " <<
    booleanAsString (fIgnoreRedundantKeys) <<
    endl <<
    
    setw (fieldWidth) << "ignoreRedundantTimes" << " : " <<
    booleanAsString (fIgnoreRedundantTimes) <<
    endl <<
    
    setw (fieldWidth) << "loopToMusicXML" << " : " <<
    booleanAsString (fLoopToMusicXML) <<
    endl;

  gIndenter--;


  gIndenter--;
}

ostream& operator<< (ostream& os, const S_musicXMLOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicXMLOptionsHandling (
  S_msrOptionsHandler optionsHandler)
{
  // create the MusicXML options
  // ------------------------------------------------------
  
  gMusicXMLOptionsUserChoices = musicXMLOptions::create (
    optionsHandler);
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
