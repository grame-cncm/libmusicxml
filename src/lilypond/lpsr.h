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


//#include "smartpointer.h"
//#include "rational.h"
#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"

#include "msr.h"

namespace MusicXML2 
{

/*
  The classes in this file implement the
  
              LPSR (LilyPond Score Representation)
*/

//______________________________________________________________________________
class lpsrElement : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrElement> create (
      int inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    lpsrElement (
      int inputLineNumber);

    virtual ~lpsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------


  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
};
typedef SMARTP<lpsrElement> S_lpsrElement;
EXP ostream& operator<< (ostream& os, const S_lpsrElement& elt);

//______________________________________________________________________________
template <typename T> class lpsrBrowser : public browser<T> 
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:
    
    lpsrBrowser (basevisitor* v) : fVisitor (v)
    {}
    
    virtual ~lpsrBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }
    
    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);
                              
                              leave (t);
                            }
};

//______________________________________________________________________________
class lpsrVarValAssoc : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    // data types
    // ------------------------------------------------------

    enum lpsrVarValAssocKind {
      kVersion,
      kWorkNumber, kWorkTitle,
      kMovementNumber, kMovementTitle,
      kEncodingDate,
      kScoreInstrument,
      kMiscellaneousField,
      kMyBreak, kMyPageBreak,
      kGlobal };

    static string lilyPondVarValAssocKindAsString (
      lpsrVarValAssocKind lilyPondVarValAssocKind);

    enum lpsrCommentedKind {
      kCommented, kUncommented};

    static string commentedKindAsString (
      lpsrCommentedKind commentedKind);
      
    enum lpsrBackSlashKind {
      kWithBackSlash, kWithoutBackSlash};

    static string backSlashKindAsString (
      lpsrBackSlashKind backSlashKind);
      
    enum lpsrVarValSeparatorKind {
      kSpace, kEqualSign};

    static string varValSeparatorKindAsString (
      lpsrVarValSeparatorKind varValSeparatorKind);
      
    enum lpsrQuotesKind {
      kQuotesAroundValue, kNoQuotesAroundValue};

    static string quotesKindAsString (
      lpsrQuotesKind quotesKind);
      
    enum lpsrEndlKind {
      kWithEndl, kWithEndlTwice, kWithoutEndl};

    static string endlKindAsString (
      lpsrEndlKind endlKind);
      
    static string const g_LilyPondVarValAssocNoUnit;
    static string const g_LilyPondVarValAssocNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVarValAssoc> create (
      int                     inputLineNumber,
      lpsrCommentedKind       commentedKind,
      lpsrBackSlashKind       backSlashKind,
      lpsrVarValAssocKind
                              lilyPondVarValAssocKind,
      lpsrVarValSeparatorKind varValSeparatorKind,
      lpsrQuotesKind          quotesKind,
      string                  value, 
      string                  unit,
      string                  comment,
      lpsrEndlKind            endlKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVarValAssoc (
      int                     inputLineNumber,
      lpsrCommentedKind       commentedKind,
      lpsrBackSlashKind       backSlashKind,
      lpsrVarValAssocKind
                              lilyPondVarValAssocKind,
      lpsrVarValSeparatorKind varValSeparatorKind,
      lpsrQuotesKind          quotesKind,
      string                  value, 
      string                  unit,
      string                  comment,
      lpsrEndlKind            endlKind);
      
    virtual ~lpsrVarValAssoc ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrCommentedKind     getCommentedKind () const
                              { return fCommentedKind; }

    lpsrBackSlashKind     getBackSlashKind () const
                              { return fBackSlashKind; }
                  
    lpsrVarValAssocKind
                          getLilyPondVarValAssocKind () const
                              { return fLilyPondVarValAssocKind; }
                  
    lpsrVarValSeparatorKind
                          getVarValSeparator () const
                              { return fVarValSeparatorKind; }
    
    lpsrQuotesKind        getQuotesKind () const
                              { return fQuotesKind; }
                  
    void                  setVariableValue (string value)
                              { fVariableValue = value; }

    string                getVariableValue () const
                              { return fVariableValue; }
    
    string                getUnit () const
                              { return fUnit; }

    string                getComment  () const
                              { return fComment; }

    lpsrEndlKind          getEndlKind () const
                              { return fEndlKind; }

  public:

    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
  
  public:
  
    // print
    // ------------------------------------------------------

    string                lilyPondVarValAssocKindAsString ()
                              {
                                return
                                  lilyPondVarValAssocKindAsString (
                                    fLilyPondVarValAssocKind);
                              }

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    lpsrCommentedKind     fCommentedKind;
    lpsrBackSlashKind     fBackSlashKind;
    lpsrVarValAssocKind
                          fLilyPondVarValAssocKind;
    lpsrVarValSeparatorKind
                          fVarValSeparatorKind;
    lpsrQuotesKind        fQuotesKind;
    string                fVariableValue;
    string                fUnit;
    string                fComment;
    lpsrEndlKind          fEndlKind;
};
typedef SMARTP<lpsrVarValAssoc> S_lpsrVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrVarValAssoc& elt);

