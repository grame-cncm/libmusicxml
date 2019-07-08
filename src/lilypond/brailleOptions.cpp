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
S_optionsUTFKindItem optionsUTFKindItem::create (
  string     shortName,
  string     longName,
  string     description,
  string     optionsValueSpecification,
  string     optionsUTFKindItemVariableName,
  bsrUTFKind optionsUTFKindItemVariable)
{
  optionsUTFKindItem* o = new
    optionsUTFKindItem (
      shortName,
      longName,
      description,
      optionsValueSpecification,
      optionsUTFKindItemVariableName,
      optionsUTFKindItemVariable);
  assert(o!=0);
  return o;
}

optionsUTFKindItem::optionsUTFKindItem (
  string     shortName,
  string     longName,
  string     description,
  string     optionsValueSpecification,
  string     optionsUTFKindItemVariableName,
  bsrUTFKind optionsUTFKindItemVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      optionsValueSpecification,
      optionsUTFKindItemVariableName),
    fOptionsUTFKindItemVariable (
      optionsUTFKindItemVariable)
{}

optionsUTFKindItem::~optionsUTFKindItem ()
{}

void optionsUTFKindItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsUTFKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableName" << " : " <<
    fOptionsItemVariableName <<
    endl <<
    setw (fieldWidth) <<
    "optionsUTFKindItemVariable" << " : \"" <<
    bsrUTFKindAsString (
      fOptionsUTFKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsUTFKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableName <<
    " : \"" <<
    bsrUTFKindAsString (
      fOptionsUTFKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsUTFKindItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsByteOrderingKindItem optionsByteOrderingKindItem::create (
  string              shortName,
  string              longName,
  string              description,
  string              optionsValueSpecification,
  string              optionsByteOrderingKindItemVariableName,
  bsrByteOrderingKind optionsByteOrderingKindItemVariable)
{
  optionsByteOrderingKindItem* o = new
    optionsByteOrderingKindItem (
      shortName,
      longName,
      description,
      optionsValueSpecification,
      optionsByteOrderingKindItemVariableName,
      optionsByteOrderingKindItemVariable);
  assert(o!=0);
  return o;
}

optionsByteOrderingKindItem::optionsByteOrderingKindItem (
  string              shortName,
  string              longName,
  string              description,
  string              optionsValueSpecification,
  string              optionsByteOrderingKindItemVariableName,
  bsrByteOrderingKind optionsByteOrderingKindItemVariable)
  : oahValuedAtom (
      shortName,
      longName,
      description,
      optionsValueSpecification,
      optionsByteOrderingKindItemVariableName),
    fOptionsByteOrderingKindItemVariable (
      optionsByteOrderingKindItemVariable)
{}

optionsByteOrderingKindItem::~optionsByteOrderingKindItem ()
{}

void optionsByteOrderingKindItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsByteOrderingKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsItemVariableName" << " : " <<
    fOptionsItemVariableName <<
    endl <<
    setw (fieldWidth) <<
    "optionsByteOrderingKindItemVariable" << " : \"" <<
    byteOrderingKindAsString (
      fOptionsByteOrderingKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsByteOrderingKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsItemVariableName <<
    " : \"" <<
    byteOrderingKindAsString (
      fOptionsByteOrderingKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsByteOrderingKindItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_brailleOptions gBrailleOptions;
S_brailleOptions gBrailleOptionsUserChoices;
S_brailleOptions gBrailleOptionsWithDetailedTrace;

S_brailleOptions brailleOptions::create (
  S_oahHandler oahHandler)
{
  brailleOptions* o = new brailleOptions (
    oahHandler);
  assert(o!=0);
  return o;
}

brailleOptions::brailleOptions (
  S_oahHandler oahHandler)
  : oahGroup (
    "Braille",
    "hbrl", "help-braille",
R"(These options control which Braille code is generated.)",
    oahHandler)
{
  // append this options group to the options handler
  // if relevant
  if (oahHandler) {
    oahHandler->
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendSubGroup (UTFEncodingSubGroup);

  // UTF encoding

  fUTFKind = kUTF8; // default value

  UTFEncodingSubGroup->
    appendOptionsItem (
      optionsUTFKindItem::create (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendSubGroup (byteOrderingSubGroup);

  // byte ordering

  fByteOrderingKind = kByteOrderingNone; // default value

  byteOrderingSubGroup->
    appendOptionsItem (
      optionsByteOrderingKindItem::create (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendSubGroup (musicFileNameSubGroup);

  // encoding in file name

  fUseEncodingInFileName = false; // default value

  musicFileNameSubGroup->
    appendOptionsItem (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendSubGroup (codeGenerationSubGroup);

  // cells per line

  fCellsPerLine = 30;
  codeGenerationSubGroup->
    appendOptionsItem (
      oahIntegerAtom::create (
        "cpl", "cells-per-line",
R"(Set the number of Braille cells per line to N. Default is 30 for A4 paper.)",
        "N",
        "cellsPerLine",
        fCellsPerLine));

  // measures per line

  fMeasuresPerLine = 7;

  codeGenerationSubGroup->
    appendOptionsItem (
      oahIntegerAtom::create (
        "mpl", "measures-per-line",
R"(Set the number of Braille measures per line to N. Default is 7.)",
        "N",
        "measuresPerLine",
        fMeasuresPerLine));

  // lines per page

  fLinesPerPage = 27;

  codeGenerationSubGroup->
    appendOptionsItem (
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
      oahSubGroup::kAlwaysShowDescription,
      this);

  appendSubGroup (codeGenerationSubGroup);

  // xml2brl infos

  fXml2brlInfos         = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      oahBooleanAtom::create (
        "xi", "xml2brl-infos",
R"(Generate initial comments showing the compilation date and options.)",
        "xml2brlInfos",
        fXml2brlInfos));

  // braille code

  fNoBrailleCode        = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
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

  // set the options handler upLink
  clone->
    setOptionsHandlerUpLink (
      fOptionsHandlerUpLink);

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
  gLogIOstream <<
    "The Braille options are:" <<
    endl;

  gIndenter++;


  // code generation
  // --------------------------------------

  gLogIOstream <<
    "Braille code generation:" <<
    endl;

  gIndenter++;

  gLogIOstream << left <<
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

S_oahValuedAtom brailleOptions::handleOptionsItem (
  ostream&      os,
  S_oahAtom item)
{
  S_oahValuedAtom result;

  if (
    // UTF kind item?
    S_optionsUTFKindItem
      UTFKindItem =
        dynamic_cast<optionsUTFKindItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'optionsUTFKindItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = UTFKindItem;
  }

  else if (
    // byte ordering kind item?
    S_optionsByteOrderingKindItem
      byteOrderingKindItem =
        dynamic_cast<optionsByteOrderingKindItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> oahAtom is of type 'optionsUTFKindItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = byteOrderingKindItem;
  }

  return result;
}

void brailleOptions::handleOptionsUTFKindItemValue (
  ostream&             os,
  S_optionsUTFKindItem UTFKindItem,
  string               theString)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'optionsUTFKindItem'" <<
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

    printHelpSummary (os);

    stringstream s;

    s <<
      "UTF kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are 8 and 16" <<
      endl;

    optionError (s.str ());

//     exit (4); // JMI
    abort ();
  }

  UTFKindItem->
    setOptionsUTFKindItemVariable (
      UTFKind);
}

void brailleOptions::handleOptionsByteOrderingKindItemValue (
  ostream&                      os,
  S_optionsByteOrderingKindItem byteOrderingKindItem,
  string                        theString)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> oahAtom is of type 'optionsByteOrderingKindItem'" <<
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

    printHelpSummary (os);

    stringstream s;

    s <<
      "byte ordering '" << theString <<
      "' is unknown" <<
      endl <<
      "The possible values are big and small" <<
      endl;

    optionError (s.str ());

//     exit (4); // JMI
    abort ();
  }

  byteOrderingKindItem->
    setOptionsByteOrderingKindItemVariable (
      byteOrderingKind);
}

void brailleOptions::handleOptionsItemValue (
  ostream&      os,
  S_oahAtom item,
  string        theString)
{
  if (
    // UTF kind item?
    S_optionsUTFKindItem
      UTFKindItem =
        dynamic_cast<optionsUTFKindItem*>(&(*item))
  ) {
    // theString contains the UTF size
    handleOptionsUTFKindItemValue (
      os,
      UTFKindItem,
      theString);
  }

  else if (
    // byte ordering item?
    S_optionsByteOrderingKindItem
      byteOrderingKindItem =
        dynamic_cast<optionsByteOrderingKindItem*>(&(*item))
  ) {
    // theString contains the byte ordering name
    handleOptionsByteOrderingKindItemValue (
      os,
      byteOrderingKindItem,
      theString);
  }
}

ostream& operator<< (ostream& os, const S_brailleOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeBrailleOptionsHandling (
  S_oahHandler oahHandler)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    gLogIOstream <<
      "Initializing Braille options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gBrailleOptionsUserChoices = brailleOptions::create (
    oahHandler);
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
R"(These options can be used to enforce values in the generated Braille code,
thus overriding the ones that may be present in the MSR data.
)",
        oahSubGroup::kAlwaysShowDescription,
        this);

    appendSubGroup (identificationSubGroup);

    // MusicXML informations

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "rights", "",
R"(Set the 'rights' to STRING in the Braille code.)",
          "STRING",
          "rights",
          fRights));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "composer", "",
R"(Set the 'composer' to STRING in the Braille code.)",
          "STRING",
          "composer",
          fComposer));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "arranger", "",
