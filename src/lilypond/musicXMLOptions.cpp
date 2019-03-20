/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "version.h"
#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"


using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________

S_musicXMLOptions gMusicXMLOptions;
S_musicXMLOptions gMusicXMLOptionsUserChoices;
S_musicXMLOptions gMusicXMLOptionsWithDetailedTrace;

S_musicXMLOptions musicXMLOptions::create (
  S_optionsHandler optionsHandler)
{
  musicXMLOptions* o = new musicXMLOptions(
    optionsHandler);
  assert(o!=0);
  return o;
}

musicXMLOptions::musicXMLOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "MusicXML",
    "hmxml", "help-musicxml",
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
  // worktitle

  {
    // variables
    
    fUseFilenameAsWorkTitle = false;
  
    // options
  
    S_optionsSubGroup
      workSubGroup =
        optionsSubGroup::create (
          "Work",
          "hmxmlw", "help-musicxml-work",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (workSubGroup);
        
    workSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "ufawt", "use-filename-as-work-title",
R"(Use the file name as work title if there is none in the MusicXML data.
Standard input (-) becomes 'Standard input' in that case.)",
          "useFilenameAsWorkTitle",
          fUseFilenameAsWorkTitle));
  }
  

  // clefs, keys, times
  // --------------------------------------

  S_optionsBooleanItem
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
  
    S_optionsSubGroup
      clefsKeysTimesSubGroup =
        optionsSubGroup::create (
          "Clefs, keys, times",
          "hmxmlckt", "help-musicxml-clefs-keys-times",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (clefsKeysTimesSubGroup);
  

    ignoreRedundantClefsItem =
        optionsBooleanItem::create (
          "irc", "ignore-redundant-clefs",
R"(Ignore clefs that are the same as the current one.)",
          "ignoreRedundantClefs",
          fIgnoreRedundantClefs);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantClefsItem);

  
    ignoreRedundantKeysItem =
        optionsBooleanItem::create (
          "irk", "ignore-redundant-keys",
R"(Ignore keys that are the same as the current one.)",
          "ignoreRedundantKeys",
          fIgnoreRedundantKeys);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantKeysItem);


    ignoreRedundantTimesItem =
        optionsBooleanItem::create (
          "irt", "ignore-redundant-times",
R"(Ignore times that are the same as the current one.)",
          "ignoreRedundantTimes",
          fIgnoreRedundantTimes);
    clefsKeysTimesSubGroup->
      appendOptionsItem (
        ignoreRedundantTimesItem);


    // '-loop' is hidden...
    S_optionsBooleanItem
      loopOptionsBooleanItem =
        optionsBooleanItem::create (
          "loop", "loop-to-musicxml",
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
    
    fCubase = false;
    fNoCubase = false;
  
    // options
  
    S_optionsSubGroup
      combinedOptionsSubGroup =
        optionsSubGroup::create (
          "Combined options",
          "hmxmlco", "help-musicxml-combined-options",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (combinedOptionsSubGroup);

    // create the 'cubase' combined item
    S_optionsCombinedItemsItem
      cubaseCombinedItemsItem =
        optionsCombinedItemsItem::create (
          "cubase", "",
R"(Useful settings for MusicXML data exported from Cubase.
  This option is set by default, and can be unset by 'noCubase'.)",
          "cubase",
          fCubase);

    combinedOptionsSubGroup->
      appendOptionsItem (
        cubaseCombinedItemsItem);

    // set the '-cubase' option by default
    cubaseCombinedItemsItem->
      setCombinedItemsVariablesValue (true);

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

    // create the '-noCubase' item
    S_optionsBooleanItem
      noCubaseBooleanItem =
        optionsBooleanItem::create (
          "noCubase", "",
R"(Prevents the default 'cubase' option.)",
          "noCubase",
          fNoCubase);

    combinedOptionsSubGroup->
      appendOptionsItem (
        noCubaseBooleanItem);
   }


  // trace
  // --------------------------------------

  {
    // variables
    
#ifdef TRACE_OPTIONS
    fTraceEncoding = boolOptionsInitialValue;
    fTraceDivisions = boolOptionsInitialValue;
#endif

    fTraceMusicXMLTreeVisitors = boolOptionsInitialValue;
  
    // options
  
    S_optionsSubGroup
      specificTraceSubGroup =
        optionsSubGroup::create (
          "Trace and display",
          "hmxmltd", "help-musicxml-trace-and-display",
R"()",
          optionsSubGroup::kAlwaysShowDescription,
          this);
  
    appendOptionsSubGroup (specificTraceSubGroup);
            
#ifdef TRACE_OPTIONS
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tenc", "trace-encoding",
R"(Encoding)",
          "traceEncoding",
          fTraceEncoding,
          gTraceOptions->fTracePasses));
          
    specificTraceSubGroup->
      appendOptionsItem (
        optionsTwoBooleansItem::create (
          "tdivs", "trace-divisions",
R"(Divisions)",
          "traceDivisions",
          fTraceDivisions,
          gTraceOptions->fTracePasses));
#endif
      
    specificTraceSubGroup->
      appendOptionsItem (
        optionsBooleanItem::create (
          "tmxmltv", "trace-musicxml-tree-visitors",
R"(Write a trace of the MusicXML tree visiting activity to standard error.)",
          "traceMusicXMLTreeVisitors",
          fTraceMusicXMLTreeVisitors));
  }
}

S_musicXMLOptions musicXMLOptions::createCloneWithDetailedTrace ()
{
  S_musicXMLOptions
    clone =
      musicXMLOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


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


  // trace
  // --------------------------------------

#ifdef TRACE_OPTIONS
  clone->fTraceEncoding =
    fTraceEncoding;

  clone->fTraceDivisions =
    fTraceDivisions;
#endif

  clone->fTraceMusicXMLTreeVisitors =
    fTraceMusicXMLTreeVisitors;

  return clone;
}  

//______________________________________________________________________________
void musicXMLOptions::setAllMusicXMLTraceOptions (
  bool boolOptionsInitialValue)
{
#ifdef TRACE_OPTIONS
  // specific trace    

    // encoding
    fTraceEncoding = boolOptionsInitialValue;

    // divisions
    fTraceDivisions = boolOptionsInitialValue;
#endif
}

//______________________________________________________________________________
void musicXMLOptions::enforceQuietness ()
{
  fTraceMusicXMLTreeVisitors = false;
}

//______________________________________________________________________________
void musicXMLOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
void musicXMLOptions::printMusicXMLOptionsValues (int fieldWidth)
{  
  gLogIOstream <<
    "The MusicXML options are:" <<
    endl;

  gIndenter++;

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

  // trace
  // --------------------------------------
  
  gLogIOstream <<
    "Trace:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
#ifdef TRACE_OPTIONS
    setw (fieldWidth) << "traceEncoding" << " : " <<
    booleanAsString (fTraceEncoding) <<
    endl <<
      
    setw (fieldWidth) << "traceDivisions" << " : " <<
    booleanAsString (fTraceDivisions) <<
    endl <<
#endif
      
    setw (fieldWidth) << "traceMusicXMLTreeVisitors" << " : " <<
    booleanAsString (fTraceMusicXMLTreeVisitors) <<
    endl;

  gIndenter--;

  gIndenter--;
}

S_optionsItem musicXMLOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;
  
  return result;
}

ostream& operator<< (ostream& os, const S_musicXMLOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeMusicXMLOptionsHandling (
  S_optionsHandler optionsHandler)
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