//______________________________________________________________________________
class lpsrVarValsListAssoc : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrVarValsListAssocKind {
      kRights,
      kComposer, kArranger, kPoet, kLyricist,
      kSoftware };

    static string lilyPondVarValsListAssocValuesAsString (
      lpsrVarValsListAssocKind varValsListAssocKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVarValsListAssoc> create (
      int                      inputLineNumber,
      lpsrVarValsListAssocKind varValsListAssocKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVarValsListAssoc (
      int                      inputLineNumber,
      lpsrVarValsListAssocKind varValsListAssocKind);
      
    virtual ~lpsrVarValsListAssoc ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrVarValsListAssocKind
                          getVarValsListAssocKind () const
                              { return fVarValsListAssocKind; }
                  
    const list<string>&   getVariableValuesList ()
                              { return fVariableValuesList; }
    
    // services
    // ------------------------------------------------------

    void                  addAssocVariableValue (string value)
                              {
                                fVariableValuesList.push_back (value);
                              }

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
  
  public:
  
    // print
    // ------------------------------------------------------

    string                lilyPondVarValsListAssocKindAsString ()
                              {
                                return
                                  lilyPondVarValsListAssocValuesAsString (
                                    fVarValsListAssocKind);
                              }
                                  
    string                lilyPondVarValsListAssocValuesAsString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    lpsrVarValsListAssocKind
                          fVarValsListAssocKind;

    list<string>          fVariableValuesList;
};
typedef SMARTP<lpsrVarValsListAssoc> S_lpsrVarValsListAssoc;
EXP ostream& operator<< (ostream& os, const S_lpsrVarValsListAssoc& elt);

//______________________________________________________________________________
class lpsrSchemeVariable : public lpsrElement
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
      
    static string const g_SchemeVariableNoUnit;
    static string const g_SchemeVariableNoComment;

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeVariable> create (
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrSchemeVariable (
      int               inputLineNumber,
      lpsrCommentedKind commentedKind,
      string            variableName,
      string            value, 
      string            comment,
      lpsrEndlKind      endlKind);
      
    virtual ~lpsrSchemeVariable ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getVariableName  () const { return fVariableName; }

    void                  setVariableValue (string value)
                              { fVariableValue = value; }

    string                getVariableValue () const { return fVariableValue; }

    lpsrCommentedKind     getCommentedKind () const { return fCommentedKind; }

    string                getComment  () const
                              { return fComment; }

    lpsrEndlKind          getEndlKind () const
                              { return fEndlKind; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    lpsrCommentedKind fCommentedKind;
    
    string                fVariableName;
    string                fVariableValue;

    string                fComment;

    lpsrEndlKind          fEndlKind;
    
};
typedef SMARTP<lpsrSchemeVariable> S_lpsrSchemeVariable;
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeVariable& elt);

//______________________________________________________________________________
class lpsrSchemeFunction : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrSchemeFunction> create (
      int    inputLineNumber,
      string functionName,
      string functionDescription,
      string functionCode);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrSchemeFunction (
      int    inputLineNumber,
      string functionName,
      string functionDescription,
      string functionCode);
      
    virtual ~lpsrSchemeFunction ();
  
  public:

    // set and get
    // ------------------------------------------------------
         
    string                getFunctionName () const
                              { return fFunctionName; };
    
    string                getFunctionDescription () const
                              { return fFunctionDescription; };

    string                getFunctionCode () const
                              { return fFunctionCode; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
    
    // fields
    // ------------------------------------------------------

    string            fFunctionName;
    
    string            fFunctionDescription;

    string            fFunctionCode;    
};
typedef SMARTP<lpsrSchemeFunction> S_lpsrSchemeFunction;
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeFunction& elt);

//______________________________________________________________________________
class lpsrComment : public lpsrElement
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
      
    virtual ~lpsrComment ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getContents () const
                              { return fContents; }

    lpsrCommentGapKind    getCommentGapKind  () const
                              { return fCommentGapKind; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string              fContents;
    lpsrCommentGapKind  fCommentGapKind;
};
typedef SMARTP<lpsrComment> S_lpsrComment;
EXP ostream& operator<< (ostream& os, const S_lpsrComment& elt);

//______________________________________________________________________________
class lpsrBarNumberCheck : public lpsrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarNumberCheck> create (
      int inputLineNumber,
      int nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarNumberCheck(
      int inputLineNumber,
      int nextBarNumber);
      
    virtual ~lpsrBarNumberCheck ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

//______________________________________________________________________________
class lpsrNewStaffgroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffgroupBlock> create (
      int inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffgroupBlock (
      int inputLineNumber);
      
    virtual ~lpsrNewStaffgroupBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    vector<S_msrElement> fNewStaffgroupElements;
};
typedef SMARTP<lpsrNewStaffgroupBlock> S_lpsrNewStaffgroupBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffgroupBlock& elt);

