/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr__
#define __lpsr__

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"
#include "rational.h"
#include "exports.h"
#include "typedefs.h"

#include "msr.h"

namespace MusicXML2 
{

/*!
\addtogroup lpsr
@{
*/

/*
  The classes in this file implement the
  
              LPSR (LilyPond Semantic Representation)
              
    - the base class is lpsrElement, derived from msrElement
    - the lpsrElement tree can be converted to:
        - a text description with         printLPSR()
        - LilyPond source code with       printLilypondCode()
*/

/*!
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________

/*!
\brief A msr absolute octave representation.
*/
//______________________________________________________________________________
/* JMI
class EXP msrAbsoluteOctave : public msrElement
{
  public:
  
    static SMARTP<msrAbsoluteOctave> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      int                    musicxmlOctave);
    
    msrAbsoluteOctave (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      int                    musicxmlOctave);
      
    virtual ~msrAbsoluteOctave();
    
    msrAbsoluteOctave& operator= (const msrAbsoluteOctave& absOct)
      {
        fMsrOctave = absOct.fMsrOctave;
        return *this;
      }
          
    bool operator!= (const msrAbsoluteOctave& otherAbsOct) const 
      { 
        return fMsrOctave != otherAbsOct.fMsrOctave;
      }
    
    string  absoluteOctaveAsLilypondString ();

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int  fMsrOctave;
};
typedef SMARTP<msrAbsoluteOctave> S_msrAbsoluteOctave;
EXP ostream& operator<< (ostream& os, const S_msrAbsoluteOctave& elt);
*/

/*!
  \brief The LPSR code generation options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/
//______________________________________________________________________________
class EXP lpsrOptions : public smartable {
  public:

    static SMARTP<lpsrOptions> create ();
    
  public:
  
    lpsrOptions();
    virtual ~lpsrOptions();
 
  public:

    // MSR display
    bool                            fDisplayLPSR;

    // LilyPond code generation
    bool                            fGenerateAbsoluteOctaves;

    bool                            fDontKeepLineBreaks;
    bool                            fKeepStaffSize;
    
    bool                            fGenerateNumericalTime;
    bool                            fGenerateComments;
    bool                            fGenerateStems;
    bool                            fGeneratePositions;
    
    bool                            fDontGenerateLilyPondLyrics;

    // LilyPond source code display
    bool                            fDontDisplayLilyPondCode;
};
typedef SMARTP<lpsrOptions> S_lpsrOptions;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP lpsrElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrElement> create (
      S_msrOptions   msrOpts,
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

  protected:
         
    lpsrElement (
      S_msrOptions   msrOpts,
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    virtual ~lpsrElement();

    S_lpsrOptions fLpsrOptions;
};
typedef SMARTP<lpsrElement> S_lpsrElement;
EXP ostream& operator<< (ostream& os, const S_lpsrElement& elt);

/*!
\brief The lpsr parallel music element
*/
//______________________________________________________________________________
class EXP lpsrParallelMusic : public lpsrElement
{
  public:
    
    enum lpsrElementsSeparator { kEndOfLine, kSpace };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrParallelMusic> create (
      S_msrOptions          msrOpts,
      S_lpsrOptions&        lpsrOpts, 
      int                   inputLineNumber,
      lpsrElementsSeparator elementsSeparator);

    // set and get
    // ------------------------------------------------------

    void          addElementToParallelMusic (S_msrElement elem)
                    { fParallelMusicElements.push_back(elem); }
    S_msrElement  getLastElementOfParallelMusic()
                    { return fParallelMusicElements.back(); }
    void          removeLastElementOfParallelMusic ()
                    { fParallelMusicElements.pop_back(); }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrParallelMusic (
      S_msrOptions          msrOpts,
      S_lpsrOptions&        lpsrOpts, 
      int                   inputLineNumber,
      lpsrElementsSeparator elementsSeparato);
      
    virtual ~lpsrParallelMusic();
    
  private:
  
