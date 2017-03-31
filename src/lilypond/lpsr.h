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
*/

//______________________________________________________________________________
// PRE-declarations for class dependencies

// JMI

/*!
\internal
\brief A macro to simplify output line elements counting
*/
//______________________________________________________________________________
#define olec ioutputLineElementsCounter::gOutputLineElementsCounter

/*!
\internal
\brief A macro to emit warning messages regarding MusicXML data
*/
//______________________________________________________________________________
#define lpsrMusicXMLWarning( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "%{" << \
    endl << \
    "!!! LPSR WARNING !!!, " << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << \
    "%}" << \
    endl << \
    endl; \
}

/*!
\internal
\brief A macro to emit error messages regarding MusicXML data and exit
*/
//______________________________________________________________________________
#define lpsrMusicXMLError( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "%{" << \
    endl << \
    "### LPSR ERROR ###, " << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << \
    "%}" << \
    endl << \
    endl << \
    idtr; \
\
  assert(false); \
}

// chords languages
//______________________________________________________________________________
enum lpsrChordsLanguage {
  k_IgnatzekChords, // LilyPond default
  k_GermanChords, k_SemiGermanChords, k_ItalianChords, k_FrenchChords };
  
string lpsrChordsLanguageAsString (
  lpsrChordsLanguage language);

extern map<string, lpsrChordsLanguage>
  gLpsrChordsLanguagesMap;

string existingLpsrChordsLanguages ();

void initializeLpsrChordsLanguages ();

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

    // initialisation
    // ------------------------------------------------------

    void                          initializeLpsrOptions ();
    
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    lpsrOptions();
    
    virtual ~lpsrOptions();

    // set and get
    // ------------------------------------------------------

    bool                          setLpsrQuartertonesPitchesLanguage (
                                    string language);    
 
    bool                          setLpsrChordsLanguage (
                                    string language);    

    bool                          setAccidentalStyle (
                                    string accidentalStyle);    
 
  public:

    // trace
    bool                          fTraceLPSR;

    // languages
    msrQuatertonesPitchesLanguage fLpsrQuatertonesPitchesLanguage;
    lpsrChordsLanguage            fLpsrChordsLanguage;
    
    // LPSR display
    bool                          fDisplayLPSR;

    // LilyPond code generation
    bool                          fGenerateAbsoluteOctaves;

    bool                          fDontKeepLineBreaks;
    bool                          fKeepStaffSize;

    bool                          fShowAllBarNumbers;
    bool                          fCompressFullBarRests;
    
    bool                          fTupletsOnALine;
    
    bool                          fBreakLinesAtIncompleteRightMeasures;
    
    bool                          fEmptyLineEveryNMeasures;
    int                           fEmptyLineEveryNMeasuresValue;
    
    bool                          fGenerateNumericalTime;
    bool                          fGenerateComments;
    bool                          fGenerateStems;
    bool                          fNoAutoBeaming;
    bool                          fGenerateInputLineNumbers;

    string                        fAccidentalStyle;

    int                           fDelayedOrnamentFractionNumerator;
    int                           fDelayedOrnamentFractionDenominator;

    bool                          fGenerateMasterVoices; // JMI
    
    bool                          fDontGenerateLilyPondLyrics;

    bool                          fDontGenerateLilyPondCode;
    
    bool                          fDontGenerateMidiCommand;

    string                        fMidiTempoDuration;
    int                           fMidiTempoPerSecond;

  private:

    set<string>                   fLilyPondAccidentalStyles;
};
typedef SMARTP<lpsrOptions> S_lpsrOptions;

extern S_lpsrOptions gLpsrOptions;

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
      int            inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    lpsrElement (
      int            inputLineNumber);

    virtual ~lpsrElement();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);
};
typedef SMARTP<lpsrElement> S_lpsrElement;
EXP ostream& operator<< (ostream& os, const S_lpsrElement& elt);

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
template <typename T> class EXP lpsrBrowser : public browser<T> 
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:
    
    lpsrBrowser (basevisitor* v) : fVisitor (v) {}
    
    virtual ~lpsrBrowser() {}

    virtual void set (basevisitor* v) { fVisitor = v; }
    
    virtual void browse (T& t) {
      enter (t);

      t.browseData (fVisitor);
      
      leave (t);
    }
};