//______________________________________________________________________________
class lpsrNewStaffTuningBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffTuningBlock> create (
      int              inputLineNumber,
      S_msrStaffTuning staffTuning);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffTuningBlock (
      int              inputLineNumber,
      S_msrStaffTuning staffTuning);
      
    virtual ~lpsrNewStaffTuningBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaffTuning      getStaffTuning () const
                              { return fStaffTuning; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStaffTuning fStaffTuning;
};
typedef SMARTP<lpsrNewStaffTuningBlock> S_lpsrNewStaffTuningBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffTuningBlock& elt);

//______________________________________________________________________________
class lpsrNewStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewStaffBlock> create (
      int inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewStaffBlock (
      int inputLineNumber);
      
    virtual ~lpsrNewStaffBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    vector<S_msrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewStaffBlock> S_lpsrNewStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewStaffBlock& elt);

//______________________________________________________________________________
class lpsrUseVoiceCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseVoiceCommand> create (
      int        inputLineNumber,
      S_msrVoice voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseVoiceCommand (
      int        inputLineNumber,
      S_msrVoice voice);
      
    virtual ~lpsrUseVoiceCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getVoice () const { return fVoice; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrVoice fVoice;
};
typedef SMARTP<lpsrUseVoiceCommand> S_lpsrUseVoiceCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseVoiceCommand& elt);

//______________________________________________________________________________
class lpsrNewLyricsBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewLyricsBlock> create (
      int         inputLineNumber,
      S_msrStanza stanza,
      S_msrVoice  voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewLyricsBlock (
      int         inputLineNumber,
      S_msrStanza stanza,
      S_msrVoice  voice);
      
    virtual ~lpsrNewLyricsBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStanza           getStanza () const { return fStanza; }
    S_msrVoice            getVoice  () const { return fVoice; }
    
    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& elt);

//______________________________________________________________________________
class lpsrVariableUseCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrVariableUseCommand> create (
      int    inputLineNumber,
      string variableName);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrVariableUseCommand (
      int    inputLineNumber,
      string variableName);
      
    virtual ~lpsrVariableUseCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    string                fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrVariableUseCommand& elt);

/* JMI ???
//______________________________________________________________________________
class lpsrUseLyricsCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseLyricsCommand> create (
      int          inputLineNumber,
      S_msrStanza& stanza);

  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseLyricsCommand (
      int            inputLineNumber,
      S_msrStanza&   stanza);
      
    virtual ~lpsrUseLyricsCommand ();
  
  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseLyricsCommand& elt);
*/

//______________________________________________________________________________
class lpsrContext : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrContextTypeKind {
      kChordNames, kFiguredBass };
    
    static string contextTypeKindAsString (
      lpsrContextTypeKind contextTypeKind);
      
    enum lpsrContextExistingKind {
      kExistingContextYes, kExistingContextNo};
    
    static string contextExistingKindAsString (
      lpsrContextExistingKind contextExistingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      lpsrContextTypeKind     contextTypeKind,
      string                  contextName);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      lpsrContextTypeKind     contextTypeKind,
      string                  contextName);
      
    virtual ~lpsrContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrContextExistingKind
                          getContextExistingKind () const
                              { return fContextExistingKind; }

    lpsrContextTypeKind   getContextTypeKind () const
                              { return fContextTypeKind; }

    string                getContextName () const
                              { return fContextName; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    string                getContextTypeKindAsString () const
                              {
                                return
                                  contextTypeKindAsString (
                                    fContextTypeKind);
                              }

    string                getContextExistingKindAsString () const
                              {
                                return
                                  contextExistingKindAsString (
                                    fContextExistingKind);
                              }

    virtual void          print (ostream& os);

  protected:
  
    // fields
    // ------------------------------------------------------

    lpsrContextExistingKind
                          fContextExistingKind;
    lpsrContextTypeKind   fContextTypeKind;
    string                fContextName;

    list<S_msrElement>    fContextElementsList;
};
typedef SMARTP<lpsrContext> S_lpsrContext;
EXP ostream& operator<< (ostream& os, const S_lpsrContext& elt);

//______________________________________________________________________________
class lpsrChordNamesContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrChordNamesContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrVoice              contextVoice);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrChordNamesContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrVoice              contextVoice);
      
    virtual ~lpsrChordNamesContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getContextVoice () const
                              { return fContextVoice; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrVoice            fContextVoice;
};
typedef SMARTP<lpsrChordNamesContext> S_lpsrChordNamesContext;
EXP ostream& operator<< (ostream& os, const S_lpsrChordNamesContext& elt);

//______________________________________________________________________________
class lpsrFiguredBassContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrFiguredBassContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrStaff              contextStaff);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrFiguredBassContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrStaff              contextStaff);
      
    virtual ~lpsrFiguredBassContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getContextStaff () const
                              { return fContextStaff; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStaff            fContextStaff;
};
typedef SMARTP<lpsrFiguredBassContext> S_lpsrFiguredBassContext;
EXP ostream& operator<< (ostream& os, const S_lpsrFiguredBassContext& elt);

