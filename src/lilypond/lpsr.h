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

//JMI#include "lpsrUtilities.h"

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

    static SMARTP<lpsrOptions> create();
    
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

    static SMARTP<lpsrElement> create (
      S_msrOptions   msrOpts,
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

  protected:
         
    lpsrElement (
      S_msrOptions   msrOpts,
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

    virtual ~lpsrElement();

  private:

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

    static SMARTP<lpsrParallelMusic> create (
      S_msrOptions          msrOpts,
      S_lpsrOptions&        lpsrOpts, 
      int                   inputLineNumber,
      lpsrElementsSeparator elementsSeparator);

    void          addElementToParallelMusic (S_msrElement elem)
                    { fParallelMusicElements.push_back(elem); }
    S_msrElement  getLastElementOfParallelMusic()
                    { return fParallelMusicElements.back(); }
    void          removeLastElementOfParallelMusic ()
                    { fParallelMusicElements.pop_back(); }

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
class EXP lpsrVarValAssoc : public lpsrElement
{
  public:

    enum lpsrVarValSeparator { kSpace, kEqualSign };
    enum lpsrQuotesKind      { kQuotesAroundValue, kNoQuotesAroundValue };
    enum lpsrCommentedKind   { kCommented, kUncommented };

    static SMARTP<lpsrVarValAssoc> create (
        S_msrOptions&       msrOpts, 
        S_lpsrOptions&      lpsrOpts, 
        int                 inputLineNumber,
        string              variableName,
        string              value, 
        lpsrVarValSeparator varValSeparator,
        lpsrQuotesKind      quotesKind,
        lpsrCommentedKind   commentedKind,
        string              unit = "");

    void    changeAssoc (string value);
    
    string    getVariableName  () const
                  { return fVariableName; };
    string    getVariableValue () const
                  { return fVariableValue; };
    
    lpsrVarValSeparator
              getVarValSeparator () const
                  { return fVarValSeparator; };
    
    lpsrQuotesKind
              getQuotesKind () const
                  { return fQuotesKind; };
    lpsrCommentedKind
              getCommentedKind () const
                  { return fCommentedKind; };
    
    string    getUnit () const
                  { return fUnit; };

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrVarValAssoc (
        S_msrOptions&       msrOpts, 
        S_lpsrOptions&      lpsrOpts, 
        int                 inputLineNumber,
        string              variableName,
        string              value, 
        lpsrVarValSeparator varValSeparator,
        lpsrQuotesKind      quotesKind,
        lpsrCommentedKind   commentedKind,
        string              unit = "");
      
    virtual ~lpsrVarValAssoc();
  
  private:

    string              fVariableName;
    string              fVariableValue;
    
    lpsrVarValSeparator fVarValSeparator;
    
    lpsrQuotesKind      fQuotesKind;
    lpsrCommentedKind   fCommentedKind;
    
    string              fUnit;
};
typedef SMARTP<lpsrVarValAssoc> S_lpsrVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrVarValAssoc& elt);

/*!
\brief A lpsr Scheme variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrSchemeVarValAssoc : public lpsrElement
{
  public:

    enum lpsrCommentedKind { kCommented, kUncommented };

    static SMARTP<lpsrSchemeVarValAssoc> create (
      S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int               inputLineNumber,
      string            variableName,
      string            value, 
      lpsrCommentedKind  commentedKind);
    
    void    changeAssoc (string value);
    
    string getVariableValue () const { return fVariableValue; };

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrSchemeVarValAssoc (
      S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int               inputLineNumber,
      string            variableName,
      string            value, 
      lpsrCommentedKind  commentedKind);
      
    virtual ~lpsrSchemeVarValAssoc();
  
  private:

    string           fVariableName;
    string           fVariableValue;
    lpsrCommentedKind fCommentedKind;
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

    static SMARTP<lpsrComment> create (
      S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 contents,
      lpsrGapKind                gapKind = kNoGapAfterwards);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrComment (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 contents,
      lpsrGapKind                gapKind = kNoGapAfterwards);
      
    virtual ~lpsrComment();
  
  private:

    string fContents;
    lpsrGapKind     fGapKind;
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
    
    static SMARTP<lpsrBarNumberCheck> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

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
class EXP lpsrNewstaffCommand : public lpsrElement
{
  public:

    static SMARTP<lpsrNewstaffCommand> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber);
     
    void addElementToNewStaff (S_msrElement elem)
        { fNewStaffElements.push_back(elem); }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrNewstaffCommand (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber);
      
    virtual ~lpsrNewstaffCommand();
  
  private:
  
    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewstaffCommand> S_lpsrNewstaffCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrNewstaffCommand& elt);

/*!
\brief A lpsr new lyrics representation.

  A new lyrics is represented by the voice name and the part name
*/
//______________________________________________________________________________
class EXP lpsrNewlyricsCommand : public lpsrElement
{
  public:

    static SMARTP<lpsrNewlyricsCommand> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 lyricsName,
      string                 voiceName);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrNewlyricsCommand (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 lyricsName,
      string                 voiceName);
      
    virtual ~lpsrNewlyricsCommand();
  
  private:
  
    string fLyricsName;
    string fVoiceName;
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

    static SMARTP<lpsrVariableUseCommand> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 variableName);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrVariableUseCommand (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 variableName);
      
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
class EXP lpsrUseLyricsCommand : public lpsrElement
{
  public:

