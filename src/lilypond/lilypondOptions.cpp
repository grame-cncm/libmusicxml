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
#include "lpsrOptions.h"
#include "lilypondOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_optionsScoreOutputKindItem optionsScoreOutputKindItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsScoreOutputKindItemVariableDisplayName,
  lpsrScoreOutputKind&
                     optionsScoreOutputKindItemVariable)
{
  optionsScoreOutputKindItem* o = new
    optionsScoreOutputKindItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsScoreOutputKindItemVariableDisplayName,
      optionsScoreOutputKindItemVariable);
  assert(o!=0);
  return o;
}

optionsScoreOutputKindItem::optionsScoreOutputKindItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsScoreOutputKindItemVariableDisplayName,
  lpsrScoreOutputKind&
                     optionsScoreOutputKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsScoreOutputKindItemVariableDisplayName (
      optionsScoreOutputKindItemVariableDisplayName),
    fOptionsScoreOutputKindItemVariable (
      optionsScoreOutputKindItemVariable)
{}

optionsScoreOutputKindItem::~optionsScoreOutputKindItem ()
{}

void optionsScoreOutputKindItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsScoreOutputKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsScoreOutputKindItemVariableDisplayName" << " : " <<
    fOptionsScoreOutputKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsScoreOutputKindItemVariable" << " : \"" <<
    lpsrScoreOutputKindAsString (
      fOptionsScoreOutputKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsScoreOutputKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsScoreOutputKindItemVariableDisplayName <<
    " : \"" <<
    lpsrScoreOutputKindAsString (
      fOptionsScoreOutputKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsScoreOutputKindItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsAbsoluteOctaveEntryItem optionsAbsoluteOctaveEntryItem::create (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsAbsoluteOctaveEntryItemVariableDisplayName)
{
  optionsAbsoluteOctaveEntryItem* o = new
    optionsAbsoluteOctaveEntryItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsAbsoluteOctaveEntryItemVariableDisplayName);
  assert(o!=0);
  return o;
}

optionsAbsoluteOctaveEntryItem::optionsAbsoluteOctaveEntryItem (
  string optionsItemShortName,
  string optionsItemLongName,
  string optionsItemDescription,
  string optionsAbsoluteOctaveEntryItemVariableDisplayName)
  : optionsItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription),
    fOptionsAbsoluteOctaveEntryItemVariableDisplayName (
      optionsAbsoluteOctaveEntryItemVariableDisplayName)
{}

optionsAbsoluteOctaveEntryItem::~optionsAbsoluteOctaveEntryItem ()
{}

void optionsAbsoluteOctaveEntryItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "optionsAbsoluteOctaveEntryItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsAbsoluteOctaveEntryItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsAbsoluteOctaveEntryItemVariableDisplayName <<
    " : " <<
    "- no value here -" <<
    endl;
}

//______________________________________________________________________________
S_optionsRelativeOctaveEntryItem optionsRelativeOctaveEntryItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsRelativeOctaveEntryItemVariableDisplayName,
  S_msrSemiTonesPitchAndOctave&
          optionsRelativeOctaveEntryItemVariable)
{
  optionsRelativeOctaveEntryItem* o = new
    optionsRelativeOctaveEntryItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsRelativeOctaveEntryItemVariableDisplayName,
      optionsRelativeOctaveEntryItemVariable);
  assert(o!=0);
  return o;
}

optionsRelativeOctaveEntryItem::optionsRelativeOctaveEntryItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsRelativeOctaveEntryItemVariableDisplayName,
  S_msrSemiTonesPitchAndOctave&
          optionsRelativeOctaveEntryItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsRelativeOctaveEntryItemVariableDisplayName (
      optionsRelativeOctaveEntryItemVariableDisplayName),
    fOptionsRelativeOctaveEntryItemVariable (
      optionsRelativeOctaveEntryItemVariable)
{}

optionsRelativeOctaveEntryItem::~optionsRelativeOctaveEntryItem ()
{}

void optionsRelativeOctaveEntryItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "optionsRelativeOctaveEntryItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsRelativeOctaveEntryItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsRelativeOctaveEntryItemVariableDisplayName <<
    " : ";
  if (fOptionsRelativeOctaveEntryItemVariable) {
    os << endl;
    gIndenter++;
    os <<
      fOptionsRelativeOctaveEntryItemVariable;
    gIndenter--;
  }
  else {
    os << "none" << endl;
  }
}

ostream& operator<< (ostream& os, const S_optionsRelativeOctaveEntryItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsFixedOctaveEntryItem optionsFixedOctaveEntryItem::create (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsFixedOctaveEntryItemVariableDisplayName,
  S_msrSemiTonesPitchAndOctave&
          optionsFixedOctaveEntryItemVariable)
{
  optionsFixedOctaveEntryItem* o = new
    optionsFixedOctaveEntryItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsFixedOctaveEntryItemVariableDisplayName,
      optionsFixedOctaveEntryItemVariable);
  assert(o!=0);
  return o;
}

optionsFixedOctaveEntryItem::optionsFixedOctaveEntryItem (
  string  optionsItemShortName,
  string  optionsItemLongName,
  string  optionsItemDescription,
  string  optionsValueSpecification,
  string  optionsFixedOctaveEntryItemVariableDisplayName,
  S_msrSemiTonesPitchAndOctave&
          optionsFixedOctaveEntryItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsFixedOctaveEntryItemVariableDisplayName (
      optionsFixedOctaveEntryItemVariableDisplayName),
    fOptionsFixedOctaveEntryItemVariable (
      optionsFixedOctaveEntryItemVariable)
{}

optionsFixedOctaveEntryItem::~optionsFixedOctaveEntryItem ()
{}

void optionsFixedOctaveEntryItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "optionsFixedOctaveEntryItem:" <<
    endl;

  gIndenter++;

  optionsElement::printElementEssentials (
    os, fieldWidth);

  gIndenter++;
  os <<
    gIndenter.indentMultiLineString (
      fOptionsElementDescription) <<
    endl;
  gIndenter--;

  gIndenter--;
}

void optionsFixedOctaveEntryItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsFixedOctaveEntryItemVariableDisplayName <<
    " : ";
  if (fOptionsFixedOctaveEntryItemVariable) {
    os << endl;
    gIndenter++;
    os <<
      fOptionsFixedOctaveEntryItemVariable;
    gIndenter--;
  }
  else {
    os << "none" << endl;
  }
}

ostream& operator<< (ostream& os, const S_optionsFixedOctaveEntryItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsAccidentalStyleKindItem optionsAccidentalStyleKindItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleKindItemVariableDisplayName,
  lpsrAccidentalStyleKind&
                     optionsAccidentalStyleKindItemVariable)
{
  optionsAccidentalStyleKindItem* o = new
    optionsAccidentalStyleKindItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsAccidentalStyleKindItemVariableDisplayName,
      optionsAccidentalStyleKindItemVariable);
  assert(o!=0);
  return o;
}

optionsAccidentalStyleKindItem::optionsAccidentalStyleKindItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsAccidentalStyleKindItemVariableDisplayName,
  lpsrAccidentalStyleKind&
                     optionsAccidentalStyleKindItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsAccidentalStyleKindItemVariableDisplayName (
      optionsAccidentalStyleKindItemVariableDisplayName),
    fOptionsAccidentalStyleKindItemVariable (
      optionsAccidentalStyleKindItemVariable)
{}

optionsAccidentalStyleKindItem::~optionsAccidentalStyleKindItem ()
{}

void optionsAccidentalStyleKindItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsAccidentalStyleKindItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleKindItemVariableDisplayName" << " : " <<
    fOptionsAccidentalStyleKindItemVariableDisplayName <<
    endl <<
    setw (fieldWidth) <<
    "fOptionsAccidentalStyleKindItemVariable" << " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fOptionsAccidentalStyleKindItemVariable) <<
      "\"" <<
    endl;
}

void optionsAccidentalStyleKindItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsAccidentalStyleKindItemVariableDisplayName <<
    " : \"" <<
    lpsrAccidentalStyleKindAsString (
      fOptionsAccidentalStyleKindItemVariable) <<
    "\"" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsAccidentalStyleKindItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsChordsDisplayItem optionsChordsDisplayItem::create (
  string                optionsItemShortName,
  string                optionsItemLongName,
  string                optionsItemDescription,
  string                optionsValueSpecification,
  string                optionsChordsDisplayItemVariableDisplayName,
  list<pair<string, string> >&
                        optionsChordsDisplayItemVariable)
{
  optionsChordsDisplayItem* o = new
    optionsChordsDisplayItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsChordsDisplayItemVariableDisplayName,
      optionsChordsDisplayItemVariable);
  assert(o!=0);
  return o;
}

