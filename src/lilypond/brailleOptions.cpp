/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include <regex>

#include "utilities.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "generalOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_brailleUTFKindAtom brailleUTFKindAtom::create (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  bsrUTFKind brailleUTFKindVariable)
{
  brailleUTFKindAtom* o = new
    brailleUTFKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      brailleUTFKindVariable);
  assert(o!=0);
  return o;
}

brailleUTFKindAtom::brailleUTFKindAtom (
  string     shortName,
  string     longName,
  string     description,
  string     valueSpecification,
  string     variableName,
  bsrUTFKind brailleUTFKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrUTFKindVariable (
      brailleUTFKindVariable)
{}

brailleUTFKindAtom::~brailleUTFKindAtom ()
{}

void brailleUTFKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'brailleUTFKindAtom'" <<
      endl;
  }
#endif

  bsrUTFKind UTFKind = kUTF8; // default value

  if (theString == "8") {
    UTFKind = kUTF8;
  }
  else if (theString == "16") {
    UTFKind = kUTF16;
  }
  else {
    // no, theString is unknown
    stringstream s;

    s <<
      "UTF kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are 8 and 16" <<
      endl;

    optionError (s.str ());
    exit (4);
  }

  fBsrUTFKindVariable = UTFKind;
}

void brailleUTFKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsUTFKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleUTFKindVariable" << " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
      "\"" <<
    endl;
}

void brailleUTFKindAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    bsrUTFKindAsString (
      fBsrUTFKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_brailleUTFKindAtom& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_brailleByteOrderingKindAtom brailleByteOrderingKindAtom::create (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  bsrByteOrderingKind brailleByteOrderingKindVariable)
{
  brailleByteOrderingKindAtom* o = new
    brailleByteOrderingKindAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName,
      brailleByteOrderingKindVariable);
  assert(o!=0);
  return o;
}

brailleByteOrderingKindAtom::brailleByteOrderingKindAtom (
  string              shortName,
  string              longName,
  string              description,
  string              valueSpecification,
  string              variableName,
  bsrByteOrderingKind brailleByteOrderingKindVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      valueSpecification,
      variableName),
    fBsrByteOrderingKindVariable (
      brailleByteOrderingKindVariable)
{}

brailleByteOrderingKindAtom::~brailleByteOrderingKindAtom ()
{}

void brailleByteOrderingKindAtom::handleValue (
  string   theString,
  ostream& os)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'brailleByteOrderingKindAtom'" <<
      endl;
  }
#endif

  bsrByteOrderingKind byteOrderingKind = kByteOrderingNone; // default value

  if (theString == "big") {
    byteOrderingKind = kByteOrderingBigEndian;
  }
  else if (theString == "small") {
    byteOrderingKind = kByteOrderingSmallEndian;
  }
  else {
    // no, theString is unknown
    stringstream s;

    s <<
      "byte ordering '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are big and small" <<
      endl;

    optionError (s.str ());
    exit (4); // JMI
  }

  fBsrByteOrderingKindVariable = byteOrderingKind;
}

void brailleByteOrderingKindAtom::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsByteOrderingKindAtom:" <<
    endl;

  gIndenter++;

  printValuedAtomEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fVariableName" << " : " <<
    fVariableName <<
    endl <<
    setw (fieldWidth) <<
    "brailleByteOrderingKindVariable" << " : \"" <<
    byteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
      "\"" <<
    endl;
}

void brailleByteOrderingKindAtom::printOptionsValues (
  indentedOstream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fVariableName <<
    " : \"" <<
    byteOrderingKindAsString (
      fBsrByteOrderingKindVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_brailleByteOrderingKindAtom& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_brailleOptions gBrailleOptions;
S_brailleOptions gBrailleOptionsUserChoices;
S_brailleOptions gBrailleOptionsWithDetailedTrace;

S_brailleOptions brailleOptions::create (
  S_oahHandler handler)
{
  brailleOptions* o = new brailleOptions (
    handler);
  assert(o!=0);
  return o;
}

brailleOptions::brailleOptions (
  S_oahHandler handler)
  : oahGroup (
    "Braille",
    "hbrl", "help-braille",
R"(These braille control which Braille code is generated.)",
    handler)
{
  // append this braille group to the braille handler
  // if relevant
  if (handler) {
    handler->
      appendGroupToHandler (this);
  }

  // initialize it
  initializeBrailleOptions (false);
}

brailleOptions::~brailleOptions ()
{}

void brailleOptions::initializeBrailleUTFEncodingOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    UTFEncodingSubGroup =
      oahSubGroup::create (
        "UTF encoding",
        "hlpue", "help-utf-encoding",
R"()",
      oahSubGroup::kSubGroupVisibilityAlways,
      this);

  appendSubGroup (UTFEncodingSubGroup);

  // UTF encoding

  fUTFKind = kUTF8; // default value

  UTFEncodingSubGroup->
    appendAtom (
      brailleUTFKindAtom::create (
        "utf", "utf-encoding",
R"(Generate Braille code using UTF ENCODING_SIZE encoding,
which can be one of 8 or 16. Default value is 8.)",
        "ENCODING_SIZE",
        "UTFKind",
        fUTFKind));
}