/*!
\brief The lpsr parallel music element
*/
//______________________________________________________________________________
class EXP lpsrParallelMusic : public lpsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum lpsrElementsSeparator {
      kEndOfLine, kSpace};

    static string elementsSeparatorAsString (
      lpsrElementsSeparator elementsSeparator);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrParallelMusic> create (
      int                   inputLineNumber,
      lpsrElementsSeparator elementsSeparator);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrParallelMusic (
      int                   inputLineNumber,
      lpsrElementsSeparator elementsSeparato);
      
    virtual ~lpsrParallelMusic();
    
  public:

    // set and get
    // ------------------------------------------------------

    const vector<S_msrElement>&
                  getParallelMusicElements () const
                      { return fParallelMusicElements; }

    // services
    // ------------------------------------------------------

    void          addElementToParallelMusic (S_msrElement elem)
                      { fParallelMusicElements.push_back(elem); }
                    
    S_msrElement  getLastElementOfParallelMusic()
                      { return fParallelMusicElements.back(); }
                    
    void          removeLastElementOfParallelMusic ()
                      { fParallelMusicElements.pop_back(); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    // data types
    // ------------------------------------------------------

    enum lpsrCommentedKind {
      kCommented, kUncommented};

    static string commentedKindAsString (
      lpsrCommentedKind commentedKind);
      
    enum lpsrBackslashKind {
      kWithBackslash, kWithoutBackslash};

    static string backslashKindAsString (
      lpsrBackslashKind backslashKind);
      
    enum lpsrVarValSeparator {
      kSpace, kEqualSign};

    static string varValSeparatorAsString (
      lpsrVarValSeparator varValSeparator);
      
    enum lpsrQuotesKind {
      kQuotesAroundValue, kNoQuotesAroundValue};

    static string quotesKindAsString (
      lpsrQuotesKind quotesKind);
      
    enum lpsrEndlKind {
      kWithEndl, kWithEndlTwice, kWithoutEndl};

    static string endlKindAsString (
      lpsrEndlKind endlKind);
      
    static string const g_VarValAssocNoUnit;
    static string const g_VarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrLilypondVarValAssoc> create (
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

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrLilypondVarValAssoc (
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
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrCommentedKind
              getCommentedKind () const
                  { return fCommentedKind; };

    lpsrBackslashKind
              getBackslashKind () const
                  { return fBackslashKind; }
                  
    string    getVariableName () const
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

    void      changeAssocVariableName (string name)
                  { fVariableName = name; }

    void      changeAssocVariableValue (string value)
                  { fVariableValue = value; }


    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    // data types
    // ------------------------------------------------------

    enum lpsrCommentedKind {
      kCommented, kUncommented};

    static string commentedKindAsString (
      lpsrCommentedKind commentedKind);
      
    enum lpsrEndlKind {
      kWithEndl, kWithEndlTwice, kWithoutEndl};

    static string endlKindAsString (
      lpsrEndlKind endlKind);
      
    static string const g_SchemeVarValAssocNoUnit;
    static string const g_SchemeVarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeVarValAssoc> create (
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrSchemeVarValAssoc (
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
      
    virtual ~lpsrSchemeVarValAssoc();
  
  public:

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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
    
    // data types
    // ------------------------------------------------------

    enum lpsrCommentGapKind {
      kGapAfterwards, kNoGapAfterwards};

    static string commentGapKindAsString (
      lpsrCommentGapKind commentGapKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrComment> create (
      int                inputLineNumber,
      string             contents,
      lpsrCommentGapKind commentGapKind = kNoGapAfterwards);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrComment (
      int                inputLineNumber,
      string             contents,
      lpsrCommentGapKind commentGapKind = kNoGapAfterwards);
      
    virtual ~lpsrComment();
  
  public:

    // set and get
    // ------------------------------------------------------

    string              getContents () const
                            { return fContents; }

    lpsrCommentGapKind  getCommentGapKind  () const
                            { return fCommentGapKind; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    string              fContents;
    lpsrCommentGapKind  fCommentGapKind;
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
      int                    inputLineNumber,
      int                    nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarNumberCheck(
      int                    inputLineNumber,
      int                    nextBarNumber);
      
    virtual ~lpsrBarNumberCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewStaffgroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffgroupBlock> create (
      int            inputLineNumberr);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffgroupBlock (
      int            inputLineNumber);
      
    virtual ~lpsrNewStaffgroupBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void addElementToNewStaffgroupBlock (S_msrElement elem)
        { fNewStaffgroupElements.push_back (elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    vector<S_msrElement> fNewStaffgroupElements;
};
typedef SMARTP<lpsrNewStaffgroupBlock> S_lpsrNewStaffgroupBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupBlock& elt);

/*!
\brief A lpsr new staff tuning representation.

  A new staff tuning is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewStafftuningBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStafftuningBlock> create (
      int              inputLineNumber,
      S_msrStafftuning stafftuning);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStafftuningBlock (
      int              inputLineNumber,
      S_msrStafftuning stafftuning);
      
    virtual ~lpsrNewStafftuningBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStafftuning
                getStafftuning () const
                    { return fStafftuning; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    S_msrStafftuning fStafftuning;
};
typedef SMARTP<lpsrNewStafftuningBlock> S_lpsrNewStafftuningBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStafftuningBlock& elt);

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffBlock> create (
      int            inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffBlock (
      int            inputLineNumber);
      
    virtual ~lpsrNewStaffBlock();
  
  public:

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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewStaffBlock> S_lpsrNewStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffBlock& elt);

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
      int            inputLineNumber,
      S_msrVoice     voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseVoiceCommand (
      int            inputLineNumber,
      S_msrVoice     voice);
      
    virtual ~lpsrUseVoiceCommand();
  
  public:

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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
class EXP lpsrNewLyricsBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewLyricsBlock> create (
      int            inputLineNumber,
      S_msrStanza    stanza,
      S_msrVoice     voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewLyricsBlock (
      int            inputLineNumber,
      S_msrStanza    stanza,
      S_msrVoice );
      
    virtual ~lpsrNewLyricsBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStanza  getStanza () const { return fStanza; }
    S_msrVoice   getVoice  () const { return fVoice; }
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    S_msrStanza fStanza;
    S_msrVoice  fVoice;
};
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& elt);


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
      int            inputLineNumber,
      string         variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVariableUseCommand (
      int            inputLineNumber,
      string         variableName);
      
    virtual ~lpsrVariableUseCommand();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
class EXP lpsrUseLyricsCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseLyricsCommand> create (
      int            inputLineNumber,
      S_msrStanza&   stanza);

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseLyricsCommand (
      int            inputLineNumber,
      S_msrStanza&   stanza);
      
    virtual ~lpsrUseLyricsCommand();
  
  private:
  
    S_msrStanza fStanza;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseLyricsCommand& elt);

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

    // data types
    // ------------------------------------------------------

    enum lpsrContextKind {
      kExistingContext, kNewContext};
    
    static string contextKindAsString (
      lpsrContextKind contextKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
      int             inputLineNumber,
      lpsrContextKind contextKind,
      string          contextType,
      string          contextName);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrContext (
      int             inputLineNumber,
      lpsrContextKind contextKind,
      string          contextType,
      string          contextName);
      
    virtual ~lpsrContext();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrContextKind   getContextKind () const
                          { return fContextKind; }

    string            getContextType () const
                          { return fContextType; }

    string            getContextName () const
                          { return fContextName; }

    // services
    // ------------------------------------------------------

    void addElementToContext (S_msrElement elem)
        { fContextElements.push_back(elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
      int            inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarCommand (
      int            inputLineNumber);
      
    virtual ~lpsrBarCommand();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
};
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrBarCommand& elt);

/*!
\brief A lpsr bar representation.

  A bar is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrMelismaCommand : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrMelismaKind {
      kMelismaStart, kMelismaEnd};

    static string melismaKindAsString (
      lpsrMelismaKind melismaKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrMelismaCommand> create (
      int             inputLineNumber,
      lpsrMelismaKind melismaKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrMelismaCommand (
      int             inputLineNumber,
      lpsrMelismaKind melismaKind);
      
    virtual ~lpsrMelismaCommand();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrMelismaKind
                getMelismaKind () const
                    { return fMelismaKind; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    lpsrMelismaKind fMelismaKind;

};
typedef SMARTP<lpsrMelismaCommand> S_lpsrMelismaCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& elt);

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
      int            inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrHeader (
      int            inputLineNumber);
      
    virtual ~lpsrHeader();
  
  public:

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

    S_lpsrLilypondVarValAssoc
                addCreator (
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
    
    const vector<S_lpsrLilypondVarValAssoc>&
                getCreators () const
                    { return fCreators; };
    
    S_lpsrLilypondVarValAssoc
                getRights () const
                    { return fRights; }
    
    const vector<S_lpsrLilypondVarValAssoc>&
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

    void        changeWorkTitleVariableName (string name);
    void        changeMovementTitleVariableName (string name);
    
    void        changeWorkNumberVariableName (string name);
    void        changeMovementNumberVariableName (string name);
    
    void        changeRightsTitleVariableName (string name);
    
    void        changeCreatorVariableName (string typeName, string newName);

    int         maxLilyPondVariablesNamesLength ();

    // visitors
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
      int           inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPaper (
      int           inputLineNumber);
      
    virtual ~lpsrPaper();
  
  public:

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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);


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
      int            inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrLayout (
      int            inputLineNumber);
      
    virtual ~lpsrLayout();
  
  public:

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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    float                             fStaffSize;
    
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
class EXP lpsrStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffBlock> create (
      S_msrStaff     staff);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrStaffBlock (
      S_msrStaff     staff);
      
    virtual ~lpsrStaffBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff    getStaff () const
                      { return fStaff; }

    const list<S_msrElement>&
                  getStaffBlockElements () const
                      { return fStaffBlockElements; }

    void          setStaffBlockInstrumentName (
                    string instrumentName)
                      {
                        fStaffBlockInstrumentName =
                          instrumentName;
                      }

    string        getStaffBlockInstrumentName () const
                      { return fStaffBlockInstrumentName; }

    void          setStaffBlockShortInstrumentName (
                    string shortInstrumentName)
                      {
                        fStaffBlockShortInstrumentName =
                          shortInstrumentName;
                      }

    string        getStaffBlockShortInstrumentName () const
                      { return fStaffBlockShortInstrumentName; }

    // services
    // ------------------------------------------------------

    void          appendVoiceUseToStaffBlock (
                    S_msrVoice voice);

    void          appendLyricsUseToStaffBlock (
                    S_msrStanza stanza);

    void          appendElementToStaffBlock (
                    S_msrElement elem)
                      { fStaffBlockElements.push_back (elem); }
                  
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrStaff         fStaff;

    list<S_msrElement> fStaffBlockElements;

    string             fStaffBlockInstrumentName;
    string             fStaffBlockShortInstrumentName;
};
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrStaffBlock& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrPartBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartBlock> create (
      S_msrPart      part);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPartBlock (
      S_msrPart      part);
      
    virtual ~lpsrPartBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart     getPart () const
                      { return fPart; }

    const list<S_msrElement>&
                  getPartBlockElements () const
                      { return fPartBlockElements; }

    void          setPartBlockInstrumentName (
                    string instrumentName)
                      {
                        fPartBlockInstrumentName =
                          instrumentName;
                      }

    string        getPartBlockInstrumentName () const
                      { return fPartBlockInstrumentName; }

    void          setPartBlockShortInstrumentName (
                    string shortInstrumentName)
                      {
                        fPartBlockShortInstrumentName =
                          shortInstrumentName;
                      }

    string        getPartBlockShortInstrumentName () const
                      { return fPartBlockShortInstrumentName; }

    // services
    // ------------------------------------------------------

    void          appendElementToPartBlock (
                    S_msrElement elem)
                      { fPartBlockElements.push_back (elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrPart          fPart;

    list<S_msrElement> fPartBlockElements;

    string             fPartBlockInstrumentName; // JMI
    string             fPartBlockShortInstrumentName; // JMI
};
typedef SMARTP<lpsrPartBlock> S_lpsrPartBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrPartBlock& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrPartgroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartgroupBlock> create (
      S_msrPartgroup partgroup);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPartgroupBlock (
      S_msrPartgroup partgroup);
      
    virtual ~lpsrPartgroupBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPartgroup
                  getPartgroup () const
                      { return fPartgroup; }

    const list<S_msrElement>&
                  getPartgroupBlockElements () const
                      { return fPartgroupBlockElements; }

    // services
    // ------------------------------------------------------

    void          appendElementToPartgroupBlock (
                    S_msrElement elem)
                      { fPartgroupBlockElements.push_back (elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrPartgroup     fPartgroup;
    
    list<S_msrElement> fPartgroupBlockElements;
};
typedef SMARTP<lpsrPartgroupBlock> S_lpsrPartgroupBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrPartgroupBlock& elt);

/*!
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScoreBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScoreBlock> create (
      int            inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScoreBlock (
      int            inputLineNumber);
      
    virtual ~lpsrScoreBlock();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusic
                  getScoreBlockParallelMusic () const
                      { return fScoreBlockParallelMusic; }

/*
    const vector<S_msrElement>&
                  getScoreBlockElements () const
                      { return fScoreBlockElements; }
*/
    S_lpsrLayout  getScoreBlockLayout () const
                    { return fScoreBlockLayout; }

    S_msrMidi     getScoreBlockMidi () const
                      { return fScoreBlockMidi; }

    // services
    // ------------------------------------------------------

    void          appendPartgroupBlockToParallelMusic (
                    S_lpsrPartgroupBlock partgroupBlock);

    void          appendVoiceUseToParallelMusic (
                    S_lpsrUseVoiceCommand voiceUse);

    void          appendLyricsUseToParallelMusic (
                    S_lpsrNewLyricsBlock lyricsUse);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_lpsrParallelMusic   fScoreBlockParallelMusic;
    
//    vector<S_msrElement>  fScoreBlockElements;

    S_lpsrLayout          fScoreBlockLayout;
    
    S_msrMidi             fScoreBlockMidi;    
};
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreBlock& elt);

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
      int            inputLineNumber,
      S_msrScore     mScore);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScore (
      int            inputLineNumber,
      S_msrScore     mScore);
      
    virtual ~lpsrScore();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
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

    const list<S_msrElement>&
              getVoicesAndLyricsList () const
                  { return fScoreElements; }

    S_lpsrScoreBlock
              getScoreBlock () const
                  { return fScoreBlock; }

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
                  
    void      appendStanzaToScoreElements (
                S_msrStanza stanza)
                  { fScoreElements.push_back (stanza); }

    void      appendVoiceUseToStoreCommand (
                S_msrVoice voice);

    void      appendLyricsUseToStoreCommand (
                S_msrStanza stanza);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // MSR data
    S_msrScore                fMsrScore;

    // general information
    S_lpsrLilypondVarValAssoc fLilyPondVersion;
    
    S_lpsrComment             fInputSourceNameComment;
    S_lpsrComment             fTranslationDateComment;
    S_lpsrComment             fCommandLineOptionsComment;
    
    S_lpsrSchemeVarValAssoc   fGlobalStaffSizeAssoc;
    
    S_lpsrHeader              fHeader;
    S_lpsrPaper               fPaper;
    S_lpsrLayout              fScoreLayout;

    // to keep the original line breaks
    S_lpsrLilypondVarValAssoc fMyBreakIsBreakAssoc;
    S_lpsrLilypondVarValAssoc fMyBreakIsEmptyAssoc;

    // variables, voices and stanzas
    list<S_msrElement>        fScoreElements;

    // score command
    S_lpsrScoreBlock          fScoreBlock;
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