//______________________________________________________________________________
class lpsrBarCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBarCommand> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBarCommand (
      int inputLineNumber);
      
    virtual ~lpsrBarCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsrBarCommand> S_lpsrBarCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrBarCommand& elt);

//______________________________________________________________________________
class lpsrMelismaCommand : public lpsrElement
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
      
    virtual ~lpsrMelismaCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrMelismaKind       getMelismaKind () const
                              { return fMelismaKind; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    lpsrMelismaKind fMelismaKind;

};
typedef SMARTP<lpsrMelismaCommand> S_lpsrMelismaCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& elt);

//______________________________________________________________________________
class lpsrHeader : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrHeader> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrHeader (
      int inputLineNumber);
      
    virtual ~lpsrHeader ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // MusicXML informations

    void                  setWorkNumber (
                            int    inputLineNumber,
                            string val);

    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    void                  setMovementNumber (
                            int    inputLineNumber,
                            string vall);

    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);

    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    void                  setMiscellaneousField (
                            int    inputLineNumber,
                            string val);

    void                  setScoreInstrument (
                            int    inputLineNumber,
                            string val);

    S_lpsrVarValAssoc
                          getWorkNumber () const
                              { return fWorkNumber; }
    
    S_lpsrVarValAssoc
                          getWorkTitle () const
                              { return fWorkTitle; }
    
    S_lpsrVarValAssoc
                          getMovementNumber () const
                              { return fMovementNumber; }
    
    S_lpsrVarValAssoc
                          getMovementTitle () const
                              { return fMovementTitle; }
    
    S_lpsrVarValsListAssoc
                          getComposers () const
                              { return fComposers; };
    
    S_lpsrVarValsListAssoc
                          getArrangers () const
                              { return fArrangers; };
    
    S_lpsrVarValsListAssoc
                          getLyricists () const
                              { return fLyricists; };
    
    S_lpsrVarValsListAssoc
                          getPoets () const
                              { return fPoets; };
    
    S_lpsrVarValsListAssoc
                          getRights () const
                              { return fRights; }
    
    S_lpsrVarValsListAssoc
                          getSoftwares () const
                              { return fSoftwares; };
    
    S_lpsrVarValAssoc
                          getEncodingDate () const
                              { return fEncodingDate; }

    S_lpsrVarValAssoc
                          getScoreInstrument () const
                              { return fScoreInstrument; }

    S_lpsrVarValAssoc
                          getMiscellaneousField () const
                              { return fMiscellaneousField; }

    // LilyPond informations

    // centered
    
    string                getLilypondDedication () const
                              { return fLilypondDedication; }
    string                getLilypondTitle () const
                              { return fLilypondTitle; }
    string                getLilypondSubtitle () const
                              { return fLilypondSubtitle; }
    string                getLilypondSubsubtitle () const
                              { return fLilypondSubsubtitle; }
                              
    void                  setLilypondDedication (
                            string lilypondDedication)
                              {
                                fLilypondDedication = lilypondDedication;
                              }
    void                  setLilypondTitle (
                            string lilypondTitle)
                              {
                                fLilypondTitle = lilypondTitle;
                              }
    void                  setLilypondSubtitle (
                            string lilypondSubtitle)
                              {
                                fLilypondSubtitle = lilypondSubtitle;
                              }
    void                  setLilypondSubsubtitle (
                            string lilypondSubsubtitle)
                              {
                                fLilypondSubsubtitle = lilypondSubsubtitle;
                              }
    
    // evenly spread on one line
    // "instrument" also appears on following pages
    
    string                getLilypondInstrument () const
                              { return fLilypondInstrument; }
    string                getLilypondPoet () const
                              { return fLilypondPoet; }
    string                getLilypondComposer () const
                              { return fLilypondComposer; }
                              
    void                  setLilypondInstrument (
                            string lilypondInstrument)
                              {
                                fLilypondInstrument = lilypondInstrument;
                              }
    void                  setLilypondPoet (
                            string lilypondPoet)
                              {
                                fLilypondPoet = lilypondPoet;
                              }
    void                  setLilypondComposer (
                            string lilypondComposer)
                              {
                                fLilypondComposer = lilypondComposer;
                              }
    
    // at opposite ends of the same line
    string                getLilypondMeter () const
                              { return fLilypondMeter; }
    string                getLilypondArranger () const
                              { return fLilypondArranger; }

    void                  setLilypondMeter (
                            string lilypondMeter)
                              {
                                fLilypondMeter = lilypondMeter;
                              }
    void                  setLilypondArranger (
                            string lilypondArranger)
                              {
                                fLilypondArranger = lilypondArranger;
                              }
    
    // centered at the bottom of the first page
    string                getLilypondCopyright () const
                              { return fLilypondCopyright; }
                              
    void                  setLilypondCopyright (
                            string lilypondCopyright)
                              {
                                fLilypondCopyright = lilypondCopyright;
                              }
    
    // centered at the bottom of the last page
    string                getLilypondTagline () const
                              { return fLilypondTagline; }
                              
    void                  setLilypondTagline (
                            string lilypondTagline)
                              {
                                fLilypondTagline = lilypondTagline;
                              }

  public:

    // public services
    // ------------------------------------------------------

  public:

    void                  addRights (
                            int    inputLineNumber,
                            string value);

    void                  addComposer (
                            int    inputLineNumber,
                            string value);

    void                  addArranger (
                            int    inputLineNumber,
                            string value);

    void                  addLyricist (
                            int    inputLineNumber,
                            string value);

    void                  addPoet (
                            int    inputLineNumber,
                            string value);

    void                  addSoftware (
                            int    inputLineNumber,
                            string value);

  private:

    // private services
    // ------------------------------------------------------

    int                   maxLilypondVariablesNamesLength ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // MusicXML informations
    
    S_lpsrVarValAssoc
                          fWorkNumber;
    S_lpsrVarValAssoc
                          fWorkTitle;
    S_lpsrVarValAssoc
                          fMovementNumber;
    S_lpsrVarValAssoc
                          fMovementTitle;
    
    S_lpsrVarValsListAssoc
                          fRights;
                          
    S_lpsrVarValsListAssoc
                          fComposers;
    S_lpsrVarValsListAssoc
                          fArrangers;
    S_lpsrVarValsListAssoc
                          fLyricists;
    S_lpsrVarValsListAssoc
                          fPoets;
    
    S_lpsrVarValsListAssoc
                          fSoftwares;
    
    S_lpsrVarValAssoc
                          fEncodingDate;
    
    S_lpsrVarValAssoc
                          fScoreInstrument;

    S_lpsrVarValAssoc
                          fMiscellaneousField;

    // Lilypond informations

    // centered
    string                fLilypondDedication;
    string                fLilypondTitle;
    string                fLilypondSubtitle;
    string                fLilypondSubsubtitle;
    
    // evenly spread on one line
    // "instrument" also appears on following pages
    string                fLilypondInstrument;
    string                fLilypondPoet;
    string                fLilypondComposer;
    
    // at opposite ends of the same line
    string                fLilypondMeter;
    string                fLilypondArranger;
    
    // centered at the bottom of the first page
    string                fLilypondCopyright;
    
    // centered at the bottom of the last page
    string                fLilypondTagline;    
};
typedef SMARTP<lpsrHeader> S_lpsrHeader;
EXP ostream& operator<< (ostream& os, const S_lpsrHeader& elt);