optionsChordsDisplayItem::optionsChordsDisplayItem (
  string                optionsItemShortName,
  string                optionsItemLongName,
  string                optionsItemDescription,
  string                optionsValueSpecification,
  string                optionsChordsDisplayItemVariableDisplayName,
  list<pair<string, string> >&
                        optionsChordsDisplayItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    foptionsChordsDisplayItemVariableDisplayName (
      optionsChordsDisplayItemVariableDisplayName),
    foptionsChordsDisplayItemVariable (
      optionsChordsDisplayItemVariable)
{}

optionsChordsDisplayItem::~optionsChordsDisplayItem ()
{}

void optionsChordsDisplayItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "optionsChordsDisplayItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "foptionsChordsDisplayItemVariableDisplayName" << " : " <<
    foptionsChordsDisplayItemVariableDisplayName <<
    setw (fieldWidth) <<
    "foptionsChordsDisplayItemVariable" << " : '" <<
    endl;

  gIndenter++;

  list<pair<string, string> >::const_iterator
    iBegin = foptionsChordsDisplayItemVariable.begin (),
    iEnd   = foptionsChordsDisplayItemVariable.end (),
    i      = iBegin;
  for ( ; ; ) {
    os <<
      (*i).first << " --> " << (*i).second <<
      endl;
    if (++i == iEnd) break;
// JMI    os << endl;
  } // for

  gIndenter--;
}

void optionsChordsDisplayItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    foptionsChordsDisplayItemVariableDisplayName <<
    " : ";

  if (foptionsChordsDisplayItemVariable.size ()) {
    os << endl;
    gIndenter++;

    list<pair<string, string> >::const_iterator
      iBegin = foptionsChordsDisplayItemVariable.begin (),
      iEnd   = foptionsChordsDisplayItemVariable.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i).first << " --> " << (*i).second;
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
      "none";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_optionsChordsDisplayItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_optionsMidiTempoItem optionsMidiTempoItem::create (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>& optionsMidiTempoItemVariable)
{
  optionsMidiTempoItem* o = new
    optionsMidiTempoItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification,
      optionsMidiTempoItemVariableDisplayName,
      optionsMidiTempoItemVariable);
  assert(o!=0);
  return o;
}

optionsMidiTempoItem::optionsMidiTempoItem (
  string             optionsItemShortName,
  string             optionsItemLongName,
  string             optionsItemDescription,
  string             optionsValueSpecification,
  string             optionsMidiTempoItemVariableDisplayName,
  pair<string, int>& optionsMidiTempoItemVariable)
  : optionsValuedItem (
      optionsItemShortName,
      optionsItemLongName,
      optionsItemDescription,
      optionsValueSpecification),
    fOptionsMidiTempoItemVariableDisplayName (
      optionsMidiTempoItemVariableDisplayName),
    fOptionsMidiTempoItemVariable (
      optionsMidiTempoItemVariable)
{}

optionsMidiTempoItem::~optionsMidiTempoItem ()
{}

void optionsMidiTempoItem::print (ostream& os) const
{
  const int fieldWidth = K_OPTIONS_FIELD_WIDTH;

  os <<
    "OptionsMidiTempoItem:" <<
    endl;

  gIndenter++;

  printValuedItemEssentials (
    os, fieldWidth);

  os << left <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariableDisplayName" << " : " <<
    fOptionsMidiTempoItemVariableDisplayName <<
    setw (fieldWidth) <<
    "fOptionsMidiTempoItemVariable" << " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

void optionsMidiTempoItem::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os << left <<
    setw (valueFieldWidth) <<
    fOptionsMidiTempoItemVariableDisplayName <<
    " : '" <<
    fOptionsMidiTempoItemVariable.first <<
    " = " <<
    fOptionsMidiTempoItemVariable.second <<
    "'" <<
    endl;
}

ostream& operator<< (ostream& os, const S_optionsMidiTempoItem& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_lilypondOptions gLilypondOptions;
S_lilypondOptions gLilypondOptionsUserChoices;
S_lilypondOptions gLilypondOptionsWithDetailedTrace;

S_lilypondOptions lilypondOptions::create (
  S_optionsHandler optionsHandler)
{
  lilypondOptions* o = new lilypondOptions (
    optionsHandler);
  assert(o!=0);
  return o;
}

lilypondOptions::lilypondOptions (
  S_optionsHandler optionsHandler)
  : optionsGroup (
    "LilyPond",
    "hlp", "help-lilypond",
R"(These options control which LilyPond code is generated.)",
    optionsHandler)
{
  // append this options group to the options handler
  // if relevant
  if (optionsHandler) {
    optionsHandler->
      appendOptionsGroupToHandler (this);
  }

  // initialize it
  initializeLilypondOptions (false);
}

lilypondOptions::~lilypondOptions ()
{}

void lilypondOptions::initializeIdentificationOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    identificationSubGroup =
      optionsSubGroup::create (
        "Identification",
        "hlpi", "help-lilypond-identification",
R"(  These options can be used to enforce values in the generated LilyPond code,
thus overriding the ones that may be present in the MSR data.)",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (identificationSubGroup);

  // MusicXML informations

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "rights", "",
R"(Set the 'rights' to STRING in the LilyPond code.)",
        "STRING",
        "rights",
        fRights));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "composer", "",
R"(Set the 'composer' to STRING in the LilyPond code.)",
        "STRING",
        "composer",
        fComposer));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "arranger", "",
R"(Set the 'arranger' to STRING in the LilyPond code.)",
        "STRING",
        "arranger",
        fArranger));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "poet", "",
R"(Set the 'poet' to STRING in the LilyPond code.)",
        "STRING",
        "poet",
        fPoet));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "lyricist", "",
R"(Set the 'lyricist' to STRING in the LilyPond code.)",
        "STRING",
        "lyricist",
        fLyricist));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "software", "",
R"(Set the 'software' to STRING in the LilyPond code.)",
        "STRING",
        "software",
        fSoftware));

  // LilyPond informations

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "dedication", "",
R"(Set 'dedication' to STRING in the \header.)",
        "STRING",
        "dedication",
        fDedication));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "piece", "",
R"(Set 'piece' to STRING in the \header.)",
        "STRING",
        "piece",
        fPiece));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "opus", "",
R"(Set 'opus' to STRING in the \header.)",
        "STRING",
        "opus",
        fOpus));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "title", "",
R"(Set 'title' to STRING in the \header.)",
        "STRING",
        "title",
        fTitle));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "stitle", "subtitle",
R"(Set 'subtitle' to STRING in the \header.)",
        "STRING",
        "subTitle",
        fSubTitle));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "sstitle", "subsubtitle",
R"(Set 'subsubtitle' to STRING in the \header.)",
        "STRING",
        "subSubTitle",
        fSubSubTitle));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "instrument", "",
R"(Set 'instrument' to STRING in the \header.)",
        "STRING",
        "instrument",
        fInstrument));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "meter", "",
R"(Set 'meter' to STRING in the \header.)",
        "STRING",
        "meter",
        fMeter));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "tagline", "",
R"(Set 'tagline' to STRING in the \header.)",
        "STRING",
        "tagline",
        fTagline));

  identificationSubGroup->
    appendOptionsItem (
      optionsStringItem::create (
        "copyright", "",
R"(Set 'copyright' to STRING in the \header.)",
        "STRING",
        "copyright",
        fCopyright));
}

void lilypondOptions::initializeEngraversOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    engraversSubGroup =
      optionsSubGroup::create (
        "Engravers",
        "hlpe", "help-lilypond-engravers",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (engraversSubGroup);

  // variables

  fAmbitusEngraver = boolOptionsInitialValue;

  engraversSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "ambitus", "",
R"(Generate an ambitus range at the beginning of the staves/voices.)",
        "ambitusEngraver",
        fAmbitusEngraver));
}

void lilypondOptions::initializeTimeOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    timeSubGroup =
      optionsSubGroup::create (
        "Time",
        "hlpti", "help-lilypond-time",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (timeSubGroup);

  // variables

  fNumericalTime = boolOptionsInitialValue;

  timeSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "numt", "numerical-time",
R"(Generate numerical time signatures, such as '4/4' instead of 'C'.)",
        "numericalTime",
        fNumericalTime));
}