    static SMARTP<lpsrUseLyricsCommand> create (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 lyricsName);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    lpsrUseLyricsCommand (
          S_msrOptions&     msrOpts, 
      S_lpsrOptions&    lpsrOpts, 
      int                    inputLineNumber,
      string                 lyricsName);
      
    virtual ~lpsrUseLyricsCommand();
  
  private:
  
    string lyricsName;
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

    enum lpsrContextKind { kExistingContext, kNewContext };
    
    static SMARTP<lpsrContext> create (
      S_msrOptions&   msrOpts, 
      S_lpsrOptions&  lpsrOpts, 
      int             inputLineNumber,
      lpsrContextKind contextKind,
      string          contextType,
      string          contextName);
    
    void addElementToContext (S_msrElement elem)
        { fContextElements.push_back(elem); }

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

    static SMARTP<lpsrBarCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);

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
\brief A lpsr score block representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScoreCommand : public lpsrElement
{
  public:

    static SMARTP<lpsrScoreCommand> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber);
     
    S_lpsrParallelMusic
                getScoreBlockParallelMusic () const
                    { return fScoreBlockParallelMusic; }

    S_msrLayout getScoreBlockLayout () const
                    { return fScoreBlockLayout; }

    S_msrMidi   getScoreBlockMidi () const
                    { return fScoreBlockMidi; }

    void      appendVoiceUseToParallelMusic (S_msrVoice voice)
                  {
                    fScoreBlockParallelMusic.
                      addElementToParallelMusic (voice);
                  }
                  
    void      appendLyricsUseToElementsList (S_msrLyrics lyrics)
                  {
                    fScoreBlockParallelMusic.
                      addElementToParallelMusic (lyrics);
                  }

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

    S_lpsrParallelMusic fScoreBlockParallelMusic;
    S_msrLayout         fScoreBlockLayout;
    S_msrMidi           fScoreBlockMidi;    
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

    static SMARTP<lpsrScore> create (
      S_msrOptions&  msrOpts, 
      S_lpsrOptions& lpsrOpts, 
      int            inputLineNumber,
      S_msrScore     mScore);
     
    S_lpsrVarValAssoc
                getLilyPondVersion () const
                    { return fLilyPondVersion; }

    S_msrHeader getHeader () const
                    { return fHeader; }

    S_msrLayout getLayout () const
                    { return fLayout; }

    list<S_msrElement>
                getVoicesAndLyricsList () const
                    { return fVoicesAndLyricsList; }

    S_lpsrScoreCommand
                getScoreBlockCommand () const
                    { return fScoreCommand; }

    void      appendVoiceToElementsList (S_msrVoice voice)
                  { fVoicesAndLyricsList.push_back (voice); }
                  
    void      appendLyricsToElementsList (S_msrLyrics lyrics)
                  { fVoicesAndLyricsList.push_back (lyrics); }

    void      appendVoiceUseToStoreCommand (S_msrVoice voice)
                  {
                    fScoreCommand.
                      appendVoiceUseToParallelMusic (voice);
                  }

    void      appendLyricsUseToStoreCommand (S_msrLyrics lyrics)
                  {
                    fScoreCommand.
                      appendVoiceUseToParallelMusic (lyrics);
                  }

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
    S_msrScore          fMsrScore;

    // general information
    S_lpsrVarValAssoc   fLilyPondVersion;
    S_msrHeader         fHeader;
    S_msrLayout         fLayout;

    // voices and lyrics
    list<S_msrElement>  fVoicesAndLyricsList;

    // score command
    S_lpsrScoreCommand  fScoreCommand;
};
typedef SMARTP<lpsrScore> S_lpsrScore;
EXP ostream& operator<< (ostream& os, const S_lpsrScore& elt);


/*! @} */

}


#endif
