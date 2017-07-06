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

#include "smartpointer.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class EXP msrSingleOption : public smartable
{
/*
struct option {
    const char *name;
    int         has_arg;
    int        *flag;
    int         val;
*/

  // data types

  enum optionArgumentKind {
    kNoArgument = 0, kRequiredArgument = 1, kOptionalArgument = 2 };
    
  public:

    static SMARTP<msrSingleOption> create ();
        
  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);
        
    void                  printMusicXMLOptionsHelp ();

    void                  printMusicXMLOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrSingleOption();
  
    virtual ~msrSingleOption();
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMusicXMLTreeVisitors;

    // other
    // --------------------------------------
  
    bool                  fOptionShortName;
    bool                  fOptionLongName;

    optionArgumentKind    fOptionArgumentKind;

    int&                  fOptionFlag;
    
    int                   fOptionValue;
};
typedef SMARTP<msrSingleOption> S_msrSingleOption;

//______________________________________________________________________________
// MusicXML options
//______________________________________________________________________________

// trace and display
// --------------------------------------

#define _TRACE_MUSICXML_TREE_VISITORS_LONG_NAME_  "traceMusicXMLTreeVisitors"
#define _TRACE_MUSICXML_TREE_VISITORS_SHORT_NAME_ "tmxtvisits"

// other
// --------------------------------------

#define _INGORE_MUSICXML_ERRORS_LONG_NAME_  "ingoreMusicXMLErrors"
#define _INGORE_MUSICXML_ERRORS_SHORT_NAME_ "ime"

#define _LOOP_TO_MUSICXML_LONG_NAME_  "loopToMusicXML"
#define _LOOP_TO_MUSICXML_SHORT_NAME_ "loop"

//______________________________________________________________________________
class EXP msrMusicXMLOptions : public smartable
{
  public:

    static SMARTP<msrMusicXMLOptions> create ();
    
    SMARTP<msrMusicXMLOptions>        createCloneWithDetailedTrace ();
    
  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);
        
    void                  printMusicXMLOptionsHelp ();

    void                  printMusicXMLOptionsValues (int fieldWidth);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrMusicXMLOptions();
  
    virtual ~msrMusicXMLOptions();
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMusicXMLTreeVisitors;

    // other
    // --------------------------------------
  
    bool                  fIgnoreMusicXMLErrors;
    
    bool                  fLoopToMusicXML;
};
typedef SMARTP<msrMusicXMLOptions> S_msrMusicXMLOptions;

extern S_msrMusicXMLOptions gMusicXMLOptions;
extern S_msrMusicXMLOptions gMusicXMLOptionsUserChoices;
extern S_msrMusicXMLOptions gMusicXMLOptionsWithDetailedTrace;


/*! @} */

}


#endif