R"(Set the 'arranger' to STRING in the Braille code.)",
          "STRING",
          "arranger",
          fArranger));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "poet", "",
R"(Set the 'poet' to STRING in the Braille code.)",
          "STRING",
          "poet",
          fPoet));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "lyricist", "",
R"(Set the 'lyricist' to STRING in the Braille code.)",
          "STRING",
          "lyricist",
          fLyricist));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "software", "",
R"(Set the 'software' to STRING in the Braille code.)",
          "STRING",
          "software",
          fSoftware));

    // Braille informations

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
          "STRING",
          "dedication",
          fDedication));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "piece", "",
R"(Set 'piece' to STRING in the \header.)",
          "STRING",
          "piece",
          fPiece));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "opus", "",
R"(Set 'opus' to STRING in the \header.)",
          "STRING",
          "opus",
          fOpus));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "title", "",
R"(Set 'title' to STRING in the \header.)",
          "STRING",
          "title",
          fTitle));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
          "STRING",
          "subTitle",
          fSubTitle));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
          "STRING",
          "subSubTitle",
          fSubSubTitle));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
          "STRING",
          "instrument",
          fInstrument));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "meter", "",
R"(Set 'meter' to STRING in the \header.)",
          "STRING",
          "meter",
          fMeter));

    identificationSubGroup->
      appendOptionsItem (
        oahStringAtom::create (
          "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
          "STRING",
          "tagline",
          fTagline));

    identificationSubGroup->
      appendOptionsItem (
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
        oahSubGroup::kAlwaysShowDescription,
        this);

    appendSubGroup (notesSubGroup);

    notesSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "abs", "absolute",