void lilypondOptions::initializeNotesOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    notesSubGroup =
      optionsSubGroup::create (
        "Notes",
        "hlpn", "help-lilypond-notes",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (notesSubGroup);

  // octave entry

  lpsrOctaveEntryKind
    octaveEntryKindDefaultValue =
      kOctaveEntryRelative; // LilyPond default value
  fOctaveEntryKind = octaveEntryKindDefaultValue;

  // leave fOctaveEntrySemiTonesPitchAndOctave equal to nullptr here,
  // since \relative without a pitch and absolute octave
  // can be used in LilyPond, in which case the pitch and absolute actave is:
  fSemiTonesPitchAndOctaveDefaultValue =
    msrSemiTonesPitchAndOctave::create (
      // F under middle C, LilyPond default for relative octave entry
      kF_Natural_STP, 3);

  notesSubGroup->
    appendOptionsItem (
      optionsAbsoluteOctaveEntryItem::create (
        "abs", "asbolute",
        replaceSubstringInString (
R"(Use asbolute octave entry in the generated LilyPond code.
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          fSemiTonesPitchAndOctaveDefaultValue->asString ()),
          "optionsAbsoluteOctaveEntryItem"));

  notesSubGroup->
    appendOptionsItem (
      optionsRelativeOctaveEntryItem::create (
        "rel", "relative",
R"(Use relative octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -rel "c''")",
        "PITCH_AND_OCTAVE",
        "relativeOctaveEntrySemiTonesPitchAndOctave",
        fRelativeOctaveEntrySemiTonesPitchAndOctave));

  notesSubGroup->
    appendOptionsItem (
      optionsFixedOctaveEntryItem::create (
        "fixed", "",
R"(Use fixed octave entry reference PITCH_AND_OCTAVE in the generated LilyPond code.
PITCH_AND_OCTAVE is made of a diatonic pitch and an optional sequence of commas or single quotes.
It should be placed between double quotes if it contains single quotes, such as:
  -rel "c''")",
        "PITCH_AND_OCTAVE",
        "fixedOctaveEntrySemiTonesPitchAndOctave",
        fFixedOctaveEntrySemiTonesPitchAndOctave));

  // durations

  fAllDurations = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "alldurs", "all-durations",
R"(Generate all LilyPond durations.
By default, a duration equal to preceding one found in the current voice
is omitted for code conciseness.)",
        "allDurations",
        fAllDurations));

  // stems

  fStems = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "stems", "",
R"(Generate \stemUp and \stemDown LilyPond commands.
By default, LilyPond will take care of that by itself.)",
        "stems",
        fStems));

  // beaming

  fNoAutoBeaming  = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "noab", "no-auto-beaming",
R"(Generate '\set Voice.autoBeaming = ##f' in each voice
to prevent LilyPond from handling beams automatically.)",
        "noAutoBeaming",
        fNoAutoBeaming));

  // string numbers

  fRomanStringNumbers = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "rsn", "roman-string-numbers",
R"(Generate '\romanStringNumbers' in each voice
for LilyPond to generate roman instead of arabic string numbers.)",
        "romanStringNumbers",
        fRomanStringNumbers));

  // open strings

  fAvoidOpenStrings    = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "aos", "avoid-open-strings",
R"(Generate '\set TabStaff.restrainOpenStrings = ##t' in each voice
to prevent LilyPond from using open strings.)",
        "avoidOpenStrings",
        fAvoidOpenStrings));

  // accidentals

  const lpsrAccidentalStyleKind
    lpsrAccidentalStyleKindDefaultValue =
      kDefault; // default value

  fAccidentalStyleKind = lpsrAccidentalStyleKindDefaultValue;

  notesSubGroup->
    appendOptionsItem (
      optionsAccidentalStyleKindItem::create (
        "as", "accidental-style", // JMI
          replaceSubstringInString (
            replaceSubstringInString (
              replaceSubstringInString (
R"(The NUMBER LilyPond accidental styles available are:
ACCIDENTAL_STYLES.
The default is 'DEFAULT_VALUE'.)",
              "NUMBER",
              to_string (gLpsrAccidentalStyleKindsMap.size ())),
            "ACCIDENTAL_STYLES",
            existingLpsrAccidentalStyleKinds ()),
          "DEFAULT_VALUE",
          lpsrAccidentalStyleKindAsString (
            lpsrAccidentalStyleKindDefaultValue)),
        "STYLE",
        "accidentalStyleKind",
        fAccidentalStyleKind));

  // rest measures

  fCompressRestMeasures = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "crmeas", "compressRestMeasures", // JMI
R"(Generate '\compressMMRests' at the beginning of voices.
By default, this command is commented.)",
        "compressRestMeasures",
        fCompressRestMeasures));

  // input line numbers

  fNoteInputLineNumbers = boolOptionsInitialValue;

  notesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "niln", "note-input-line-numbers",
R"(Generate after each note and barline a comment containing
its MusicXML input line number.
This is useful when debugging xml2ly.)",
        "noteInputLineNumbers",
        fNoteInputLineNumbers));
}

void lilypondOptions::initializeBarsOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    barsSubGroup =
      optionsSubGroup::create (
        "Bars",
        "hlpb", "help-lilypond-bars",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (barsSubGroup);

  // bar numbers

  fShowAllBarNumbers = boolOptionsInitialValue;

  barsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "abn", "all-bar-numbers",
R"(Generate LilyPond code to show all bar numbers.)",
        "showAllBarNumbers",
        fShowAllBarNumbers));
}

void lilypondOptions::initializeLineBreaksOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    lineBreaksSubGroup =
      optionsSubGroup::create (
        "Line breaks",
        "hlplb", "help-lilypond-line-breaks",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (lineBreaksSubGroup);

  // lines

  fIgnoreLineBreaks = boolOptionsInitialValue;

  lineBreaksSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "ilb", "ignore-line-breaks",
R"(Ignore the line breaks from the MusicXML input
and let LilyPond decide about them.)",
        "ignoreLineBreaks",
        fIgnoreLineBreaks));

  fBreakLinesAtIncompleteRightMeasures = boolOptionsInitialValue;

  lineBreaksSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "blairm", "break-lines-at-incomplete-right-measures",
R"(Generate a '\break' command at the end of incomplete right measures
which is handy in popular folk dances and tunes.)",
        "breakLinesAtIncompleteRightMeasures",
        fBreakLinesAtIncompleteRightMeasures));

  fSeparatorLineEveryNMeasures         = -1;

  lineBreaksSubGroup->
    appendOptionsItem (
      optionsIntegerItem::create (
        "slenm", "separator-line-every-n-measures",
R"(Generate an additional separator line for readability every N measures,
where N is a positive integer.
Nothing special is done by default.)",
        "N",
        "separatorLineEveryNMeasures",
        fSeparatorLineEveryNMeasures));
}

void lilypondOptions::initializePageBreaksOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    pageBreaksSubGroup =
      optionsSubGroup::create (
        "Page breaks",
        "hlppb", "help-lilypond-page-breaks",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (pageBreaksSubGroup);

  // pages

  fIgnorePageBreaks = boolOptionsInitialValue;

  pageBreaksSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "ipb", "ignore-page-breaks",
R"(Ignore the page breaks from the MusicXML input
and let LilyPond decide about them.)",
        "ignorePageBreaks",
        fIgnorePageBreaks));
}

void lilypondOptions::initializeStavesOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    stavesSubGroup =
      optionsSubGroup::create (
        "Staves",
        "hlps", "helpLilypondStaves",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (stavesSubGroup);

  // tabs

  fModernTab = boolOptionsInitialValue;

  stavesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "mtab", "modernTab",
R"(Generate '\moderntab' instead of the default '\tab'.)",
        "modernTab",
        fModernTab));

  // staves

  fKeepStaffSize = boolOptionsInitialValue;

  stavesSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "kss", "keep-staff-size",
R"(Use the staff size value found in the MusicXML data.)",
        "keepStaffSize",
        fKeepStaffSize));
}

void lilypondOptions::initializeChordsOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    chordsSubGroup =
      optionsSubGroup::create (
        "Chords",
        "hlpc", "help-lilypond-chordss",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (chordsSubGroup);

  // arpeggios

  fConnectArpeggios = boolOptionsInitialValue;

  chordsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "conarp", "connect-arpeggios",
R"(Connect arpeggios across piano staves.)",
        "connectArpeggios",
        fConnectArpeggios));
}

void lilypondOptions::initializeTupletsOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    tupletsSubGroup =
      optionsSubGroup::create (
        "Tuplets",
        "hlpt", "help-lilypond-tuplets",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (tupletsSubGroup);

  // tuplets

  fIndentTuplets = boolOptionsInitialValue;

  tupletsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "itups", "indent-tuplets",
R"(Indent tuplets notes on a line of their own,
instead of keeping the on one and the same line.)",
        "indentTuplets",
        fIndentTuplets));
}