//______________________________________________________________________________
class lpsrPaper : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPaper> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPaper (
      int inputLineNumber);
      
    virtual ~lpsrPaper ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // page width, height, margins and indents

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    
    void    setIndent             (float val);
    void    setShortIndent        (float val);

    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    
    float   getIndent             () const    { return fIndent; }
    float   getShortIndent        () const    { return fShortIndent; }

    // spaces

    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }

    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }

    // headers and footers

    void    setOddHeaderMarkup   (string val) { fOddHeaderMarkup = val; }
    void    setEvenHeaderMarkup  (string val) { fEvenHeaderMarkup = val; }
    void    setOddFooterMarkup   (string val) { fOddFooterMarkup = val; }
    void    setEvenFooterMarkup  (string val) { fEvenFooterMarkup = val; }

    string  getOddHeaderMarkup    () const    { return fOddHeaderMarkup; }
    string  getEvenHeaderMarkup   () const    { return fEvenHeaderMarkup; }
    string  getOddFooterMarkup    () const    { return fOddFooterMarkup; }
    string  getEvenFooterMarkup   () const    { return fEvenFooterMarkup; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);


  private:

    // fields
    // ------------------------------------------------------

    // page width, height, margins and indents (centimeters)
    
    float             fPaperWidth;
    float             fPaperHeight;
    
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;
    
    float             fIndent;
    float             fShortIndent;

    // spaces (centimeters)
    
    float             fBetweenSystemSpace;
    float             fPageTopSpace;

    // headers and footers
    
    string            fOddHeaderMarkup;
    string            fEvenHeaderMarkup;
    string            fOddFooterMarkup;
    string            fEvenFooterMarkup;
};
typedef SMARTP<lpsrPaper> S_lpsrPaper;
EXP ostream& operator<< (ostream& os, const S_lpsrPaper& elt);