    vector<S_msrElement>  fParallelMusicElements;
    lpsrElementsSeparator fElementsSeparator;

};
typedef SMARTP<lpsrParallelMusic> S_lpsrParallelMusic;
EXP ostream& operator<< (ostream& os, const S_lpsrParallelMusic& elt);

/*!
\brief A lpsr LilyPond variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrLilypondVarValAssoc : public lpsrElement
{
  public:

    enum lpsrCommentedKind   { kCommented, kUncommented };
    enum lpsrBackslashKind   { kWithBackslash, kWithoutBackslash };
    enum lpsrVarValSeparator { kSpace, kEqualSign };
    enum lpsrQuotesKind      { kQuotesAroundValue, kNoQuotesAroundValue };
    enum lpsrEndlKind        { kWithEndl, kWithoutEndl };

    static string const g_VarValAssocNoUnit;
    static string const g_VarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrLilypondVarValAssoc> create (
        S_msrOptions&       msrOpts, 
        S_lpsrOptions&      lpsrOpts, 
        int                 inputLineNumber,
        lpsrCommentedKind   commentedKind,
        lpsrBackslashKind   backslashKind,
        string              variableName,
        lpsrVarValSeparator varValSeparator,
        lpsrQuotesKind      quotesKind,
        string              value, 
        string              unit,
        string              comment,
        lpsrEndlKind        endlKind);

    // set and get
    // ------------------------------------------------------

    void      changeAssocValue (string value)
                  { fVariableValue = value; }

    lpsrCommentedKind
              getCommentedKind () const
                  { return fCommentedKind; };

    lpsrBackslashKind
              getBackslashKind () const
                  { return fBackslashKind; }
                  
    string    getVariableName  () const
                  { return fVariableName; };
                  
    lpsrVarValSeparator
              getVarValSeparator () const
                  { return fVarValSeparator; };
    
    lpsrQuotesKind
              getQuotesKind () const
                  { return fQuotesKind; };
                  
    string    getVariableValue () const
                  { return fVariableValue; };
    
    string    getUnit () const
                  { return fUnit; };

    string
              getComment  () const
                  { return fComment; }

    lpsrEndlKind
              getEndlKind () const
                  { return fEndlKind; };

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrLilypondVarValAssoc (
        S_msrOptions&       msrOpts, 
        S_lpsrOptions&      lpsrOpts, 
        int                 inputLineNumber,
        lpsrCommentedKind   commentedKind,
        lpsrBackslashKind   backslashKind,
        string              variableName,
        lpsrVarValSeparator varValSeparator,
        lpsrQuotesKind      quotesKind,
        string              value, 
        string              unit,
        string              comment,
        lpsrEndlKind        endlKind);
      
    virtual ~lpsrLilypondVarValAssoc();
  
  private:

    lpsrCommentedKind   fCommentedKind;
    lpsrBackslashKind   fBackslashKind;
    string              fVariableName;
    lpsrVarValSeparator fVarValSeparator;
    lpsrQuotesKind      fQuotesKind;
    string              fVariableValue;
    string              fUnit;
    string              fComment;
    lpsrEndlKind        fEndlKind;
};
typedef SMARTP<lpsrLilypondVarValAssoc> S_lpsrLilypondVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrLilypondVarValAssoc& elt);

/*!
\brief A lpsr Scheme variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrSchemeVarValAssoc : public lpsrElement
{
  public:

    enum lpsrCommentedKind { kCommented, kUncommented };
    enum lpsrEndlKind      { kWithEndl, kWithoutEndl };

    static string const g_SchemeVarValAssocNoUnit;
    static string const g_SchemeVarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeVarValAssoc> create (
      S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
    
    // set and get
    // ------------------------------------------------------

    void      changeAssocValue (string value)
                  { fVariableValue = value; }

    lpsrCommentedKind
              getCommentedKind () const { return fCommentedKind; };

    string    getVariableName  () const { return fVariableName; };
    string    getVariableValue () const { return fVariableValue; };

    string    getComment  () const
                  { return fComment; }

    lpsrEndlKind
              getEndlKind () const
                  { return fEndlKind; };

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrSchemeVarValAssoc (
      S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
      
    virtual ~lpsrSchemeVarValAssoc();
  
  private:

    lpsrCommentedKind fCommentedKind;
    
    string            fVariableName;
    string            fVariableValue;

    string            fComment;

    lpsrEndlKind      fEndlKind;
    
};
typedef SMARTP<lpsrSchemeVarValAssoc> S_lpsrSchemeVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeVarValAssoc& elt);

/*!
\brief A lpsr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________
class EXP lpsrComment : public lpsrElement
{
  public:
    
    enum lpsrGapKind { kGapAfterwards, kNoGapAfterwards };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrComment> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      string         contents,
      lpsrGapKind    gapKind = kNoGapAfterwards);

    // set and get
    // ------------------------------------------------------

    string       getContents () const { return fContents; }

    lpsrGapKind  getGapKind  () const { return fGapKind; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrComment (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      string         contents,
      lpsrGapKind    gapKind = kNoGapAfterwards);
      
    virtual ~lpsrComment();
  
  private:

    string      fContents;
    lpsrGapKind fGapKind;
};
typedef SMARTP<lpsrComment> S_lpsrComment;
EXP ostream& operator<< (ostream& os, const S_lpsrComment& elt);

/*!
\brief A lpsr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarNumberCheck : public lpsrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarNumberCheck> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrBarNumberCheck(
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
      
    virtual ~lpsrBarNumberCheck();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewStaffgroupCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffgroupCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumberr);
     
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void addElementToNewStaff (S_msrElement elem)
        { fNewStaffgroupElements.push_back(elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrNewStaffgroupCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrNewStaffgroupCommand();
  
  private:
  
    vector<S_msrElement> fNewStaffgroupElements;
};
typedef SMARTP<lpsrNewStaffgroupCommand> S_lpsrNewStaffgroupCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupCommand& elt);

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewStaffCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
     
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void addElementToNewStaff (S_msrElement elem)
        { fNewStaffElements.push_back(elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrNewStaffCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrNewStaffCommand();
  
  private:
  
    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewStaffCommand> S_lpsrNewStaffCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffCommand& elt);

/*!
\brief A voice use representation.

  A voice use is represented by the voice to use
*/
//______________________________________________________________________________
class EXP lpsrUseVoiceCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseVoiceCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrVoice     voice);

    // set and get
    // ------------------------------------------------------

    S_msrVoice   getVoice () const { return fVoice; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrUseVoiceCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrVoice     voice);
      
    virtual ~lpsrUseVoiceCommand();
  
  private:
  
    S_msrVoice fVoice;
};
typedef SMARTP<lpsrUseVoiceCommand> S_lpsrUseVoiceCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseVoiceCommand& elt);