void lilypondOptions::initializeRepeatOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    repeatsSubGroup =
      optionsSubGroup::create (
        "Repeats",
        "hlpr", "help-lilypond-repeats",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (repeatsSubGroup);

  // repeats

  fKeepRepeatBarlines = boolOptionsInitialValue;

  repeatsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "krbs", "keep-repeat-barlines",
R"(Generate repeats start and and bar lines even though LilyPond would take care of them.)",
        "keepRepeatBarlines",
        fKeepRepeatBarlines));

  fRepeatBrackets = boolOptionsInitialValue;

  repeatsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "rbracks", "repeat-brackets",
R"(Generate repeats with brackets instead of regular bar lines.)",
        "repeatBrackets",
        fRepeatBrackets));

  fIgnoreRepeatNumbers = boolOptionsInitialValue;

  repeatsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "irn", "ignore-repeat-numbers",
R"(Ignore repeats numbers and let LilyPond determine them.)",
        "ignoreRepeatNumbers",
        fIgnoreRepeatNumbers));
}

void lilypondOptions::initializeOrnamentsOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    ornamentsSubGroup =
      optionsSubGroup::create (
        "Ornaments",
        "hlporns", "help-lilypond-ornaments",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (ornamentsSubGroup);

  // ornaments

  fDelayedOrnamentsFraction = rational (1, 2);

  string delayedOrnamentsFractionDefaultValue =
    to_string (fDelayedOrnamentsFraction.getNumerator ()) +
      "/" +
    to_string (fDelayedOrnamentsFraction.getDenominator ());

  ornamentsSubGroup->
    appendOptionsItem (
      optionsRationalItem::create (
        "dof", "delayed-ornaments-fraction",
        replaceSubstringInString (
R"(Place the delayed turn/reverseturn at the given fraction
between the ornemented note and the next one.
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          delayedOrnamentsFractionDefaultValue),
        "NUM/DENOM",
        "delayedOrnamentsFraction",
        fDelayedOrnamentsFraction));
}

void lilypondOptions::initializeChordsDisplayOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    chordsDiaplaySubGroup =
      optionsSubGroup::create (
        "Chords display",
        "hchd", "help-chords-display",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (chordsDiaplaySubGroup);

  // chords

  chordsDiaplaySubGroup->
    appendOptionsItem (
      optionsChordsDisplayItem::create (
        "chd", "chords-display",
/*
Including a closing parenthesis in a raw string:
  choose ### as the delimiter so only )###" ends the string:
    R"###(
        Some Text)"
    )###";
*/
R"###(Use SPECIFICATION to display chords using LilyPond's chordNameExceptions.
SPECIFICATION should contain a chord contents such as '<c ees ges bes>',
followed by code to display it, for example:
  '<c ees ges bes> \\super {"-7(" {\\small\\raise #0.5 \\flat} "5)"}'.
The LilyPond code has to escape backslashed, thus use '\\' to obtain '\'.
These two elements are passed over to LilyPond verbatim, without any check.
This option can be used any number of times.)###",
        "SPECIFICATION",
        "chordsDisplayList",
        fChordsDisplayList));

  fJazzChordsDisplay = boolOptionsInitialValue;

  fJazzChordsDisplayLilypondcode =
R"###(  <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
  <c e g bes>1-\markup { \super "7" }
  <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
  <c f g bes>1-\markup { \super {"7(sus4)"} }
  <c e g a d'>1-\markup { \super "6/9" }
  <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
  <c f g bes d'>1-\markup { \super {"9(sus4)"} }
  <c e g bes d'>1-\markup { \super "9" }
  <c e g b d'>1-\markup { \super "maj9" }
  <c e gis bes d'>1-\markup { \super "9+" }
  <c e g bes d' fis'>1-\markup { \super "9#11" }
  <c e g bes d' f'>1-\markup { \super "11" }
  <c e g bes d' a'>1-\markup { \super "13" }
  <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
  <c e g a d'>1-\markup { \super "6(add9)" })###";

  chordsDiaplaySubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "jchd", "jazz-chords-display",
        replaceSubstringInString (
R"(Display the chords using LilyPond's chordNameExceptions
and a set of standard specifications to display them, i.e.:
LILYPOND_CODE)",
          "LILYPOND_CODE",
          fJazzChordsDisplayLilypondcode),
        "jazzChordsDisplay",
        fJazzChordsDisplay));
}

void lilypondOptions::initializeFontsOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    fontsSubGroup =
      optionsSubGroup::create (
        "Fonts",
        "hfonts", "help-fonts",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (fontsSubGroup);

  // fonts

  fJazzFonts = boolOptionsInitialValue;

  fontsSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "jazz", "jazz-fonts",
R"(Use the LilyJazz font for notes, braces, text and chords.
It can be downloaded from https://github.com/OpenLilyPondFonts/lilyjazz.
This font should be installed so that LilyPond can use it, see:
https://github.com/OpenLilyPondFonts/lilyjazz/blob/master/LilyPond-Fonts-Installation-And-Usage.txt.)",
        "jazzFonts",
        fJazzFonts));
}

void lilypondOptions::initializeCodeGenerationOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    codeGenerationSubGroup =
      optionsSubGroup::create (
        "Code generation",
        "hlpcg", "help-lilypond-code-generation",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (codeGenerationSubGroup);

  // infos

  fXml2lyInfos = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "xi", "xml2ly-infos",
R"(Generate initial comments showing the compilation date and options.)",
        "xml2lyInfos",
        fXml2lyInfos));

  // code generation

  fComments = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "com", "comments",
R"(Generate comments showing the structure of the score
such as '% part P_POne (P1).)",
        "comments",
        fComments));

  fGlobal = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "global", "",
R"(Generate a 'global' empty variable and place a use of it
at the beginning of all voices.)",
        "global",
        fGlobal));

  fDisplayMusic = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "dm", "display-music",
R"(Place the contents of all voices inside a '\displayMusic' block,
for LilyPond to show its internal representation of the music.)",
        "displayMusic",
        fDisplayMusic));

  fNoLilypondCode = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "nolpc", "no-lilypond-code",
R"(Don't generate any LilyPond code.
That can be useful if only a summary of the score is needed.)",
        "noLilypondCode",
        fNoLilypondCode));

  fNoLilypondLyrics = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "nolpl", "no-lilypond-lyrics",
R"(Don't generate any lyrics in the LilyPond code.)",
        "noLilypondLyrics",
        fNoLilypondLyrics));

  fLilypondCompileDate = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "lpcd", "lilypond-compile-date",
R"(Generate code to include the compilation date
when LilyPond creates the score.)",
        "lilypondCompileDate",
        fLilypondCompileDate));

  fPointAndClickOff = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "paco", "point-and-clic-off",
R"(Generate \pointAndClickOff at the beginning of the LilyPond code
to reduce the size of the resulting PDF file.)",
        "pointAndClickOff",
        fPointAndClickOff));


  fWhiteNoteHeads = boolOptionsInitialValue;

  codeGenerationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "wnh", "white-note-heads",
R"(Generate Scheme function 'whiteNoteHeads'
at the beginning of the LilyPond code.)",
        "whiteNoteHeads",
        fWhiteNoteHeads));
}

void lilypondOptions::initializeScoreNotationOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    scoreNotationSubGroup =
      optionsSubGroup::create (
        "Score notation",
        "hlpsn", "help-lilypond-score-notation",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (scoreNotationSubGroup);

  // score notation

//  fScoreNotationKind =  lilypondOptions::kWesternNotation;
    // default value

  fJianpu = false;

  scoreNotationSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "jianpu", "",
R"(Generate the score using jianpu (numbered) notation
instead of the default western notation.
That option needs lilypond-Jianpu to be accessible to LilyPond
(https://github.com/nybbs2003/lilypond-Jianpu/jianpu10a.ly).)",
        "jianpu",
        fJianpu));
}

void lilypondOptions::initializeMidiOptions (
  bool boolOptionsInitialValue)
{
  S_optionsSubGroup
    midiSubGroup =
      optionsSubGroup::create (
        "Midi",
        "hlpm", "help-lilypond-midi",
R"()",
      optionsSubGroup::kAlwaysShowDescription,
      this);

  appendOptionsSubGroup (midiSubGroup);

  // MIDI

  string midiTempoDuration  = "4";
  int    midiTempoPerSecond = 90;

  fMidiTempo.first  = midiTempoDuration;
  fMidiTempo.second = midiTempoPerSecond;

  string midiTempoDefaultValue =
    midiTempoDuration + " = " + to_string (midiTempoPerSecond);

  midiSubGroup->
    appendOptionsItem (
      optionsMidiTempoItem::create (
        "midi-tempo", "",
        replaceSubstringInString (
R"(Generate a '\tempo' command in the \midi block.
MIDI_TEMPO_SPEC can be:
'DURATION = PER_SECOND'
or
"DURATION = PER_SECOND" .
DURATION is a string such as '8.', and PER_SECOND is an integer.
The single or double quotes are used to allow spaces around the '=' sign,
otherwise they can be dispensed with.
Using double quotes allows for shell variables substitutions, as in:
PER_SECOND=66
xml2ly -midiTempo "8. ${PER_SECOND}" .
The default is 'DEFAULT_VALUE'.)",
          "DEFAULT_VALUE",
          midiTempoDefaultValue),
        "MIDI_TEMPO_SPEC",
        "midiTempo",
        fMidiTempo));

  // variables

  fNoMidi = boolOptionsInitialValue;

  midiSubGroup->
    appendOptionsItem (
      optionsBooleanItem::create (
        "no-midi", "",
R"(Generate the '\midi' block as a comment instead of active code.)",
        "noMidi",
        fNoMidi));
}

