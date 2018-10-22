/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iomanip>      // setw, setprecision, ...

#include "msrOptions.h"

#include "msr2Names.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2NamesVisitor::msr2NamesVisitor (
  S_msrOptions&    msrOpts,
  indentedOstream& ios)
    : fMsrNamesOutputStream (ios)
{
  fMsrOptions = msrOpts;

  // part groups
  fPartGroupsCounter = 0;

  // parts
  fPartsCounter = 0;

  // staves
  fStavesCounter = 0;
  fOnGoingStaff = false;

  // voices
  fVoicesCounter = 0;
};
  
msr2NamesVisitor::~msr2NamesVisitor ()
{}

//________________________________________________________________________
void msr2NamesVisitor::printNamesFromMsrScore (
  const S_msrScore& score)
{
  if (score) {    
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrScore& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrScore" <<
      endl;
  }
  
  fMsrNamesOutputStream <<
    "Score contains " <<
    singularOrPlural (
      elt->getPartGroupsList ().size (),
      "part group", "part groups") <<
    ", " <<
    singularOrPlural (
      elt->getScoreNumberOfMeasures (),
      "measure", "measures") <<
    endl <<
    endl;

  gIndenter++;
}

void msr2NamesVisitor::visitEnd (S_msrScore& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrScore" <<
      endl;
  }
  
  fMsrNamesOutputStream <<
    "The score contains:" <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 3;

  fMsrNamesOutputStream <<
    setw (fieldWidth) <<
    singularOrPlural (
      fPartGroupsCounter,
      "part group",
      "part groups") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fPartsCounter,
      "part",
      "parts") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fStavesCounter,
      "stave",
      "staves") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fVoicesCounter,
      "voice",
      "voices") <<
    endl;

  gIndenter--;
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrPartGroup" <<
      endl;
  }

  fPartGroupsCounter++;

  int partGroupElementsSize = elt->getPartGroupElements ().size ();
  
  fMsrNamesOutputStream <<
    "PartGroup" << " " << elt->getPartGroupCombinedName () <<
    " contains " <<
    singularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    endl;

  gIndenter++;

  const int fieldWidth = 25;
  
  fMsrNamesOutputStream << left <<
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    elt->getPartGroupName  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupInstrumentName" << " : \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    endl <<
    endl;
}

void msr2NamesVisitor::visitEnd (S_msrPartGroup& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrPartGroup" <<
      endl;
  }
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrPart& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrPart" <<
      endl;
  }

  fPartsCounter++;
  
  int partStavesMapSize = elt->getPartStavesMap ().size ();
  
  fMsrNamesOutputStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    singularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    endl;
    
  gIndenter++;
  
  const int fieldWidth = 27;
  
  fMsrNamesOutputStream << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    elt->getPartID () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    elt->getPartMsrName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partName" << " : \"" <<
    elt->getPartName  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    elt->getPartAbbreviation () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    elt->getPartInstrumentName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    endl;

  fMsrNamesOutputStream <<
    endl;
}

void msr2NamesVisitor::visitEnd (S_msrPart& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrPart" <<
      endl;
  }
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrStaff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrStaff" <<
      endl;
  }

  fStavesCounter++;
  
  int staffAllVoicesListSize = elt->getStaffAllVoicesList ().size ();
    
  fMsrNamesOutputStream <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " <<
    singularOrPlural (
      staffAllVoicesListSize,
      "voice", "voices") <<
    endl;

  gIndenter++;

  
  const int fieldWidth = 28;

  fMsrNamesOutputStream << left <<
    setw (fieldWidth) <<
    "staffNumber" << " : " <<
    elt->getStaffNumber () <<
    endl <<

    setw (fieldWidth) <<
    "staffInstrumentName" << ": \"" <<
    elt->getStaffInstrumentName () << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "staffInstrumentAbbreviation" << ": \"" <<
    elt->getStaffInstrumentAbbreviation () << "\"" <<
    endl;

  fMsrNamesOutputStream <<
    endl;

  fOnGoingStaff = true;
}

void msr2NamesVisitor::visitEnd (S_msrStaff& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrStaff" <<
      endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrVoice& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrVoice" <<
      endl;
  }

  fVoicesCounter++;

  int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  fMsrNamesOutputStream <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " <<
    singularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;
  
  fMsrNamesOutputStream << left <<
    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    elt->getVoiceNumber () <<
    endl <<
    setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (
      elt->getMusicHasBeenInsertedInVoice ()) <<
    endl;

  fMsrNamesOutputStream <<
    endl;
}

void msr2NamesVisitor::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrVoice" <<
      endl;
  }
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrVarValAssoc" <<
      endl;
  }

  fMsrNamesOutputStream <<
    "VarValAssoc" <<
    endl;
  
  gIndenter++;
  
  const int fieldWidth = 16;
  
  fMsrNamesOutputStream <<
    setw (fieldWidth) <<
    "varValAssocKind" << " : " <<
    "\"" <<
    elt->varValAssocKindAsString () <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "variableValue" << " : " <<
    "\"" << elt->getVariableValue () << "\"" <<
    endl <<
    endl;
  
  gIndenter--;
}

void msr2NamesVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrVarValAssoc" <<
      endl;
  }
}

//________________________________________________________________________
void msr2NamesVisitor::visitStart (S_msrVarValsListAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> Start visiting msrVarValsListAssoc" <<
      endl;
  }

  fMsrNamesOutputStream <<
    "VarValsListAssoc" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 23;
  
  fMsrNamesOutputStream << left <<
    setw (fieldWidth) <<
    "varValsListAssocKind" << " : " <<
    "\"" <<
    elt->varValsListAssocKindAsString () <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "varValsListAssocValues" << " : " <<
    "'" << elt->varValsListAssocValuesAsString () << "'" <<
    endl <<
    endl;
  
  gIndenter--;
}

void msr2NamesVisitor::visitEnd (S_msrVarValsListAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrNamesOutputStream <<
      "--> End visiting msrVarValsListAssoc" <<
      endl;
  }
}


} // namespace