/*!
\brief A lpsr new lyrics representation.

  A new lyrics is represented by the voice name and the part name
*/

//______________________________________________________________________________
class EXP lpsrNewlyricsCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewlyricsCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrLyrics    lyrics,
      S_msrVoice     voice);

    // set and get
    // ------------------------------------------------------

    S_msrLyrics  getLyrics () const { return fLyrics; }
    S_msrVoice   getVoice  () const { return fVoice; }
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrNewlyricsCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrLyrics    lyrics,
      S_msrVoice );
      
    virtual ~lpsrNewlyricsCommand();
  
  private:
  
    S_msrLyrics fLyrics;
    S_msrVoice  fVoice;
};
typedef SMARTP<lpsrNewlyricsCommand> S_lpsrNewlyricsCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrNewlyricsCommand& elt);


/*!
\brief A lpsr variable use representation.

  A variable use is represented by the name of the variable to use
*/
//______________________________________________________________________________
class EXP lpsrVariableUseCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVariableUseCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      string         variableName);

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrVariableUseCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      string         variableName);
      
    virtual ~lpsrVariableUseCommand();
  
  private:
  
    string fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& elt);

/*!
\brief A lyrics use representation.

  A lyrics use is represented by the lyrics to use
*/
//______________________________________________________________________________
/*
class EXP lpsrUseLyricsCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseLyricsCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrLyrics&   lyrics);

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrUseLyricsCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrLyrics&   lyrics);
      
    virtual ~lpsrUseLyricsCommand();
  
  private:
  
    S_msrLyrics fLyrics;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseLyricsCommand& elt);
*/

/*!
\brief A context representation.

  A context is represented by its type, name and contents
  It may be:
    existing (such as an implicit Voice in a Score), \context Voice...
    or a new one, \new Voice...
*/
//______________________________________________________________________________
class EXP lpsrContext : public lpsrElement
{
  public:

    enum lpsrContextKind { kExistingContext, kNewContext };
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
      S_msrOptions&   msrOpts, 
      S_lpsrOptions&  lpsrOpts, 
      int             inputLineNumber,
      lpsrContextKind contextKind,
      string          contextType,
      string          contextName);
    
    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void addElementToContext (S_msrElement elem)
        { fContextElements.push_back(elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrContext (
      S_msrOptions&   msrOpts, 
      S_lpsrOptions&  lpsrOpts, 
      int             inputLineNumber,
      lpsrContextKind contextKind,
      string          contextType,
      string          contextName);
      
    virtual ~lpsrContext();
  
  private:
  
    lpsrContextKind   fContextKind;
    string            fContextType;
    string            fContextName;

    vector<S_msrElement> fContextElements;
};
typedef SMARTP<lpsrContext> S_lpsrContext;
EXP ostream& operator<< (ostream& os, const S_lpsrContext& elt);

/*!
\brief A lpsr bar representation.

  A bar is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrBarCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrBarCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrBarCommand();
  
  private:
};
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrBarCommand& elt);

/*!
\brief A lpsr header representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrHeader : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrHeader> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
    
    // set and get
    // ------------------------------------------------------

    void        setWorkNumber (
                  int    inputLineNumber,
                  string val);

    void        setWorkTitle (
                  int    inputLineNumber,
                  string val);

    void        setMovementNumber (
                  int    inputLineNumber,
                  string vall);

    void        setMovementTitle (
                  int    inputLineNumber,
                  string val);

    void        addCreator (
                  int    inputLineNumber,
                  string type,
                  string val);

    void        setRights (
                  int    inputLineNumber,
                  string val);

    void        addSoftware (
                  int    inputLineNumber,
                  string val);

    void        setEncodingDate (
                  int    inputLineNumber,
                  string val);

    void        setScoreInstrument (
                  int    inputLineNumber,
                  string val);

    S_lpsrLilypondVarValAssoc
                getWorkNumber () const
                    { return fWorkNumber; }
    
    S_lpsrLilypondVarValAssoc
                getWorkTitle () const
                    { return fWorkTitle; }
    
    S_lpsrLilypondVarValAssoc
                getMovementNumber () const
                    { return fMovementNumber; }
    
    S_lpsrLilypondVarValAssoc
                getMovementTitle () const
                    { return fMovementTitle; }
    
    vector<S_lpsrLilypondVarValAssoc>
                getCreators () const
                    { return fCreators; };
    
    S_lpsrLilypondVarValAssoc
                getRights () const
                    { return fRights; }
    
    vector<S_lpsrLilypondVarValAssoc>
                getSoftwares () const
                    { return fSoftwares; };
    
    S_lpsrLilypondVarValAssoc
                getEncodingDate () const
                    { return fEncodingDate; }
    
    S_lpsrLilypondVarValAssoc
                getScoreInstrument () const
                    { return fScoreInstrument; }

    // services
    // ------------------------------------------------------

    int         maxLilyPondVariablesNamesLength ();

    // visitors
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrHeader (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrHeader();
  
  private:

    S_lpsrLilypondVarValAssoc         fWorkNumber;
    S_lpsrLilypondVarValAssoc         fWorkTitle;
    S_lpsrLilypondVarValAssoc         fMovementNumber;
    S_lpsrLilypondVarValAssoc         fMovementTitle;
    vector<S_lpsrLilypondVarValAssoc> fCreators;
    S_lpsrLilypondVarValAssoc         fRights;
    vector<S_lpsrLilypondVarValAssoc> fSoftwares;
    S_lpsrLilypondVarValAssoc         fEncodingDate;
    S_lpsrLilypondVarValAssoc         fScoreInstrument;

};
typedef SMARTP<lpsrHeader> S_lpsrHeader;
EXP ostream& operator<< (ostream& os, const S_lpsrHeader& elt);

/*!
\brief A msr paper representation.

  A paper is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrPaper : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPaper> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
    // set and get
    // ------------------------------------------------------

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }


    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);


  protected:

    lpsrPaper (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~lpsrPaper();
  
  private:

    // page height, margins and the like in centimeters are in centimeters
    float             fPaperWidth;
    float             fPaperHeight;
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;
    
    float             fBetweenSystemSpace;
    float             fPageTopSpace; 
};
typedef SMARTP<lpsrPaper> S_lpsrPaper;
EXP ostream& operator<< (ostream& os, const S_lpsrPaper& elt);

/*!
\brief A LPSR layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrLayout : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrLayout> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    // set and get
    // ------------------------------------------------------

    float   getStaffSize () const
                { return fStaffSize; }
                
    void    setGlobalStaffSize (float size)
                { fStaffSize = size; }

    // services
    // ------------------------------------------------------

    void    addLilypondVarValAssoc (
              S_lpsrLilypondVarValAssoc assoc)
                { flpsrLilypondVarValAssocs.push_back (assoc); }
      
    void    addSchemeVarValAssoc (
              S_lpsrSchemeVarValAssoc assoc)
                { fLpsrSchemeVarValAssocs.push_back (assoc); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrLayout (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrLayout();
  
  private:
  
    float                           fStaffSize;
    
    vector<S_lpsrLilypondVarValAssoc> flpsrLilypondVarValAssocs;
    vector<S_lpsrSchemeVarValAssoc>   fLpsrSchemeVarValAssocs;
};
typedef SMARTP<lpsrLayout> S_lpsrLayout;
EXP ostream& operator<< (ostream& os, const S_lpsrLayout& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrRepeatalternative : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrRepeatalternative> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts);
     
    // set and get
    // ------------------------------------------------------

    list<S_msrRepeatending>
              getRepeatendings () const
                  { return fRepeatendings; }

    // services
    // ------------------------------------------------------

    void      appendRepeatending (
                S_msrRepeatending repeatending)
                  { fRepeatendings.push_back (repeatending); }
                  
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrRepeatalternative (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts);
      
    virtual ~lpsrRepeatalternative();
  
  private:

    list<S_msrRepeatending> fRepeatendings;
};
typedef SMARTP<lpsrRepeatalternative> S_lpsrRepeatalternative;
EXP ostream& operator<< (ostream& os, const S_lpsrRepeatalternative& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrRepeat : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrRepeat> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts);
     
    // set and get
    // ------------------------------------------------------

    S_lpsrRepeatalternative
              getRepeatalternative () const
                  { return fRepeatalternative; }

    // services
    // ------------------------------------------------------

    void      addRepeatending (
                S_msrRepeatending repeatending)
                  {
                    fRepeatalternative->appendRepeatending (
                      repeatending);
                  }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrRepeat (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts);
      
    virtual ~lpsrRepeat();
  
  private:

    S_msrVoicechunk         fCommonPart;
    S_lpsrRepeatalternative fRepeatalternative;
};
typedef SMARTP<lpsrRepeat> S_lpsrRepeat;
EXP ostream& operator<< (ostream& os, const S_lpsrRepeat& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrStaffCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrStaff     staff);
     
    // set and get
    // ------------------------------------------------------

    S_msrStaff
              getStaff () const
                      { return fStaff; }

    list<S_msrElement>
              getStaffCommandElements () const
                  { return fStaffCommandElements; }

    // services
    // ------------------------------------------------------

    void      appendVoiceUseToStaffCommand (
                S_msrVoice voice);

    void      appendLyricsUseToStaffCommand (
                S_msrLyrics lyrics);

    void      appendElementToStaffCommand (
                S_msrElement elem)
                  { fStaffCommandElements.push_back (elem); }
                  
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrStaffCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrStaff     staff);
      
    virtual ~lpsrStaffCommand();
  
  private:

    S_msrStaff         fStaff;

    list<S_msrElement> fStaffCommandElements;
};
typedef SMARTP<lpsrStaffCommand> S_lpsrStaffCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrStaffCommand& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrPartCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrPart      part);
     
    // set and get
    // ------------------------------------------------------

    S_msrPart     getPart () const
                      { return fPart; }

    list<S_msrElement>
                  getPartCommandElements () const
                      { return fPartCommandElements; }

    // services
    // ------------------------------------------------------

    void          appendElementToPartCommand (
                    S_msrElement elem)
                      { fPartCommandElements.push_back (elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrPartCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrPart      part);
      
    virtual ~lpsrPartCommand();
  
  private:

    S_msrPart          fPart;

    list<S_msrElement> fPartCommandElements;
};
typedef SMARTP<lpsrPartCommand> S_lpsrPartCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrPartCommand& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrPartgroupCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartgroupCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrPartgroup partgroup);
     
    // set and get
    // ------------------------------------------------------

    S_msrPartgroup
                  getPartgroup () const
                      { return fPartgroup; }

    list<S_msrElement>
                  getPartgroupCommandElements () const
                      { return fPartgroupCommandElements; }

    // services
    // ------------------------------------------------------

    void          appendElementToPartgroupCommand (
                    S_msrElement elem)
                      { fPartgroupCommandElements.push_back (elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrPartgroupCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts,
      S_msrPartgroup partgroup);
      
    virtual ~lpsrPartgroupCommand();
  
  private:

    S_msrPartgroup     fPartgroup;
    
    list<S_msrElement> fPartgroupCommandElements;
};
typedef SMARTP<lpsrPartgroupCommand> S_lpsrPartgroupCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrPartgroupCommand& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScoreCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScoreCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
     
    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusic
                  getScoreCommandParallelMusic () const
                      { return fScoreCommandParallelMusic; }

    S_lpsrLayout  getScoreCommandLayout () const
                    { return fScoreCommandLayout; }

    S_msrMidi     getScoreCommandMidi () const
                      { return fScoreCommandMidi; }

    // services
    // ------------------------------------------------------

    void          appendPartgroupCommandToParallelMusic (
                    S_lpsrPartgroupCommand partgroupCommand)
                      {
                        fScoreCommandParallelMusic->
                          addElementToParallelMusic (partgroupCommand);
                      }

    void          appendVoiceUseToParallelMusic (
                    S_lpsrUseVoiceCommand voiceUse)
                      {
                        fScoreCommandParallelMusic->
                          addElementToParallelMusic (voiceUse);
                      }
                  
    void          appendLyricsUseToParallelMusic (
                    S_lpsrNewlyricsCommand lyricsUse)
                      {
                        fScoreCommandParallelMusic->
                          addElementToParallelMusic (lyricsUse);
                      }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrScoreCommand (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
      
    virtual ~lpsrScoreCommand();
  
  private:

    S_lpsrParallelMusic fScoreCommandParallelMusic;
    S_lpsrLayout        fScoreCommandLayout;
    S_msrMidi           fScoreCommandMidi;    
};
typedef SMARTP<lpsrScoreCommand> S_lpsrScoreCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreCommand& elt);

/*!
\brief A lpsr score representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScore : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScore> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrScore     mScore);
     
    // set and get
    // ------------------------------------------------------

 // JMI   void      setScoreCommand (S_lpsrScoreCommand scoreCommand)
       //           { fScoreCommand = scoreCommand; }
                  
    S_lpsrLilypondVarValAssoc
              getLilyPondVersion () const
                  { return fLilyPondVersion; }

    S_lpsrSchemeVarValAssoc
              getGlobalStaffSizeAssoc () const
                  { return fGlobalStaffSizeAssoc; }

    S_lpsrHeader
              getHeader () const
                  { return fHeader; }

    S_lpsrPaper
              getPaper () const
                  { return fPaper; }

    S_lpsrLayout
              getScoreLayout () const
                  { return fScoreLayout; }

    list<S_msrElement>
              getVoicesAndLyricsList () const
                  { return fScoreElements; }

    S_lpsrScoreCommand
              getScoreCommand () const
                  { return fScoreCommand; }

    void      setGlobalStaffSize (float size)
                  {
                    stringstream s;
                    s << size;
                    fGlobalStaffSizeAssoc->
                      changeAssocValue (s.str());
                  }
    
    // services
    // ------------------------------------------------------

    void      appendCommentToScore (
                S_lpsrComment comment)
                  { fScoreElements.push_back (comment); }
                  
    void      appendSchemeVarValAssocToScore (
                S_lpsrSchemeVarValAssoc assoc)
                  { fScoreElements.push_back (assoc); }
                  
    void      prependSchemeVarValAssocToScore (
                S_lpsrSchemeVarValAssoc assoc)
                  { fScoreElements.push_front (assoc); }
                  
    void      appendVoiceToScoreElements (
                S_msrVoice voice)
                  { fScoreElements.push_back (voice); }
                  
    void      appendLyricsToScoreElements (
                S_msrLyrics lyrics)
                  { fScoreElements.push_back (lyrics); }

    void      appendVoiceUseToStoreCommand (
                S_msrVoice voice);

    void      appendLyricsUseToStoreCommand (
                S_msrLyrics lyrics);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrScore (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrScore     mScore);
      
    virtual ~lpsrScore();
  
  private:

    // MSR data
    S_msrScore                fMsrScore;

    // general information
    S_lpsrLilypondVarValAssoc fLilyPondVersion;
    
    S_lpsrComment             fInputSourceNameComment;
    S_lpsrComment             fTranslationDate;
    
    S_lpsrSchemeVarValAssoc   fGlobalStaffSizeAssoc;
    
    S_lpsrHeader              fHeader;
    S_lpsrPaper               fPaper;
    S_lpsrLayout              fScoreLayout;

    // variables, voices and lyrics
    list<S_msrElement>        fScoreElements;

    // score command
    S_lpsrScoreCommand        fScoreCommand;
};
typedef SMARTP<lpsrScore> S_lpsrScore;
EXP ostream& operator<< (ostream& os, const S_lpsrScore& elt);


/*! @} */

}

    // creation from MusicXML
    // ------------------------------------------------------

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------


#endif