void lilypondOptions::initializeLilypondOptions (
  bool boolOptionsInitialValue)
{
  // identification
  // --------------------------------------
  initializeIdentificationOptions (
    boolOptionsInitialValue);

  // engravers
  // --------------------------------------
  initializeEngraversOptions (
    boolOptionsInitialValue);

  // time
  // --------------------------------------
  initializeTimeOptions (
    boolOptionsInitialValue);

  // notes
  // --------------------------------------
  initializeNotesOptions (
    boolOptionsInitialValue);

  // bars
  // --------------------------------------
  initializeBarsOptions (
    boolOptionsInitialValue);

  // line breaks
  // --------------------------------------
  initializeLineBreaksOptions (
    boolOptionsInitialValue);

  // page breaks
  // --------------------------------------
  initializePageBreaksOptions (
    boolOptionsInitialValue);

  // staves
  // --------------------------------------
  initializeStavesOptions (
    boolOptionsInitialValue);

  // chords
  // --------------------------------------
  initializeChordsOptions (
    boolOptionsInitialValue);

  // tuplets
  // --------------------------------------
  initializeTupletsOptions (
    boolOptionsInitialValue);

  // repeats
  // --------------------------------------
  initializeRepeatOptions (
    boolOptionsInitialValue);

  // ornaments
  // --------------------------------------
  initializeOrnamentsOptions (
    boolOptionsInitialValue);

  // chords display
  // --------------------------------------
  initializeChordsDisplayOptions (
    boolOptionsInitialValue);

  // fonts
  // --------------------------------------
  initializeFontsOptions (
    boolOptionsInitialValue);

  // code generation
  // --------------------------------------
  initializeCodeGenerationOptions (
    boolOptionsInitialValue);

  // score notation
  // --------------------------------------
  initializeScoreNotationOptions (
    boolOptionsInitialValue);

  // midi
  // --------------------------------------
  initializeMidiOptions (
    boolOptionsInitialValue);
}

S_lilypondOptions lilypondOptions::createCloneWithDetailedTrace ()
{
  S_lilypondOptions
    clone =
      lilypondOptions::create (0);
      // 0 not to have it inserted twice in the option handler

  // set the options handler uplink
  clone->
    setOptionsHandlerUplink (
      fOptionsHandlerUplink);


  // identification
  // --------------------------------------

  // MusicXML informations
  clone->fRights =
    fRights;
  clone->fComposer =
    fComposer;
  clone->fArranger =
    fArranger;
  clone->fPoet =
    fPoet;
  clone->fLyricist =
    fLyricist;
  clone->fSoftware =
    fSoftware;

  // LilyPond informations
  clone->fDedication =
    fDedication;
  clone->fPiece =
    fPiece;
  clone->fOpus =
    fOpus;
  clone->fTitle =
    fTitle;
  clone->fSubTitle =
    fSubTitle;
  clone->fSubSubTitle =
    fSubSubTitle;
  clone->fInstrument =
    fInstrument;
  clone->fMeter =
    fMeter;
  clone->fTagline =
    fTagline;
  clone->fCopyright =
    fCopyright;


  // engravers
  // --------------------------------------

  clone->fAmbitusEngraver =
    fAmbitusEngraver;


  // time
  // --------------------------------------

  clone->fNumericalTime =
    fNumericalTime;


  // notes
  // --------------------------------------

  clone->fOctaveEntryKind =
    fOctaveEntryKind;

  clone->fAllDurations =
    fAllDurations;

  clone->fStems =
    fStems;
  clone->fNoAutoBeaming =
    fNoAutoBeaming;

  clone->fRomanStringNumbers =
    fRomanStringNumbers;
  clone->fAvoidOpenStrings =
    fAvoidOpenStrings;

  clone->fAccidentalStyleKind =
    fAccidentalStyleKind;

  clone->fCompressRestMeasures =
    fCompressRestMeasures;

  clone->fNoteInputLineNumbers =
    true;


  // bars
  // --------------------------------------

  clone->fShowAllBarNumbers =
    true;


  // line breaks
  // --------------------------------------

  clone->fIgnoreLineBreaks =
    fIgnoreLineBreaks;

  clone->fBreakLinesAtIncompleteRightMeasures =
    fBreakLinesAtIncompleteRightMeasures;

  clone->fSeparatorLineEveryNMeasures =
    fSeparatorLineEveryNMeasures;


  // page breaks
  // --------------------------------------

  clone->fIgnorePageBreaks =
    fIgnorePageBreaks;


  // staves
  // --------------------------------------

  clone->fModernTab =
    fModernTab;


  // chords
  // --------------------------------------

  clone->fConnectArpeggios =
    fConnectArpeggios;


  // tuplets
  // --------------------------------------

  clone->fIndentTuplets =
    fIndentTuplets;


  // repeats
  // --------------------------------------

  clone->fKeepRepeatBarlines =
    fKeepRepeatBarlines;
  clone->fRepeatBrackets =
    fRepeatBrackets;
  clone->fIgnoreRepeatNumbers =
    fIgnoreRepeatNumbers;


  // ornaments
  // --------------------------------------

  clone->fDelayedOrnamentsFraction =
    fDelayedOrnamentsFraction;


  // chords display
  // --------------------------------------

  clone->fJazzChordsDisplay =
    fJazzChordsDisplay;
  clone->fJazzChordsDisplayLilypondcode =
    fJazzChordsDisplayLilypondcode;


  // fonts
  // --------------------------------------

  clone->fJazzFonts =
    fJazzFonts;


  // code generation
  // --------------------------------------

  clone->fXml2lyInfos =
    fXml2lyInfos;

  clone->fComments =
    fComments;

  clone->fGlobal =
    fGlobal;

  clone->fDisplayMusic =
    fDisplayMusic;

  clone->fNoLilypondCode =
    fNoLilypondCode;

  clone->fNoLilypondLyrics =
    fNoLilypondLyrics;

  clone->fLilypondCompileDate =
    fLilypondCompileDate;

  clone->fPointAndClickOff =
    fPointAndClickOff;

  clone->fWhiteNoteHeads =
    fWhiteNoteHeads;


  // score notation
  // --------------------------------------

  clone->fJianpu =
    fJianpu;


  // midi
  // --------------------------------------

  clone->fMidiTempo =
    fMidiTempo;

  clone->fNoMidi =
    fNoMidi;

// JMI ???

  clone->fKeepStaffSize =
    fKeepStaffSize;

  return clone;
}

/* JMI
bool lilypondOptions::setAccidentalStyleKind (lpsrAccidentalStyleKind accidentalStyleKind)
{
  // is accidentalStyleKind in the accidental styles set?
  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gLpsrAccidentalStyleKindsMap.find (accidentalStyleKind);

  if (it == gLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidentalStyleKind is unknown
    return false;
  }

  fAccidentalStyleKind = it.second;

  return true;
}
*/

//______________________________________________________________________________
void lilypondOptions::enforceQuietness ()
{}

//______________________________________________________________________________
void lilypondOptions::checkOptionsConsistency ()
{
  // JMI
}

//______________________________________________________________________________
string lilypondOptions::scoreNotationKindAsString (
  scoreNotationKind notationKind)
{
  string result;

  switch (notationKind) {
    case lilypondOptions::kWesternNotation:
      result = "western";
      break;
    case lilypondOptions::kJianpuNotation:
      result = "jianpu";
      break;
    case lilypondOptions::kABCNotation:
      result = "abc";
      break;
  } // switch

  return result;
}