//______________________________________________________________________________
class lpsrLayout : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrLayout> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrLayout (
      int inputLineNumber);
      
    virtual ~lpsrLayout ();
  
  public:

    // set and get
    // ------------------------------------------------------

    float                 getStaffSize () const
                              { return fStaffSize; }
                
    void                  setGlobalStaffSize (float size)
                              { fStaffSize = size; }

    // services JMI ???
    // ------------------------------------------------------

    void                  addLilypondVarValAssoc (
                            S_lpsrVarValAssoc assoc)
                              {
                                flpsrVarValAssocs.push_back (assoc);
                              }
      
    void                  addSchemeVariable (
                            S_lpsrSchemeVariable assoc)
                              {
                                fLpsrSchemeVariables.push_back (assoc);
                              }

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    float
                          fStaffSize;
    
    vector<S_lpsrVarValAssoc>
                          flpsrVarValAssocs;
    vector<S_lpsrSchemeVariable>
                          fLpsrSchemeVariables;
};
typedef SMARTP<lpsrLayout> S_lpsrLayout;
EXP ostream& operator<< (ostream& os, const S_lpsrLayout& elt);

//______________________________________________________________________________
class lpsrStaffBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrStaffBlock> create (
      S_msrStaff staff);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrStaffBlock (
      S_msrStaff staff);
      
    virtual ~lpsrStaffBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaff () const
                              { return fStaff; }

    const list<S_msrElement>&
                          getStaffBlockElements () const
                              { return fStaffBlockElements; }

    void                  setStaffBlockInstrumentName (
                            string instrumentName)
                              {
                                fStaffBlockInstrumentName =
                                  instrumentName;
                              }

    string                getStaffBlockInstrumentName () const
                              { return fStaffBlockInstrumentName; }

    void                  setStaffBlockShortInstrumentName (
                            string shortInstrumentName)
                              {
                                fStaffBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    string                getStaffBlockShortInstrumentName () const
                              { return fStaffBlockShortInstrumentName; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendVoiceUseToStaffBlock (
                            S_msrVoice voice);

    void                  appendLyricsUseToStaffBlock (
                            S_msrStanza stanza);

    void                  appendElementToStaffBlock (
                            S_msrElement elem)
                              { fStaffBlockElements.push_back (elem); }
                  
  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaff            fStaff;

    list<S_msrElement>    fStaffBlockElements;

    string                fStaffBlockInstrumentName;
    string                fStaffBlockShortInstrumentName;
};
typedef SMARTP<lpsrStaffBlock> S_lpsrStaffBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrStaffBlock& elt);

//______________________________________________________________________________
class lpsrPartBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartBlock> create (
      S_msrPart part);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPartBlock (
      S_msrPart part);
      
    virtual ~lpsrPartBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getPart () const
                              { return fPart; }

    const list<S_msrElement>&
                          getPartBlockElementsList () const
                              { return fPartBlockElementsList; }

    void                  setPartBlockInstrumentName (
                            string instrumentName)
                              {
                                fPartBlockInstrumentName =
                                  instrumentName;
                              }

    string                getPartBlockInstrumentName () const
                              { return fPartBlockInstrumentName; }

    void                  setPartBlockShortInstrumentName (
                            string shortInstrumentName)
                              {
                                fPartBlockShortInstrumentName =
                                  shortInstrumentName;
                              }

    string                getPartBlockShortInstrumentName () const
                              { return fPartBlockShortInstrumentName; }

  public:
  
    // pbulic services
    // ------------------------------------------------------

    void                  appendStaffBlockToPartBlock (
                            S_lpsrStaffBlock staffBlock);

    void                  appendChordNamesContextToPartBlock (
                            int                     inputLineNumber,
                            S_lpsrChordNamesContext chordNamesContext);

    void                  appendFiguredBassContextToPartBlock (
                            S_lpsrFiguredBassContext context);

  private:
  
    // private services
    // ------------------------------------------------------

    // voices ordering in staves
    
    static bool           compareElementsToHaveHarmoniesAboveCorrespondingStaff (
                            const S_msrElement& first,
                            const S_msrElement& second);

    static bool           compareStaffBlockWithOtherElement (
                            const S_lpsrStaffBlock& staffBlock,
                            const S_msrElement&     otherElement);

    static bool           compareChordNamesContextWithOtherElement (
                            const S_lpsrChordNamesContext& chordNamesContext,
                            const S_msrElement&            otherElement);

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrPart             fPart;

    list<S_msrElement>    fPartBlockElementsList;

    string                fPartBlockInstrumentName; // JMI
    string                fPartBlockShortInstrumentName; // JMI
};
typedef SMARTP<lpsrPartBlock> S_lpsrPartBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrPartBlock& elt);

//______________________________________________________________________________
class lpsrPartGroupBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrPartGroupBlock> create (
      S_msrPartGroup partGroup);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrPartGroupBlock (
      S_msrPartGroup partGroup);
      
    virtual ~lpsrPartGroupBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPartGroup        getPartGroup () const
                              { return fPartGroup; }

    const list<S_msrElement>&
                          getPartGroupBlockElements () const
                              { return fPartGroupBlockElements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendElementToPartGroupBlock (
                            S_msrElement elem)
                              { fPartGroupBlockElements.push_back (elem); }

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrPartGroup        fPartGroup;
    
    list<S_msrElement>    fPartGroupBlockElements;
};
typedef SMARTP<lpsrPartGroupBlock> S_lpsrPartGroupBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrPartGroupBlock& elt);