void brailleOptions::initializeBrailleByteOrderingOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    byteOrderingSubGroup =
      oahSubGroup::create (
        "Byte ordering",
        "hlpbo", "help-byte-ordering",
R"()",
      oahSubGroup::kSubGroupVisibilityAlways,
      this);

  appendSubGroup (byteOrderingSubGroup);

  // byte ordering

  fByteOrderingKind = kByteOrderingNone; // default value

  byteOrderingSubGroup->
    appendAtom (
      brailleByteOrderingKindAtom::create (
        "bom", "byte-ordering-mark",
R"(Generate an initial BOM_ENDIAN byte ordering mark (BOM)
ahead of the Braille nusic code,
which can be one of big or small.
By default, BOM is generated.)",
        "BOM_ENDIAN",
        "byteOrderingKind",
        fByteOrderingKind));
}

void brailleOptions::initializeBrailleMusicFileNameOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    musicFileNameSubGroup =
      oahSubGroup::create (
        "Braille music file name",
        "hlpbmfn", "help-braille-music-file-name",
R"()",
      oahSubGroup::kSubGroupVisibilityAlways,
      this);

  appendSubGroup (musicFileNameSubGroup);

  // encoding in file name

  fUseEncodingInFileName = false; // default value

  musicFileNameSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "ueifn", "use-encoding-in-file-name",
R"(Append a description of the encoding used
and the presence of a BOM if any to the file name.)",
        "useEncodingInFileName",
        fUseEncodingInFileName));
}

void brailleOptions::initializeBraillePageParametersOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    codeGenerationSubGroup =
      oahSubGroup::create (
        "Page parameters",
        "hlpbpp", "help-braille-pages-parameters",
R"()",
      oahSubGroup::kSubGroupVisibilityAlways,
      this);

  appendSubGroup (codeGenerationSubGroup);

  // cells per line

  fCellsPerLine = 30;
  codeGenerationSubGroup->
    appendAtom (
      oahIntegerAtom::create (
        "cpl", "cells-per-line",
R"(Set the number of Braille cells per line to N. Default is 30 for A4 paper.)",
        "N",
        "cellsPerLine",
        fCellsPerLine));

  // measures per line

  fMeasuresPerLine = 7;

  codeGenerationSubGroup->
    appendAtom (
      oahIntegerAtom::create (
        "mpl", "measures-per-line",
R"(Set the number of Braille measures per line to N. Default is 7.)",
        "N",
        "measuresPerLine",
        fMeasuresPerLine));

  // lines per page

  fLinesPerPage = 27;

  codeGenerationSubGroup->
    appendAtom (
      oahIntegerAtom::create (
        "lpp", "lines-per-page",
R"(Set the number of Braille lines per page to N. Default is 27 for A4 paper.)",
        "N",
        "linesPerPage",
        fLinesPerPage));
}

void brailleOptions::initializeBrailleCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_oahSubGroup
    codeGenerationSubGroup =
      oahSubGroup::create (
        "Code generation",
        "hlpcg", "help-braille-code-generation",
R"()",
      oahSubGroup::kSubGroupVisibilityAlways,
      this);

  appendSubGroup (codeGenerationSubGroup);

  // xml2brl infos

  fXml2brlInfos         = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "xi", "xml2brl-infos",
R"(Generate initial comments showing the compilation date and braille.)",
        "xml2brlInfos",
        fXml2brlInfos));

  // braille code

  fNoBrailleCode        = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendAtom (
      oahBooleanAtom::create (
        "nolpc", "no-braille-code",
R"(Don't generate any Braille code.
That can be useful if only a summary of the score is needed.)",
        "noBrailleCode",
        fNoBrailleCode));
}