R"(Generate Braille absolute note octaves.
By default, relative octaves are generated.)",
          "absoluteOctaves",
          fAbsoluteOctaves));

    notesSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "alldurs", "all-durations",
R"(Generate all Braille durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
          "allDurations",
          fAllDurations));

    notesSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for Braille to generate roman instead of arabic string numbers.)",
          "romanStringNumbers",
          fRomanStringNumbers));

    notesSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent Braille from using open strings.)",
          "avoidOpenStrings",
          fAvoidOpenStrings));

    notesSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "niln", "note-input-line-numbers",
          replaceSubstringInString (
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging EXECUTABLE.)",
            "EXECUTABLE",
            gGeneralOptions->fExecutableName),
          "noteInputLineNumbers",
          fInputLineNumbers));
  }


  // bars
  // --------------------------------------

  {
    // variables

    fShowAllBarNumbers = boolOptionsInitialValue;

    // options

    S_oahSubGroup
      barsSubGroup =
        oahSubGroup::create (
          "Bars",
          "hlpb", "help-braille-bars",
R"()",
        oahSubGroup::kAlwaysShowDescription,
        this);

    appendSubGroup (barsSubGroup);

    barsSubGroup->
      appendOptionsItem (
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

    // options

    S_oahSubGroup
      lineBreaksSubGroup =
        oahSubGroup::create (
          "Line breaks",
          "hlplb", "help-braille-line-breaks",
R"()",
        oahSubGroup::kAlwaysShowDescription,
        this);

    appendSubGroup (lineBreaksSubGroup);

    lineBreaksSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let Braille decide about them.)",
          "ignoreLineBreaks",
          fIgnoreLineBreaks));

    lineBreaksSubGroup->
      appendOptionsItem (
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

    // options

    S_oahSubGroup
      pageBreaksSubGroup =
        oahSubGroup::create (
          "Page breaks",
          "hlppb", "help-braille-page-breaks",
R"()",
        oahSubGroup::kAlwaysShowDescription,
        this);

    appendSubGroup (pageBreaksSubGroup);

    pageBreaksSubGroup->
      appendOptionsItem (
        oahBooleanAtom::create (
          "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let Braille decide about them.)",
          "ignorePageBreaks",
          fIgnorePageBreaks));
  }
        */