//______________________________________________________________________________
class lpsrParallelMusicBLock : public lpsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum lpsrElementsSeparatorKind {
      kEndOfLine, kSpace};

    static string elementsSeparatorKindAsString (
      lpsrElementsSeparatorKind elementsSeparatorKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrParallelMusicBLock> create (
      int                       inputLineNumber,
      lpsrElementsSeparatorKind elementsSeparatorKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrParallelMusicBLock (
      int                       inputLineNumber,
      lpsrElementsSeparatorKind elementsSeparatorKind);
      
    virtual ~lpsrParallelMusicBLock ();
    
  public:

    // set and get
    // ------------------------------------------------------

    const list<S_lpsrPartGroupBlock>&
                          getParallelMusicBLockPartGroupBlocks () const
                              { return fParallelMusicBLockPartGroupBlocks; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToParallelMusicBLock ( // JMI
                            S_lpsrPartGroupBlock partGroupBlock)
                              {
                                fParallelMusicBLockPartGroupBlocks.push_back (
                                  partGroupBlock);
                              }
                    
/* JMI
    S_lpsrPartGroupBlock  getLastPartGroupBlockOfParallelMusicBLock ()
                              {
                                return
                                  fParallelMusicBLockPartGroupBlocks.back ();
                              }
                    
    void                  removeLastPartGroupBlockOfParallelMusicBLock () // JMI
                              {
                                fParallelMusicBLockPartGroupBlocks.pop_back ();
                              }
*/

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    list<S_lpsrPartGroupBlock>
                          fParallelMusicBLockPartGroupBlocks;
    
    lpsrElementsSeparatorKind
                          fElementsSeparatorKind;
};
typedef SMARTP<lpsrParallelMusicBLock> S_lpsrParallelMusicBLock;
EXP ostream& operator<< (ostream& os, const S_lpsrParallelMusicBLock& elt);

//______________________________________________________________________________
class lpsrScoreBlock : public lpsrElement
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
      
    virtual ~lpsrScoreBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusicBLock
                          getScoreBlockParallelMusicBLock () const
                              {
                                return
                                  fScoreBlockParallelMusicBLock;
                              }

/*
    const vector<S_msrElement>&
                  getScoreBlockElements () const
                      { return fScoreBlockElements; }
*/
    S_lpsrLayout          getScoreBlockLayout () const
                            { return fScoreBlockLayout; }

    S_msrMidi             getScoreBlockMidi () const
                              { return fScoreBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToScoreBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

/* JMI
    void                  appendVoiceUseToParallelMusic (
                            S_lpsrUseVoiceCommand voiceUse);

    void                  appendLyricsUseToParallelMusic (
                            S_lpsrNewLyricsBlock lyricsUse);
                            */

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_lpsrParallelMusicBLock
                          fScoreBlockParallelMusicBLock;
    
    S_lpsrLayout          fScoreBlockLayout;
    
    S_msrMidi             fScoreBlockMidi;    
};
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreBlock& elt);

//______________________________________________________________________________
class lpsrScore : public lpsrElement
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
      
    virtual ~lpsrScore ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

    S_lpsrVarValAssoc
                          getLilypondVersion () const
                              { return fLilypondVersion; }

    S_lpsrSchemeVariable
                          getGlobalStaffSizeAssoc () const
                              { return fGlobalStaffSizeAssoc; }

    S_lpsrHeader          getHeader () const
                              { return fHeader; }

    S_lpsrPaper           getPaper () const
                              { return fPaper; }

    S_lpsrLayout          getScoreLayout () const
                              { return fScoreLayout; }

    const list<S_msrElement>&
                          getVoicesAndLyricsList () const
                              { return fScoreElements; }

    S_lpsrScoreBlock      getScoreBlock () const
                              { return fScoreBlock; }

    void                  setGlobalStaffSize (float size);

    // files includes
    void                  setJianpuFileIncludeIsNeeded ();
                        
    bool                  getJianpuFileIncludeIsNeeded () const
                              { return fJianpuFileIncludeIsNeeded; }

    // Scheme modules
    void                  setScmAndAccregSchemeModulesAreNeeded ();
                        
    bool                  getScmAndAccregSchemeModulesAreNeeded () const
                              { return fScmAndAccregSchemeModulesAreNeeded; }

    // Scheme functions
    void                  setCustomShortBarLineSchemeFunctionIsNeeded ();
                        
    bool                  getCustomShortBarLineSchemeFunctionIsNeeded () const
                              { return fCustomShortBarLineSchemeFunctionIsNeeded; }
    
    void                  setTongueSchemeFunctionIsNeeded ();
                        
    bool                  getTongueSchemeFunctionIsNeeded () const
                              { return fTongueSchemeFunctionIsNeeded; }
    
    void                  setEditorialAccidentalSchemeFunctionIsNeeded ();
                        
    bool                  getEditorialAccidentalSchemeFunctionIsNeeded () const
                              {
                                return
                                  fEditorialAccidentalSchemeFunctionIsNeeded;
                              }
    
    void                  setDynamicsSchemeFunctionIsNeeded ();
                        
    bool                  getDynamicsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fDynamicsSchemeFunctionIsNeeded;
                              }
    
    void                  setTupletsCurvedBracketsSchemeFunctionIsNeeded ();

    bool                  getTupletsCurvedBracketsSchemeFunctionIsNeeded () const
                              {
                                return
                                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
                              }
    
    void                  setAfterSchemeFunctionIsNeeded ();

    bool                  getAfterSchemeFunctionIsNeeded () const
                              {
                                return
                                  fAfterSchemeFunctionIsNeeded;
                              }
    
    void                  setTempoRelationshipSchemeFunctionIsNeeded ();
                        
    // markups
    void                  setDampMarkupIsNeeded ();
                        
    void                  setDampAllMarkupIsNeeded ();

  public:
  
    // public services
    // ------------------------------------------------------