void brailleOptions::initializeBrailleOptions (
  bool boolOptionsInitialValue)
{
  // UTF encoding
  // --------------------------------------
  initializeBrailleUTFEncodingOptions (
    boolOptionsInitialValue);

  // byte ordering
  // --------------------------------------
  initializeBrailleByteOrderingOptions (
    boolOptionsInitialValue);

  // braille music file name
  // --------------------------------------
  initializeBrailleMusicFileNameOptions (
    boolOptionsInitialValue);

  // page parameters
  // --------------------------------------
  initializeBraillePageParametersOptions (
    boolOptionsInitialValue);

  // code generation
  // --------------------------------------
  initializeBrailleCodeGenerationOptions (
    boolOptionsInitialValue);
}

S_brailleOptions brailleOptions::createCloneWithDetailedTrace ()
{
  S_brailleOptions
    clone =
      brailleOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the braille handler upLink
  clone->fHandlerUpLink =
    fHandlerUpLink;;

  // code generation JMI ???
  // --------------------------------------

  clone->fXml2brlInfos =
    fXml2brlInfos;

  clone->fNoBrailleCode =
    fNoBrailleCode;

  return clone;
}

//______________________________________________________________________________
void brailleOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void brailleOptions::checkOptionsConsistency ()
{
  // JMI
}

void brailleOptions::printBrailleOptionsValues (int fieldWidth)
{
  gLogOstream <<
    "The Braille braille are:" <<
    endl;

  gIndenter++;


  // code generation
  // --------------------------------------

  gLogOstream <<
    "Braille code generation:" <<
    endl;

  gIndenter++;

  gLogOstream << left <<
    setw (fieldWidth) << "UTFKind" << " : " <<
      bsrUTFKindAsString (fUTFKind) <<
      endl <<

    setw (fieldWidth) << "byteOrderingKind" << " : " <<
      byteOrderingKindAsString (fByteOrderingKind) <<
      endl <<

    setw (fieldWidth) << "useEncodingInFileName" << " : " <<
      booleanAsString (fUseEncodingInFileName) <<
      endl <<

    setw (fieldWidth) << "cellsPerLine" << " : " <<
      fCellsPerLine <<
      endl <<
    setw (fieldWidth) << "measuresPerLine" << " : " <<
      fMeasuresPerLine <<
      endl <<
    setw (fieldWidth) << "linesPerPage" << " : " <<
      fLinesPerPage <<
      endl <<

    setw (fieldWidth) << "xml2brlInfos" << " : " <<
      booleanAsString (fXml2brlInfos) <<
      endl <<

    setw (fieldWidth) << "noBrailleCode" << " : " <<
      booleanAsString (fNoBrailleCode) <<
      endl;

  gIndenter--;

  gIndenter--;
}

