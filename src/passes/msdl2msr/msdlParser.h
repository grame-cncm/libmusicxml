/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msdlParser__
#define __msdlParser__

#include "msr.h"

#include "msdlTokens.h"

#include "msdlScanner.h"

#include "msdlParserWaeHandlers.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
class msdlParser : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlParser> create (
                            string   inputSourceName,
                            istream& inputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlParser (
                            string   inputSourceName,
                            istream& inputStream);

    virtual               ~msdlParser ();

    // initialization
    // ------------------------------------------------------

    void                  initializeTokensHandling ();

    // set and get
    // ------------------------------------------------------

    S_msrScore            getCurrentScore () const
                              { return fCurrentScore; }

    bool                  getSourceIsSyntacticallyCorrect () const
                              { return fSourceIsSyntacticallyCorrect; }

  public:

    // public services
    // ------------------------------------------------------

    void                  parse ();

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const;

  private:

    // private services
    // ------------------------------------------------------

    // lexical analysis
    string                currentTokenAsString () const;
    string                currentTokenAsMsdlString () const;

    void                  fetchNextToken ();

    // error recovery
    void                  displayTokenKindsSetsStack (string context);
    bool                  isCurrentTokenKindInSetsStack (string context);

    bool                  checkMandatoryTokenKind (
                            string        fileName,
                            int           lineNumber,
                            msdlTokenKind tokenKind,
                            string        context);

    bool                  checkMandatoryTokenKindsSet (
                            string              fileName,
                            int                 lineNumber,
                            S_msdlTokenKindsSet tokenKindsSet,
                            string              context);

    bool                  checkOptionalTokenKind (
                            string        fileName,
                            int           lineNumber,
                            msdlTokenKind tokenKind,
                            string        context);

    bool                  checkOptionalTokenKindsSet (
                            string              fileName,
                            int                 lineNumber,
                            S_msdlTokenKindsSet tokenKindsSet,
                            string              context);

  private:

    // private parsing fields
    // ------------------------------------------------------

    // trace
#ifdef TRACING_IS_ENABLED
    bool                  fTraceSyntax;
    bool                  fTraceSyntaxDetails;

    bool                  fTraceSyntaxErrorRecovery;
    bool                  fTraceSyntaxErrorRecoveryDetails;
#endif

    // software
    string                fSoftware;

    // user language
    msdlUserLanguageKind  fUserLanguageKind;

    // keywords language
    msdlKeywordsLanguageKind
                          fKeywordsInputLanguageKind;

    S_msrIdentification   fMsrIdentification;

    // pitches
    msrQuarterTonesPitchesLanguageKind
                          fPitchesLanguageKind;

    // octaves
    msrOctaveEntryKind    fOctavesEntryKind;

    // input source name
    string                fInputSourceName;

    // scanner
    msdlScanner           fScanner;

    // tokens
    msdlIgnoreSeparatorTokensKind
                          fIgnoreSeparatorTokensKind;

    msdlTokenKind         fCurrentTokenKind;
    const msdlToken&      fCurrentToken;

    // error recovery
    S_msdlTokenKindsSet   fEmptyTokenKindsSet;
    S_msdlTokenKindsSet   fTokenEOFTokenKindsSet;

    list<S_msdlTokenKindsSet>
                          fMsdlTokensSetsStack;

    // warnings and errors

    S_msdlParserWaeHandler
                          fParserWaeHandler;

    // syntax correctness
    bool                  fSourceIsSyntacticallyCorrect;

  private:

    // private score building fields
    // ------------------------------------------------------

    // book
    S_msrBook             fCurrentBook;

    // score
    int                   fCurrentScoreNumber;
    S_msrScore            fCurrentScore;

    // partGroup
    int                   fCurrentPartGroupNumber;
    S_msrPartGroup        fCurrentPartGroup;

    // part
    int                   fCurrentPartNumber;
    S_msrPart             fCurrentPart;

    // staff
    int                   fCurrentStaffNumber;
    S_msrStaff            fCurrentStaff;

    // voice
    int                   fCurrentVoiceNumber;
    S_msrVoice            fCurrentVoice;

    // measure
    int                   fCurrentMeasureNumber;
    S_msrMeasure          fCurrentMeasure;

    // octaves entry
      /* this reference is:
            mobile in relative mode
            unused in absolute mode
            fixed  in fixed mode
      */
    S_msrNote             fCurrentOctaveEntryReference;
    void                  setCurrentOctaveEntryReference ();

    string                lilypondOctaveInRelativeEntryMode (
                            S_msrNote note);

    string                lilypondOctaveInFixedEntryMode (
                            S_msrNote note);

    // note
    rational              fCurrentNoteSoundingWholeNotes;
    rational              fCurrentNoteDisplayWholeNotes;
    int                   fCurrentNoteDotsNumber;

    // private score building methods
    // ------------------------------------------------------

    void                  createBookIfNeeded (int inputLineNumber);

    void                  createScoreIfNeeded (int inputLineNumber);
    void                  createIdentificationIfNeeded (int inputLineNumber);

    void                  createPartGroupIfNeeded (int inputLineNumber);

    void                  createPartIfNeeded (int inputLineNumber);

    void                  createStaffIfNeeded (int inputLineNumber);

    void                  createVoiceIfNeeded (int inputLineNumber);

    void                  createMeasureNumber (
                            int inputLineNumber,
                            int measureNumber);

  private:

    // syntax error recovery
    // ------------------------------------------------------

    // Identification
    S_msdlTokenKindsSet   fIdentificationFIRST;
    S_msdlTokenKindsSet   fIdentificationFOLLOW;

    // Anacrusis
    bool                  fAnacrusis;

    // Structure
    S_msdlTokenKindsSet   fStructureFIRST;
    S_msdlTokenKindsSet   fStructureFOLLOW;

    // Book
    S_msdlTokenKindsSet   fBookFIRST;
    S_msdlTokenKindsSet   fBookFOLLOW;

    // Score
    S_msdlTokenKindsSet   fScoreFIRST;
    S_msdlTokenKindsSet   fScoreFOLLOW;

    // PartGroup
    S_msdlTokenKindsSet   fPartGroupFIRST;
    S_msdlTokenKindsSet   fPartGroupFOLLOW;

    // Part
    S_msdlTokenKindsSet   fPartFIRST;
    S_msdlTokenKindsSet   fPartFOLLOW;

    // Music
    S_msdlTokenKindsSet   fMusicFIRST;
    S_msdlTokenKindsSet   fMusicFOLLOW;

    // MeasuresSequence
    S_msdlTokenKindsSet   fMeasuresSequenceFIRST;
    S_msdlTokenKindsSet   fMeasuresSequenceFOLLOW;

    // Measure
    S_msdlTokenKindsSet   fMeasureFIRST;
    S_msdlTokenKindsSet   fMeasureFOLLOW;

    // Clef
    S_msdlTokenKindsSet   fClefFIRST;
    S_msdlTokenKindsSet   fClefFOLLOW;

    // Note
    S_msdlTokenKindsSet   fNoteFIRST;
    S_msdlTokenKindsSet   fNoteFOLLOW;

    // Pitch
    S_msdlTokenKindsSet   fPitchFIRST;
    S_msdlTokenKindsSet   fPitchFOLLOW;

    // Octaves
    S_msdlTokenKindsSet   fOctavesFIRST;
    S_msdlTokenKindsSet   fOctavesFOLLOW;

    // OctaveIndication
    S_msdlTokenKindsSet   fOctaveIndicationFIRST;
    S_msdlTokenKindsSet   fOctaveIndicationFOLLOW;

    // NoteDuration
    S_msdlTokenKindsSet   fNoteDurationFIRST;
    S_msdlTokenKindsSet   fNoteDurationFOLLOW;

  private:

    // non-terminal notions
    // ------------------------------------------------------


    // syntactical analysis, the main non-terminal notion
    void                  Specification (S_msdlTokenKindsSet stopperTokensSet);

    // identification
    void                  Identification (S_msdlTokenKindsSet stopperTokensSet);

    void                    Title (S_msdlTokenKindsSet stopperTokensSet);

    void                    Composer (S_msdlTokenKindsSet stopperTokensSet);

    void                    Opus (S_msdlTokenKindsSet stopperTokensSet);

    // identifier
    void                  Identifier (S_msdlTokenKindsSet stopperTokensSet);

    // pitches
    void                  Pitches (S_msdlTokenKindsSet stopperTokensSet);

    void                    PitchesLanguage (S_msdlTokenKindsSet stopperTokensSet);

    void                    PitchesOctaveEntry (S_msdlTokenKindsSet stopperTokensSet);

    // octaves
    void                  Octaves (S_msdlTokenKindsSet stopperTokensSet);

    // anacrusis
    void                  Anacrusis (S_msdlTokenKindsSet stopperTokensSet);

    // structure
    void                  Structure (S_msdlTokenKindsSet stopperTokensSet);

    // book
    void                    Book (S_msdlTokenKindsSet stopperTokensSet);

    // score
    void                    Score (S_msdlTokenKindsSet stopperTokensSet);

    // part group
    void                    PartGroup (S_msdlTokenKindsSet stopperTokensSet);

    // part
    void                    Part (S_msdlTokenKindsSet stopperTokensSet);

    // music
    void                    Music (S_msdlTokenKindsSet stopperTokensSet);

    // fragment
    void                    Fragment (S_msdlTokenKindsSet stopperTokensSet);

    // measures sequence
    void                  MeasuresSequence (S_msdlTokenKindsSet stopperTokensSet);

    // measure
    void                  Measure (S_msdlTokenKindsSet stopperTokensSet);

    // measure number
    void                  MeasureNumber (S_msdlTokenKindsSet stopperTokensSet);

    // clef
    void                  Clef (S_msdlTokenKindsSet stopperTokensSet);

    // key
    void                  Key (S_msdlTokenKindsSet stopperTokensSet);

    // time
    void                  Time (S_msdlTokenKindsSet stopperTokensSet);

    // note
    void                  Note (S_msdlTokenKindsSet stopperTokensSet);

    void                    Pitch (S_msdlTokenKindsSet stopperTokensSet);

    msrOctaveKind           OctaveIndication (S_msdlTokenKindsSet stopperTokensSet);

    void                    NoteDuration (S_msdlTokenKindsSet stopperTokensSet);
};
typedef SMARTP<msdlParser> S_msdlParser;
EXP ostream& operator<< (ostream& os, const msdlParser& elt);


}


#endif