/* JMI ???
    void                  appendCommentToScore (
                            S_lpsrComment comment)
                              { fScoreElements.push_back (comment); }
                  
    void                  appendSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_back (assoc); }
                  
    void                  prependSchemeVariableToScore (
                            S_lpsrSchemeVariable assoc)
                              { fScoreElements.push_front (assoc); }
                  
    void                  appendVoiceUseToStoreCommand (
                            S_msrVoice voice);

    void                  appendLyricsUseToStoreCommand (
                            S_msrStanza stanza);
*/

    void                  appendVoiceToScoreElements (
                            S_msrVoice voice)
                              { fScoreElements.push_back (voice); }
                  
    void                  appendStanzaToScoreElements (
                            S_msrStanza stanza)
                              { fScoreElements.push_back (stanza); }

  private:
  
    // private services
    // ------------------------------------------------------

    // files includes

    void                  addJianpuFileIncludeToScore ();

    // Scheme modules

    void                  addAccordionRegistrationSchemeModulesToScore ();

    // Scheme functions
    
    void                  addDateAndTimeSchemeFunctionsToScore ();
    
    void                  addPointAndClickOffSchemeFunctionsToScore ();
    
    void                  addCustomShortBarLineSchemeFunctionToScore ();

    void                  addTongueSchemeFunctionToScore ();
    
    void                  addEditorialAccidentalSchemeFunctionToScore ();

    void                  addDynamicsSchemeFunctionToScore ();
    
    void                  addTupletsCurvedBracketsSchemeFunctionToScore ();
    
    void                  addAfterSchemeFunctionToScore ();
    
    void                  addTempoRelationshipSchemeFunctionToScore ();

    // markups
    
    void                  addDampMarkupToScore ();
    void                  addDampAllMarkupToScore ();

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // MSR data
    S_msrScore            fMsrScore;

    // general information
    S_lpsrVarValAssoc
                          fLilypondVersion;
    
    S_lpsrComment         fInputSourceNameComment;
    S_lpsrComment         fTranslationDateComment;
    S_lpsrComment         fCommandLineLongOptionsComment;
    S_lpsrComment         fCommandLineShortOptionsComment;
    
    S_lpsrSchemeVariable
                          fGlobalStaffSizeAssoc;
    
    S_lpsrHeader          fHeader;
    S_lpsrPaper           fPaper;
    S_lpsrLayout          fScoreLayout;

    // to keep the original line breaks
    S_lpsrVarValAssoc
                          fMyBreakIsBreakAssoc;
    S_lpsrVarValAssoc
                          fMyBreakIsEmptyAssoc;

    // to keep the original page breaks
    S_lpsrVarValAssoc
                          fMyPageBreakIsPageBreakAssoc;
    S_lpsrVarValAssoc
                          fMyPageBreakIsEmptyAssoc;

    // to generate 'global' variable
    S_lpsrVarValAssoc
                          fGlobalAssoc;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElements;

    // score command
    S_lpsrScoreBlock      fScoreBlock;

    // files includes
    bool                  fJianpuFileIncludeIsNeeded;

    // Scheme modules
    bool                  fScmAndAccregSchemeModulesAreNeeded;

    // Scheme functions
    bool                  fTongueSchemeFunctionIsNeeded;
    bool                  fCustomShortBarLineSchemeFunctionIsNeeded;
    bool                  fEditorialAccidentalSchemeFunctionIsNeeded;
    bool                  fDynamicsSchemeFunctionIsNeeded;
    bool                  fTupletsCurvedBracketsSchemeFunctionIsNeeded;
    bool                  fAfterSchemeFunctionIsNeeded;
    bool                  fTempoRelationshipSchemeFunctionIsNeeded;

    // markups
    bool                  fDampMarkupIsNeeded;
    bool                  fDampAllMarkupIsNeeded;

    map<string, S_lpsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
};
typedef SMARTP<lpsrScore> S_lpsrScore;
EXP ostream& operator<< (ostream& os, const S_lpsrScore& elt);

//______________________________________________________________________________
EXP void initializeLPSR ();


}


#endif