void lilypondOptions::printOptionsValues (
  ostream& os,
  int      valueFieldWidth) const
{
  os <<
    "The LilyPond options are:" <<
    endl;

  gIndenter++;

  // identification
  // --------------------------------------
  os <<
    "Identification:" <<
    endl;

  gIndenter++;

  // MusicXML informations
  os << left <<
    setw (valueFieldWidth) << "rights" << " : " <<
      fRights <<
      endl <<

    setw (valueFieldWidth) << "composer" << " : " <<
      fComposer <<
      endl <<
    setw (valueFieldWidth) << "arranger" << " : " <<
      fArranger <<
      endl <<
    setw (valueFieldWidth) << "poet" << " : " <<
      fPoet <<
      endl <<
    setw (valueFieldWidth) << "lyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (valueFieldWidth) << "software" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations
    setw (valueFieldWidth) << "dedication" << " : " <<
      fDedication <<
      endl <<

    setw (valueFieldWidth) << "piece" << " : " <<
      fPiece <<
      endl <<
    setw (valueFieldWidth) << "opus" << " : " <<
      fOpus <<
      endl <<

    setw (valueFieldWidth) << "title" << " : " <<
      fTitle <<
      endl <<
    setw (valueFieldWidth) << "subTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (valueFieldWidth) << "subSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<

    setw (valueFieldWidth) << "instrument" << " : " <<
      fInstrument <<
      endl <<

    setw (valueFieldWidth) << "meter" << " : " <<
      fMeter <<
      endl <<

    setw (valueFieldWidth) << "tagline" << " : " <<
      fTagline <<
      endl <<

    setw (valueFieldWidth) << "copyright" << " : " <<
      fCopyright <<
      endl;

  gIndenter--;


  // engravers
  // --------------------------------------
  os <<
    "Engravers:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl;

  gIndenter--;


  // time
  // --------------------------------------
  os <<
    "Time:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  gIndenter--;


  // notes
  // --------------------------------------
  os <<
    "Notes:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "octaveEntryKind" << " : " <<
    lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
    endl;

  switch (gLilypondOptions->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      if (fRelativeOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) << "relativeOctaveEntrySemiTonesPitchAndOctave" << " : " <<
          fRelativeOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
    case kOctaveEntryAbsolute:
      break;
    case kOctaveEntryFixed:
      if (fFixedOctaveEntrySemiTonesPitchAndOctave) {
        os << left <<
          setw (valueFieldWidth) << "fixedOctaveEntrySemiTonesPitchAndOctave" << " : " <<
          fFixedOctaveEntrySemiTonesPitchAndOctave->asString () <<
          endl;
      }
      break;
  } // switch

  os << left <<
    setw (valueFieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (valueFieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<
    setw (valueFieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<

    setw (valueFieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
    setw (valueFieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<

    setw (valueFieldWidth) << "accidentalStyleKind" << " : " <<
      fAccidentalStyleKind <<
      endl <<

    setw (valueFieldWidth) << "compressRestMeasures" << " : " <<
      booleanAsString (fCompressRestMeasures) <<
      endl <<

    setw (valueFieldWidth) << "noteInputLineNumbers" << " : " <<
      booleanAsString (fNoteInputLineNumbers) <<
      endl;

  gIndenter--;


  // bars
  // --------------------------------------
  os <<
    "Bars:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gIndenter--;


  // line breaks
  // --------------------------------------
  os <<
    "Line breaks:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ignoreLineBreaks" << " : " <<
      booleanAsString (fIgnoreLineBreaks) <<
      endl <<

    setw (valueFieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (valueFieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  gIndenter--;


  // page breaks
  // --------------------------------------
  os <<
    "Page breaks:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "ignorePageBreaks" << " : " <<
    booleanAsString (fIgnorePageBreaks) <<
    endl;

  gIndenter--;


  // staves
  // --------------------------------------
  os <<
    "Staves:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl <<
    setw (valueFieldWidth) << "keepStaffSize" << " : " <<
    booleanAsString (fKeepStaffSize) <<
    endl;

  gIndenter--;


  // chords
  // --------------------------------------

  os <<
    "Chords:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  gIndenter--;


  // tuplets
  // --------------------------------------

  os <<
    "Tuplets:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  gIndenter--;


  // repeats
  // --------------------------------------

  os <<
    "Repeats:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "keepRepeatBarlines" << " : " <<
    booleanAsString (fKeepRepeatBarlines) <<
    endl <<
    setw (valueFieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl <<
    setw (valueFieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl;

  gIndenter--;


  // ornaments
  // --------------------------------------

  os <<
    "Ornaments:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  gIndenter--;


  // chords display
  // --------------------------------------

  os <<
    "Chords display:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "chordsDisplayList" << " : ";

  if (fChordsDisplayList.size ()) {
    os << endl;

    gIndenter++;

    list<pair<string, string> >::const_iterator
      iBegin = fChordsDisplayList.begin (),
      iEnd   = fChordsDisplayList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        gTab <<
        (*i).first <<
        " -> " <<
        (*i).second <<
        endl;
      if (++i == iEnd) break;
  //     os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  os << left <<
    setw (valueFieldWidth) << "jazzChordsDisplay" << " : " <<
    booleanAsString (fJazzChordsDisplay) <<
    endl <<
    setw (valueFieldWidth) << "jazzChordsDisplayLilypondcode" << " : " <<
    fJazzChordsDisplayLilypondcode <<
    endl;

  gIndenter--;


  // fonts
  // --------------------------------------

  os <<
    "Fonts:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  gIndenter--;


  // code generation
  // --------------------------------------

  os <<
    "LilyPond code generation:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (valueFieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<

    setw (valueFieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<

    setw (valueFieldWidth) << "displayMusic" << " : " <<
      booleanAsString (fDisplayMusic) <<
      endl <<

    setw (valueFieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    setw (valueFieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    setw (valueFieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl <<

    setw (valueFieldWidth) << "pointAndClickOff" << " : " <<
      booleanAsString (fPointAndClickOff) <<
      endl <<

    setw (valueFieldWidth) << "whiteNoteHeads" << " : " <<
      booleanAsString (fWhiteNoteHeads) <<
      endl;

  gIndenter--;


  // score notation
  // --------------------------------------

  os <<
    "Score notation:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  gIndenter--;


  // midi
  // --------------------------------------
  os <<
    "Midi:" <<
    endl;

  gIndenter++;

  os << left <<
    setw (valueFieldWidth) << "midiTempo" << " : " <<
      fMidiTempo.first << " = " << fMidiTempo.second <<
      endl <<

    setw (valueFieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  gIndenter--;
}

void lilypondOptions::printLilypondOptionsValues (int fieldWidth)
{
  glogIOstream <<
    "The LilyPond options are:" <<
    endl;

  gIndenter++;

  // identification
  // --------------------------------------
  glogIOstream <<
    "Identification:" <<
    endl;

  gIndenter++;

  // MusicXML informations
  glogIOstream << left <<
    setw (fieldWidth) << "rights" << " : " <<
      fRights <<
      endl <<

    setw (fieldWidth) << "composer" << " : " <<
      fComposer <<
      endl <<
    setw (fieldWidth) << "arranger" << " : " <<
      fArranger <<
      endl <<
    setw (fieldWidth) << "poet" << " : " <<
      fPoet <<
      endl <<
    setw (fieldWidth) << "lyricist" << " : " <<
      fLyricist <<
      endl <<

    setw (fieldWidth) << "software" << " : " <<
      fSoftware <<
      endl <<

  // LilyPond informations
    setw (fieldWidth) << "dedication" << " : " <<
      fDedication <<
      endl <<

    setw (fieldWidth) << "piece" << " : " <<
      fPiece <<
      endl <<
    setw (fieldWidth) << "opus" << " : " <<
      fOpus <<
      endl <<

    setw (fieldWidth) << "title" << " : " <<
      fTitle <<
      endl <<
    setw (fieldWidth) << "subTitle" << " : " <<
      fSubTitle <<
      endl <<
    setw (fieldWidth) << "subSubTitle" << " : " <<
      fSubSubTitle <<
      endl <<
    setw (fieldWidth) << "instrument" << " : " <<
      fInstrument <<
      endl <<
    setw (fieldWidth) << "meter" << " : " <<
      fMeter <<
      endl <<

    setw (fieldWidth) << "tagline" << " : " <<
      fTagline <<
      endl <<

    setw (fieldWidth) << "copyright" << " : " <<
      fCopyright <<
      endl;

  gIndenter--;


  // engravers
  // --------------------------------------
  glogIOstream <<
    "Engravers:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "ambitusEngraver" << " : " <<
      booleanAsString (fAmbitusEngraver) <<
      endl;

  gIndenter--;


  // time
  // --------------------------------------
  glogIOstream <<
    "Time:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "numericalTime" << " : " <<
      booleanAsString (fNumericalTime) <<
      endl;

  gIndenter--;


  // notes
  // --------------------------------------
  glogIOstream <<
    "Notes:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "octaveEntryKind" << " : " <<
      lpsrOctaveEntryKindAsString (fOctaveEntryKind) <<
      endl <<

    setw (fieldWidth) << "allDurations" << " : " <<
      booleanAsString (fAllDurations) <<
      endl <<

    setw (fieldWidth) << "stems" << " : " <<
      booleanAsString (fStems) <<
      endl <<
    setw (fieldWidth) << "noAutoBeaming" << " : " <<
      booleanAsString (fNoAutoBeaming) <<
      endl <<

    setw (fieldWidth) << "romanStringNumbers" << " : " <<
      booleanAsString (fRomanStringNumbers) <<
      endl <<
    setw (fieldWidth) << "avoidOpenString" << " : " <<
      booleanAsString (fAvoidOpenStrings) <<
      endl <<

    setw (fieldWidth) << "accidentalStyleKind" << " : " <<
      fAccidentalStyleKind <<
      endl <<

    setw (fieldWidth) << "compressRestMeasures" << " : " <<
      booleanAsString (fCompressRestMeasures) <<
      endl <<

    setw (fieldWidth) << "inputLineNumbers" << " : " <<
      booleanAsString (fNoteInputLineNumbers) <<
      endl;

  gIndenter--;


  // bars
  // --------------------------------------
  glogIOstream <<
    "Bars:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "showAllBarNumbers" << " : " <<
    booleanAsString (fShowAllBarNumbers) <<
    endl;

  gIndenter--;


  // line breaks
  // --------------------------------------
  glogIOstream <<
    "Line breaks:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "ignoreLineBreaks" << " : " <<
      booleanAsString (fIgnoreLineBreaks) <<
      endl <<

    setw (fieldWidth) << "breakLinesAtIncompleteRightMeasures" << " : " <<
      booleanAsString (fBreakLinesAtIncompleteRightMeasures) <<
      endl <<

    setw (fieldWidth) << "separatorLineEveryNMeasures" << " : " <<
      booleanAsString (fSeparatorLineEveryNMeasures) <<
      endl;

  gIndenter--;


  // page breaks
  // --------------------------------------
  glogIOstream <<
    "Page breaks:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "ignorePageBreaks" << " : " <<
    booleanAsString (fIgnorePageBreaks) <<
    endl;

  gIndenter--;


  // staves
  // --------------------------------------
  glogIOstream <<
    "Staves:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "modernTab" << " : " <<
    booleanAsString (fModernTab) <<
    endl;

  gIndenter--;


  // chords
  // --------------------------------------

  glogIOstream <<
    "Chords:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "connectArpeggios" << " : " <<
    booleanAsString (fConnectArpeggios) <<
    endl;

  gIndenter--;


  // tuplets
  // --------------------------------------

  glogIOstream <<
    "Tuplets:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "indentTuplets" << " : " <<
    booleanAsString (fIndentTuplets) <<
    endl;

  gIndenter--;


  // repeats
  // --------------------------------------

  glogIOstream <<
    "Repeats:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "keepRepeatBarlines" << " : " <<
    booleanAsString (fKeepRepeatBarlines) <<
    endl <<
    setw (fieldWidth) << "repeatBrackets" << " : " <<
    booleanAsString (fRepeatBrackets) <<
    endl <<
    setw (fieldWidth) << "ignoreRepeatNumbers" << " : " <<
    booleanAsString (fIgnoreRepeatNumbers) <<
    endl;

  gIndenter--;


  // ornaments
  // --------------------------------------

  glogIOstream <<
    "Ornaments:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "fDelayedOrnamentsFraction" << " : " <<
    fDelayedOrnamentsFraction <<
    endl;

  gIndenter--;


  // chords display
  // --------------------------------------

  glogIOstream <<
    "Chords display:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "jazzChordsDisplay" << " : " <<
    booleanAsString (fJazzChordsDisplay) <<
    endl <<
    setw (fieldWidth) << "jazzChordsDisplayLilypondcode" << " : " <<
    fJazzChordsDisplayLilypondcode <<
    endl;

  gIndenter--;


  // fonts
  // --------------------------------------

  glogIOstream <<
    "Fonts:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "jazzFonts" << " : " <<
    booleanAsString (fJazzFonts) <<
    endl;

  gIndenter--;


  // code generation
  // --------------------------------------

  glogIOstream <<
    "LilyPond code generation:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "xml2lyInfos" << " : " <<
      booleanAsString (fXml2lyInfos) <<
      endl <<

    setw (fieldWidth) << "comments" << " : " <<
      booleanAsString (fComments) <<
      endl <<

    setw (fieldWidth) << "global" << " : " <<
      booleanAsString (fGlobal) <<
      endl <<

    setw (fieldWidth) << "displayMusic" << " : " <<
      booleanAsString (fDisplayMusic) <<
      endl <<

    setw (fieldWidth) << "noLilypondCode" << " : " <<
      booleanAsString (fNoLilypondCode) <<
      endl <<

    setw (fieldWidth) << "noLilypondLyrics" << " : " <<
      booleanAsString (fNoLilypondLyrics) <<
      endl <<

    setw (fieldWidth) << "lilypondCompileDate" << " : " <<
      booleanAsString (fLilypondCompileDate) <<
      endl <<

    setw (fieldWidth) << "pointAndClickOff" << " : " <<
      booleanAsString (fPointAndClickOff) <<
      endl <<

    setw (fieldWidth) << "whiteNoteHeads" << " : " <<
      booleanAsString (fWhiteNoteHeads) <<
      endl;

  gIndenter--;


  // score notation
  // --------------------------------------

  glogIOstream <<
    "Score notation:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "fJianpu" << " : " <<
      booleanAsString (fJianpu) <<
      endl;

  gIndenter--;


  // midi
  // --------------------------------------
  glogIOstream <<
    "Midi:" <<
    endl;

  gIndenter++;

  glogIOstream << left <<
    setw (fieldWidth) << "fMidiTempo" << " : " <<
      fMidiTempo.first << " = " << fMidiTempo.second <<
      endl <<

    setw (fieldWidth) << "noMidi" << " : " <<
      booleanAsString (fNoMidi) <<
      endl;

  gIndenter--;

  gIndenter--;
}

S_optionsItem lilypondOptions::handleOptionsItem (
  ostream&      os,
  S_optionsItem item)
{
  S_optionsItem result;

  if (
    // absolute octave entry item?
    S_optionsAbsoluteOctaveEntryItem
      absoluteOctaveEntryKindItem =
        dynamic_cast<optionsAbsoluteOctaveEntryItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsAbsoluteOctaveEntryItem'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryAbsolute;
  }

  else if (
    // relative octave entry item?
    S_optionsRelativeOctaveEntryItem
      relativeOctaveEntryItem =
        dynamic_cast<optionsRelativeOctaveEntryItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsRelativeOctaveEntryItem'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryRelative;

    // wait until the value is met
    result = relativeOctaveEntryItem;
  }

  else if (
    // fixed octave entry item?
    S_optionsFixedOctaveEntryItem
      fixedOctaveEntryItem =
        dynamic_cast<optionsFixedOctaveEntryItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsFixedOctaveEntryItem'" <<
        endl;
    }
#endif

    // set octave entry kind it at once
    fOctaveEntryKind = kOctaveEntryFixed;

    // wait until the value is met
    result = fixedOctaveEntryItem;
  }

  else if (
    // acccidentals style item?
    S_optionsAccidentalStyleKindItem
      accidentalStyleKindItem =
        dynamic_cast<optionsAccidentalStyleKindItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsAccidentalStyleKindItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = accidentalStyleKindItem;
  }

  else if (
    // chords display item?
    S_optionsChordsDisplayItem
      chordsDisplayItem =
        dynamic_cast<optionsChordsDisplayItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsChordsDisplayItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = chordsDisplayItem;
  }

  else if (
    // midi tempo item?
    S_optionsMidiTempoItem
      midiTempoItem =
        dynamic_cast<optionsMidiTempoItem*>(&(*item))
    ) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceOptions) {
      os <<
        "==> optionsItem is of type 'optionsMidiTempoItem'" <<
        endl;
    }
#endif

    // wait until the value is met
    result = midiTempoItem;
  }

  return result;
}

void lilypondOptions::handleOptionsRelativeOctaveEntryItemValue (
  ostream&      os,
  S_optionsRelativeOctaveEntryItem
                relativeOctaveEntryItem,
  string        theString)
{
  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsRelativeOctaveEntryItem'" <<
      endl;
  }
#endif

/* JMI
  map<string, lpsrOctaveEntryKind>::const_iterator
    it =
      gLpsrOctaveEntryKindsMap.find (
        theString);

  if (it == gLpsrOctaveEntryKindsMap.end ()) {
    // no, octave entry kind is unknown in the map

    printHelpSummary (os);

    stringstream s;

    s <<
      "octave entry kind '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrOctaveEntryKindsMap.size () <<
      " known octave entry kinds are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrOctaveEntryKinds ();

    gIndenter--;

    optionError (s.str ());

//     exit (4); // JMI
    abort ();
  }
*/

  relativeOctaveEntryItem->
    setRelativeOctaveEntryItemVariableValue (
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        theString));
}

void lilypondOptions::handleOptionsFixedOctaveEntryItemValue (
  ostream&      os,
  S_optionsFixedOctaveEntryItem
                fixedOctaveEntryItem,
  string        theString)
{
  // theString contains the score output kind:
  // is it in the score output kinds map?

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsRelativeOctaveEntryItem'" <<
      endl;
  }
#endif

  fixedOctaveEntryItem->
    setFixedOctaveEntryItemVariableValue (
      msrSemiTonesPitchAndOctave::createFromString (
        K_NO_INPUT_LINE_NUMBER,
        theString));
}

void lilypondOptions:: handleOptionsAccidentalStyleKindItemValue (
  ostream&      os,
  S_optionsAccidentalStyleKindItem
                accidentalStyleKindItem,
  string        theString)
{
  // theString contains the language name:
  // is it in the accidental styles map?

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsAccidentalStyleKindItem'" <<
      endl;
  }
#endif

  map<string, lpsrAccidentalStyleKind>::const_iterator
    it =
      gLpsrAccidentalStyleKindsMap.find (
        theString);

  if (it == gLpsrAccidentalStyleKindsMap.end ()) {
    // no, accidental style is unknown in the map
    stringstream s;

    s <<
      "LPSR accidental style '" << theString <<
      "' is unknown" <<
      endl <<
      "The " <<
      gLpsrAccidentalStyleKindsMap.size () - 1 <<
      " known LPSR accidental styles are:" <<
      endl;

    gIndenter++;

    s <<
      existingLpsrAccidentalStyleKinds ();

    gIndenter--;

    optionError (s.str ());

    printHelpSummary (os);

    exit (4);
  }

  accidentalStyleKindItem->
    setAccidentalStyleKindItemVariableValue (
      (*it).second);
}

void lilypondOptions::handleOptionsChordsDisplayItemValue (
  ostream&      os,
  S_optionsChordsDisplayItem
                chordsDisplayItem,
  string        theString)
{
  /*
    % Exception music is chords with markups
    chExceptionMusic = {
      <c ees ges bes>1-\markup { \super {"-7(" {\small\raise #0.5 \flat} "5)"} }
      <c e g bes>1-\markup { \super "7" }
      <c e gis bes>1-\markup { \super {"7(" {\small\raise #0.5 \sharp} "5)"} }
      <c f g bes>1-\markup { \super {"7(sus4)"} }
      <c e g a d'>1-\markup { \super "6/9" }
      <c e g bes des'>1-\markup { \super {"7(" {\small\raise #0.5 \flat} "9)"} }
      <c f g bes d'>1-\markup { \super {"9(sus4)"} }
      <c e g bes d'>1-\markup { \super "9" }
      <c e g b d'>1-\markup { \super "maj9" }
      <c e gis bes d'>1-\markup { \super "9+" }
      <c e g bes d' fis'>1-\markup { \super "9#11" }
      <c e g bes d' f'>1-\markup { \super "11" }
      <c e g bes d' a'>1-\markup { \super "13" }
      <c e g bes d' fis' a'>1-\markup { \super {"13(" {\small\raise #0.5 \sharp} "11)"} }
      <c e g a d'>1-\markup { \super "6(add9)" }
    }

    chExceptions = #(append
                     (sequential-music-to-chord-exceptions chExceptionMusic #t)
                     ignatzekExceptions)

    -7b5:
      -cd, -chord-display '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'
  */

  // theString contains a specification such as:
  //   '<c ees ges bes> \super {"-7(" {\small\raise #0.5 \flat} "5)"}'

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsChordsDisplayItem'" <<
      endl;
  }
#endif

  string regularExpression (
    "(<.+>)"
    "[[:space:]]+"
    "(.+)"
    );

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "There are " << smSize << " matches" <<
      " for chords display string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-chords-display argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

    printSpecificSubGroupHelp (
      os,
      chordsDisplayItem->
        getOptionsSubGroupUplink ());

    exit (4);
  }

  string chordContents     = sm [1];
  string chordsDisplaycode = sm [2];

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "FAA chordContents  = " <<
      chordContents <<
      endl <<
      "chordsDisplaycode = " <<
      chordsDisplaycode <<
      endl;

  chordsDisplayItem->
    setOptionsChordsDisplayItemVariableDisplayName (
      make_pair (
        chordContents, chordsDisplaycode));
  }
#endif
}

void lilypondOptions::handleOptionsMidiTempoItemValue (
  ostream&      os,
  S_optionsMidiTempoItem
                midiTempoItem,
  string        theString)
{
  // theString contains the midi tempo specification
  // decipher it to extract duration and perSecond values

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "==> optionsItem is of type 'optionsMidiTempoItem'" <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:digit:]]+\\.*)"
    "[[:space:]]*"
    "="
    "[[:space:]]*"
    "([[:digit:]]+)"
    "[[:space:]]*");

  regex  e (regularExpression);
  smatch sm;

  regex_match (theString, sm, e);

  unsigned smSize = sm.size ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "There are " << smSize << " matches" <<
      " for MIDI tempo string '" << theString <<
      "' with regex '" << regularExpression <<
      "':" <<
      endl;

    gIndenter++;

    for (unsigned i = 0; i < smSize; ++i) {
      os <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    os << endl;

    gIndenter--;
  }
#endif

  if (smSize != 3) {
    stringstream s;

    s <<
      "-midiTempo argument '" << theString <<
      "' is ill-formed";

    optionError (s.str ());

    printSpecificSubGroupHelp (
      os,
      midiTempoItem->
        getOptionsSubGroupUplink ());

    exit (4);
  }

  string midiTempoDuration  = sm [1];

  int    midiTempoPerSecond;
  {
    stringstream s;
    s << sm [2];
    s >> midiTempoPerSecond;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions) {
    os <<
      "midiTempoDuration  = " <<
      midiTempoDuration <<
      endl <<
      "midiTempoPerSecond = " <<
      midiTempoPerSecond <<
      endl;

  midiTempoItem->
    setMidiTempoItemVariableValue (
      pair<string, int> (
        midiTempoDuration, midiTempoPerSecond));
  }
#endif
}

void lilypondOptions::handleOptionsItemValue (
  ostream&      os,
  S_optionsItem item,
  string        theString)
{
  if (
    // relative octave entry item?
    S_optionsRelativeOctaveEntryItem
      relativeOctaveEntryItem =
        dynamic_cast<optionsRelativeOctaveEntryItem*>(&(*item))
  ) {
    handleOptionsRelativeOctaveEntryItemValue (
      os,
      relativeOctaveEntryItem,
      theString);
  }

  if (
    // fixed octave entry item?
    S_optionsFixedOctaveEntryItem
      fixedOctaveEntryItem =
        dynamic_cast<optionsFixedOctaveEntryItem*>(&(*item))
  ) {
    handleOptionsFixedOctaveEntryItemValue (
      os,
      fixedOctaveEntryItem,
      theString);
  }

  else if (
    // accidental style item?
    S_optionsAccidentalStyleKindItem
      accidentalStyleKindItem =
        dynamic_cast<optionsAccidentalStyleKindItem*>(&(*item))
  ) {
    handleOptionsAccidentalStyleKindItemValue (
      os,
      accidentalStyleKindItem,
      theString);
  }

  else if (
    // chords display item?
    S_optionsChordsDisplayItem
      chordsDisplayItem =
        dynamic_cast<optionsChordsDisplayItem*>(&(*item))
  ) {
    handleOptionsChordsDisplayItemValue (
      os,
      chordsDisplayItem,
      theString);
  }

  else if (
    // midi tempo item?
    S_optionsMidiTempoItem
      midiTempoItem =
        dynamic_cast<optionsMidiTempoItem*>(&(*item))
  ) {
    handleOptionsMidiTempoItemValue (
      os,
      midiTempoItem,
      theString);
  }
}

ostream& operator<< (ostream& os, const S_lilypondOptions& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
void initializeLilypondOptionsHandling (
  S_optionsHandler optionsHandler)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceOptions && ! gGeneralOptions->fQuiet) {
    glogIOstream <<
      "Initializing LilyPond options handling" <<
      endl;
  }
#endif

  // create the options variables
  // ------------------------------------------------------

  gLilypondOptionsUserChoices = lilypondOptions::create (
    optionsHandler);
  assert(gLilypondOptionsUserChoices != 0);

  gLilypondOptions =
    gLilypondOptionsUserChoices;

  // prepare for measure detailed trace
  // ------------------------------------------------------

  gLilypondOptionsWithDetailedTrace =
    gLilypondOptions->
      createCloneWithDetailedTrace ();
}


}