S_oahValuedAtom brailleOptions::handleAtom (
  indentedOstream&  os,
  S_oahAtom atom)
{
  S_oahValuedAtom result;

  if (
    // UTF kind atom?
    S_brailleUTFKindAtom
      UTFKindAtom =
        dynamic_cast<brailleUTFKindAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'brailleUTFKindAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = UTFKindAtom;
  }

  else if (
    // byte ordering kind atom?
    S_brailleByteOrderingKindAtom
      byteOrderingKindAtom =
        dynamic_cast<brailleByteOrderingKindAtom*>(&(*atom))
    ) {
#ifdef TRACE_OPTIONS
    if (gOahBasicOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'brailleUTFKindAtom'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = byteOrderingKindAtom;
  }

  return result;
}

void brailleOptions::handleValuedAtomValue (
  indentedOstream& os,
  S_oahAtom        atom,
  string           theString)
{
  if (
    // UTF kind atom?
    S_brailleUTFKindAtom
      UTFKindAtom =
        dynamic_cast<brailleUTFKindAtom*>(&(*atom))
  ) {
    // theString contains the UTF size
    UTFKindAtom->handleValue (
      theString,
      os);
  }

  else if (
    // byte ordering atom?
    S_brailleByteOrderingKindAtom
      byteOrderingKindAtom =
        dynamic_cast<brailleByteOrderingKindAtom*>(&(*atom))
  ) {
    // theString contains the byte ordering name
    byteOrderingKindAtom->handleValue (
      theString,
      os);
  }
}

ostream& operator<< (ostream& os, const S_brailleOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBrailleOptionsHandling (
  S_oahHandler handler)
{
#ifdef TRACE_OPTIONS
  if (gOahBasicOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogOstream <<
      "Initializing Braille braille handling" <<
      endl;
  }
#endif

  // create the braille variables
  // ------------------------------------------------------

  gBrailleOptionsUserChoices = brailleOptions::create (
    handler);
  assert(gBrailleOptionsUserChoices != 0);

  gBrailleOptions =
    gBrailleOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gBrailleOptionsWithDetailedTrace =
    gBrailleOptions->
      createCloneWithDetailedTrace ();
}


}


  /*
  // identification
  // --------------------------------------

  {
    // variables

    S_oahSubGroup
      identificationSubGroup =
        oahSubGroup::create (
          "Identification",
          "hlpi", "help-braille-identification",
R"(These braille can be used to enforce values in the generated Braille code,
thus overriding the ones that may be present in the MSR data.
)",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (identificationSubGroup);

    // MusicXML informations

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "rights", "",
R"(Set the 'rights' to STRING in the Braille code.)",
          "STRING",
          "rights",
          fRights));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "composer", "",
R"(Set the 'composer' to STRING in the Braille code.)",
          "STRING",
          "composer",
          fComposer));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "arranger", "",
R"(Set the 'arranger' to STRING in the Braille code.)",
          "STRING",
          "arranger",
          fArranger));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "poet", "",
R"(Set the 'poet' to STRING in the Braille code.)",
          "STRING",
          "poet",
          fPoet));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "lyricist", "",
R"(Set the 'lyricist' to STRING in the Braille code.)",
          "STRING",
          "lyricist",
          fLyricist));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "software", "",
R"(Set the 'software' to STRING in the Braille code.)",
          "STRING",
          "software",
          fSoftware));

    // Braille informations

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
          "STRING",
          "dedication",
          fDedication));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "piece", "",
R"(Set 'piece' to STRING in the \header.)",
          "STRING",
          "piece",
          fPiece));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "opus", "",
R"(Set 'opus' to STRING in the \header.)",
          "STRING",
          "opus",
          fOpus));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "title", "",
R"(Set 'title' to STRING in the \header.)",
          "STRING",
          "title",
          fTitle));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
          "STRING",
          "subTitle",
          fSubTitle));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
          "STRING",
          "subSubTitle",
          fSubSubTitle));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
          "STRING",
          "instrument",
          fInstrument));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "meter", "",
R"(Set 'meter' to STRING in the \header.)",
          "STRING",
          "meter",
          fMeter));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
          "STRING",
          "tagline",
          fTagline));

    identificationSubGroup->
      appendAtom (
        oahStringAtom::create (
          "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
          "STRING",
          "copyright",
          fCopyright));
  }


  // notes
  // --------------------------------------

  {
    // variables

    fAbsoluteOctaves  = boolOptionsInitialValue;

    fAllDurations  = boolOptionsInitialValue;

    fRomanStringNumbers = boolOptionsInitialValue;
    fAvoidOpenStrings    = boolOptionsInitialValue;

    fCompressMultiMeasureRests = boolOptionsInitialValue;

    fInputLineNumbers = boolOptionsInitialValue;

    S_oahSubGroup
      notesSubGroup =
        oahSubGroup::create (
          "Notes",
          "hlpn", "help-braille-notes",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (notesSubGroup);

    notesSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "abs", "absolute",
R"(Generate Braille absolute note octaves.
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    notesSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "alldurs", "all-durations",
R"(Generate all Braille durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    notesSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for Braille to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent Braille from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    notesSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "niln", "note-input-line-numbers",
          replaceSubstringInString (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
            "EXECUTABLE",
            gOahBasicOptions->fHandlerExecutableName),
          "noteInputLineNumbers",
          fInputLineNumbers));
  }


  // bars
  // --------------------------------------

  {
    // variables

    fShowAllBarNumbers = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      barsSubGroup =
        oahSubGroup::create (
          "Bars",
          "hlpb", "help-braille-bars",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (barsSubGroup);

    barsSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "abn", "all-bar-numbers",
R"(Generate Braille code to show all bar numbers.)",
          "showAllBarNumbers",
          fShowAllBarNumbers));
  }


  // line breaks
  // --------------------------------------

  {
    // variables

    fIgnoreLineBreaks                    = boolOptionsInitialValue;

    fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      lineBreaksSubGroup =
        oahSubGroup::create (
          "Line breaks",
          "hlplb", "help-braille-line-breaks",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let Braille decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    lineBreaksSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
          "breakLinesAtIncompleteRightMeasures",
          fBreakLinesAtIncompleteRightMeasures));
  }


  // page breaks
  // --------------------------------------

  {
    // variables

    fIgnorePageBreaks = boolOptionsInitialValue;

    // braille

    S_oahSubGroup
      pageBreaksSubGroup =
        oahSubGroup::create (
          "Page breaks",
          "hlppb", "help-braille-page-breaks",
R"()",
        oahSubGroup::kSubGroupVisibilityAlways,
        this);

    appendSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendAtom (
        oahBooleanAtom::create (
          "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let Braille decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
        */
