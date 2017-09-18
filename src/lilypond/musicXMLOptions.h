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
  // ------------------------------------------------------

  enum msrOptionArgumentKind {
    // mapped to the needs of getopt_long()
    kNoArgument = 0, kRequiredArgument = 1, kOptionalArgument = 2 };
    
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrSingleOption> create (
      string             optionShortName,
      string             optionLongName,
      string             optionHelp,
      msrOptionArgumentKind
                         optionArgumentKind,
      int&               optionFlag,
      int                optionValue);
        
    SMARTP<msrSingleOption> createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeSingleOption
                            (bool boolOptionsInitialValue);
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrSingleOption(
      string             optionShortName,
      string             optionLongName,
      string             optionHelp,
      msrOptionArgumentKind
                         optionArgumentKind,
      int&               optionFlag,
      int                optionValue);
  
    virtual ~msrSingleOption();
 
  public:

    // set and get
    // --------------------------------------
    
    bool                  getOptionFlag () const // JMI ???
                              { return fOptionFlag; }

    void                  setOptionHasBeenSelected (
                            bool value)
                              {
                                fOptionHasBeenSelected = value;
                              }

    bool                  getOptionHasBeenSelected () const
                              { return fOptionHasBeenSelected; }
                              
    // services
    // ------------------------------------------------------

    void                  copyToStructOption (
                            struct option & structOption) const;

    // print
    // ------------------------------------------------------

    void                  printSingleOptionHelp ();

    void                  printSingleOptionValue (int fieldWidth);

  private:
  
    // fields
    // ------------------------------------------------------

    string                fOptionHelp;

    // for 'struct option'
  
    string                fOptionShortName;
    string                fOptionLongName;

    /*
    struct option {
        const char *name;
        int         has_arg;
        int        *flag;
        int         val;
    */
  
    msrOptionArgumentKind fOptionArgumentKind;

    int&                  fOptionFlag;
    
    int                   fOptionValue;

    // has this option been selected?
    
    bool                  fOptionHasBeenSelected;

};
typedef SMARTP<msrSingleOption> S_msrSingleOption;

//______________________________________________________________________________
class EXP msrOptionsGroup : public smartable
{    
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrOptionsGroup> create (
      string optionGroupNName);
        
    SMARTP<msrOptionsGroup> createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeOptionsGroup (
                            bool boolOptionsInitialValue);

  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrOptionsGroup(
      string optionGroupNName);
  
    virtual ~msrOptionsGroup();
 
  public:

    // set and get
    // ------------------------------------------------------

    string                getOptionGroupNName () const
                              { return fOptionGroupNName; }
                              
    // services
    // ------------------------------------------------------

    void                  appendSingleOption (
                            S_msrSingleOption singleOption)
                              {
                                fOptionGroupSingleOptionsList.
                                  push_back (
                                    singleOption);
                              }
                              
    // print
    // ------------------------------------------------------

    void                  printOptionsGroupHelp ();

    void                  printOptionsGroupValue (int fieldWidth);
    
  public:

    // fields
    // ------------------------------------------------------
  
    string                fOptionGroupNName;

    list<S_msrSingleOption>
                          fOptionGroupSingleOptionsList;

};
typedef SMARTP<msrOptionsGroup> S_msrOptionsGroup;

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

    // creation
    // ------------------------------------------------------

    static SMARTP<msrMusicXMLOptions> create ();
    
    SMARTP<msrMusicXMLOptions>        createCloneWithDetailedTrace ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicXMLOptions (
                            bool boolOptionsInitialValue);
        
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrMusicXMLOptions();
  
    virtual ~msrMusicXMLOptions();
 
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
typedef SMARTP<msrMusicXMLOptions> S_msrMusicXMLOptions;

extern S_msrMusicXMLOptions gMusicXMLOptions;
extern S_msrMusicXMLOptions gMusicXMLOptionsUserChoices;
extern S_msrMusicXMLOptions gMusicXMLOptionsWithDetailedTrace;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
void initializeMusicXMLOptions ();

/*! @} */

}


#endif
