/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrPartGroups.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"
#include "xml2lyOptionsHandling.h"


using namespace std;


namespace MusicXML2 
{

//______________________________________________________________________________
S_msrPartGroup msrPartGroup::create (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrPartGroup           partGroupPartGroupUplink,
  S_msrScore               partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      inputLineNumber,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      partGroupSymbolKind,
      partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitNo,
      partGroupBarlineKind,
      partGroupPartGroupUplink,
      partGroupScoreUplink);
  assert(o!=0);
  return o;
}

S_msrPartGroup msrPartGroup::createImplicitPartGroup (
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrScore               partGroupScoreUplink)
{
  msrPartGroup* o =
    new msrPartGroup (
      NO_INPUT_LINE_NUMBER,
      partGroupNumber,
      partGroupAbsoluteNumber,
      partGroupName,
      partGroupNameDisplayText,
      partGroupAccidentalText,
      partGroupAbbreviation,
      msrPartGroup::kPartGroupSymbolNone, // partGroupSymbolKind
      0,                                  // partGroupSymbolDefaultX,
      msrPartGroup::kPartGroupImplicitYes,
      partGroupBarlineKind,
      0,                                  // partGroupPartGroupUplink,
                                          // will be set below
      partGroupScoreUplink);
  assert(o!=0);

  // the implicit part group it the top-most one:
  // set its group uplink points to itself
  o->fPartGroupPartGroupUplink = o;
  
  return o;
}

msrPartGroup::msrPartGroup (
  int                      inputLineNumber,
  int                      partGroupNumber,
  int                      partGroupAbsoluteNumber,
  string                   partGroupName,
  string                   partGroupNameDisplayText,
  string                   partGroupAccidentalText,
  string                   partGroupAbbreviation,
  msrPartGroupSymbolKind   partGroupSymbolKind,
  int                      partGroupSymbolDefaultX,
  msrPartGroupImplicitKind partGroupImplicitKind,
  msrPartGroupBarlineKind  partGroupBarlineKind,
  S_msrPartGroup           partGroupPartGroupUplink,
  S_msrScore               partGroupScoreUplink)
    : msrElement (inputLineNumber)
{
  // no sanity check on partGroupPartGroupUplink here,
  // it will be set after all 'real' (i.e. not implicit)
  // part groups and part have been analyzed
  fPartGroupPartGroupUplink = partGroupPartGroupUplink;

/* JMI
  // sanity check
  msrAssert (
    fPartGroupScoreUplink != nullptr,
    "fPartGroupScoreUplink is null");
    */
    
  fPartGroupScoreUplink     = partGroupScoreUplink;

  // other fields
  fPartGroupNumber          = partGroupNumber;
  fPartGroupAbsoluteNumber  = partGroupAbsoluteNumber;
          
  fPartGroupName            = partGroupName;

  int partGroupNameLength =
    fPartGroupName.size ();
  
  if (
    partGroupNameLength
      >
    fPartGroupScoreUplink->getScorePartGroupNamesMaxLength ()) {  // JMI sanity check ???
    fPartGroupScoreUplink->
      setScorePartGroupNamesMaxLength (
        partGroupNameLength);
  }
          
  fPartGroupNameDisplayText = partGroupNameDisplayText;
  
  fPartGroupAccidentalText  = partGroupAccidentalText;
  
  fPartGroupAbbreviation    = partGroupAbbreviation;

  fPartGroupSymbolKind      = partGroupSymbolKind;
  fPartGroupSymbolDefaultX  = partGroupSymbolDefaultX;

  fPartGroupImplicitKind    = partGroupImplicitKind;
    
  fPartGroupBarlineKind     = partGroupBarlineKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating part group '" << fPartGroupNumber << "'" <<
      ", partGroupAbsoluteNumber = " << fPartGroupAbsoluteNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif
}

msrPartGroup::~msrPartGroup ()
{}

S_msrPartGroup msrPartGroup::createPartGroupNewbornClone (
  S_msrPartGroup partGroupClone,
  S_msrScore     scoreClone)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "--------------------------------------------" <<
      endl <<
      "Creating a newborn clone part group " <<
      getPartGroupCombinedName () <<
      endl;
  }
#endif

  // don't check partGroupClone against 0, since the partGroup stack JMI
  // that it comes from may be empty
/* JMI
  // sanity check
  msrAssert(
    partGroupClone != nullptr,
    "partGroupClone is null");
    */
    
  // sanity check
  msrAssert(
    scoreClone != nullptr,
    "scoreClone is null");
    
  S_msrPartGroup
    newbornClone =
      msrPartGroup::create (
        fInputLineNumber,
        fPartGroupNumber,
        fPartGroupAbsoluteNumber,
        fPartGroupName,
        fPartGroupNameDisplayText,
        fPartGroupAccidentalText,
        fPartGroupAbbreviation,
        fPartGroupSymbolKind,
        fPartGroupSymbolDefaultX,
        fPartGroupBarlineKind,
        partGroupClone,
        scoreClone);
  
  newbornClone->fPartGroupImplicitKind =
    fPartGroupImplicitKind;
  
  newbornClone->fPartGroupInstrumentName =
    fPartGroupInstrumentName;
  
  return newbornClone;
}

string msrPartGroup::getPartGroupCombinedName () const
{
  stringstream s;

  s <<
    "PartGroup_" << fPartGroupAbsoluteNumber <<
    " ('" << fPartGroupNumber <<
    "', partGroupName \"" << fPartGroupName << "\")";

  return s.str ();
}

void msrPartGroup::setPartGroupInstrumentName (
  string partGroupInstrumentName)
{
  fPartGroupInstrumentName = partGroupInstrumentName;

  S_msrScore
    score =
      fPartGroupScoreUplink;
        
  int partGroupInstrumentNameLength = fPartGroupInstrumentName.size ();
  
  if (
    partGroupInstrumentNameLength
      >
    score->getScoreInstrumentNamesMaxLength ())
    score->
      setScoreInstrumentNamesMaxLength (
        partGroupInstrumentNameLength);
}

S_msrPart msrPartGroup::appendPartToPartGroupByItsPartID (
  int    inputLineNumber,
  string partID)
{
  // sanity check
  msrAssert (
    partID.size () > 0,
    "partID is empty");

  // has this partID already been added to this part?
  if (fPartGroupPartsMap.count (partID)) {
    stringstream s;

    s <<
      "partID \"" << partID <<
      "\" already exists in part group " <<
      getPartGroupCombinedName ();

    msrMusicXMLWarning ( // JMI
      gXml2lyOptions->fInputSourceName,
      inputLineNumber,
      s.str ());

    return fPartGroupPartsMap [partID];
  }

  // create the part
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        partID,
        this);

  // register it in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group '" <<
      fPartGroupNumber <<
      "'" <<
      endl;
  }
#endif
  
  fPartGroupPartsMap [partID] = part;
  fPartGroupElements.push_back (part);

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {
    gLogIOstream <<
      endl <<
      "After appendPartToPartGroupByItsID, fPartGroupPartsMap contains:" <<
      endl;
        
    gIndenter++;
    
    for (
        map<string, S_msrPart>::const_iterator i = fPartGroupPartsMap.begin ();
        i != fPartGroupPartsMap.end ();
        i++) {
      gLogIOstream <<
        "\"" << (*i).first << "\" --% --> " <<
        (*i).second->
          getPartCombinedName () <<
        endl;
    } // for
    
    gIndenter--;
    
    gLogIOstream <<
      "After appendPartToPartGroupByItsID, fPartGroupElements contains:" <<
      endl;
      
    gIndenter++;

    if (fPartGroupElements.size ()) {
      list<S_msrElement>::const_iterator
        iBegin = fPartGroupElements.begin (),
        iEnd   = fPartGroupElements.end (),
        i      = iBegin;
        
      for ( ; ; ) {
        gLogIOstream <<
          (*i);
        if (++i == iEnd) break;
        gLogIOstream <<
          endl;
      } // for
    }
    
    gIndenter--;
  }
#endif
  
  // return the part
  return part;
}

void msrPartGroup::appendPartToPartGroup (S_msrPart part)
{
  // register part in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to part group " << fPartGroupNumber <<
      endl;
  }
#endif

  // register part into this part group's data
  fPartGroupPartsMap [part->getPartID ()] = part;
  fPartGroupElements.push_back (part);

  // set part's partgroup uplink
  part->setPartPartGroupUplink (this);
}

void msrPartGroup::removePartFromPartGroup (
  int       inputLineNumber,
  S_msrPart partToBeRemoved)
{
  // register part in this part group
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Removing part " <<
      partToBeRemoved->getPartCombinedName () <<
      " from part group " << fPartGroupNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  for (
    list<S_msrElement>::iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        nestedPartGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      if (part == partToBeRemoved) {
        fPartGroupElements.erase (i);
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  gIndenter--;
}

void msrPartGroup::prependSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Prepending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }
#endif

  // register it in this part group
  fPartGroupElements.push_front (partGroup);
}

void msrPartGroup::appendSubPartGroupToPartGroup (
  S_msrPartGroup partGroup)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending (sub-)part group " << partGroup->getPartGroupNumber () <<
      " to part group " << getPartGroupNumber ()  << endl;
  }
#endif

  // register it in this part group
  fPartGroupElements.push_back (partGroup);
}


void msrPartGroup::printPartGroupParts (
  int      inputLineNumber,
  ostream& os)
{
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        nestedPartGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      gLogIOstream <<
        nestedPartGroup->
          getPartGroupCombinedName () <<
        endl;

      gIndenter++;

      nestedPartGroup->
        printPartGroupParts (
          inputLineNumber,
          os);
      
      gIndenter--;
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      gLogIOstream <<
        part->
          getPartCombinedName () <<
        endl;
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for
}

S_msrPart msrPartGroup::fetchPartFromPartGroupByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {    
    gLogIOstream <<
      "fetchPartFromPartGroupByItsPartID(" << partID << "), fPartGroupElements contains:" <<
      endl;

    gIndenter++;

    printPartGroupParts (
      inputLineNumber,
      gLogIOstream);
    
    gIndenter--;

    gLogIOstream <<
      "<=- fetchPartFromPartGroupByItsPartID(" << partID << ")" <<
      endl <<
      endl;
  }
#endif

/* JMI
  list<S_msrElement>::const_iterator
    iBegin = fPartGroupElements.begin (),
    iEnd   = fPartGroupElements.end (),
    i      = iBegin;
  */
    
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      S_msrPart
        inter =
          partGroup->
            fetchPartFromPartGroupByItsPartID (
              inputLineNumber,
              partID);

      if (inter) {
        result = inter;
        break;
      }
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      if (part->getPartID () == partID) {
        result = part;
        break;
      }
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for

  return result;
}

void msrPartGroup::collectPartGroupPartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    S_msrElement
      element = (*i);

    if (
      S_msrPartGroup
        partGroup =
          dynamic_cast<msrPartGroup*>(&(*element))
      ) {
      // this is a part group          
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
    }

    else if (
      S_msrPart
        part =
          dynamic_cast<msrPart*>(&(*element))
      ) {
      // this is a part
      partsList.push_back (part);
    }

    else {
      stringstream s;

      s <<
        "an element of partgroup " <<
        getPartGroupCombinedName () <<
        " is not a part group nor a part";

      msrInternalError (
        gXml2lyOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  } // for
}

void msrPartGroup::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPartGroup::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPartGroup>*
    p =
      dynamic_cast<visitor<S_msrPartGroup>*> (v)) {
        S_msrPartGroup elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPartGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPartGroup::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPartGroup::browseData ()" <<
      endl;
  }
  
  for (
    list<S_msrElement>::const_iterator i = fPartGroupElements.begin ();
    i != fPartGroupElements.end ();
    i++) {
    // browse the part element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrPartGroup::browseData ()" <<
      endl;
  }
}

string msrPartGroup::partGroupImplicitKindAsString (
  msrPartGroupImplicitKind partGroupImplicitKind)
{
  string result;
  
  switch (partGroupImplicitKind) {
    case msrPartGroup::kPartGroupImplicitYes:
      result = "partGroupImplicitYes";
      break;
    case msrPartGroup::kPartGroupImplicitNo:
      result = "partGroupImplicitNo";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupImplicitKindAsString () const
{
  return
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind);
}

string msrPartGroup::partGroupTypeKindAsString (
  msrPartGroupTypeKind partGroupTypeKind)
{
  string result;
  
  switch (partGroupTypeKind) {
    case msrPartGroup::kPartGroupTypeNone:
      result = "partGroupTypeNone";
      break;
    case msrPartGroup::kPartGroupTypeStart:
      result = "partGroupTypeStart";
      break;
    case msrPartGroup::kPartGroupTypeStop:
      result = "partGroupTypeStop";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarlineKindAsString () const
{
  return
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind);
}

string msrPartGroup::asString () const
{
  stringstream s;

  s <<
    "PartGroup \"" <<
    getPartGroupCombinedName () <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

string msrPartGroup::partGroupSymbolKindAsString (
  msrPartGroupSymbolKind partGroupSymbolKind)
{
  string result;
  
  switch (partGroupSymbolKind) {
    case msrPartGroup::kPartGroupSymbolNone:
      result = "partGroupSymbolNone";
      break;
    case msrPartGroup::kPartGroupSymbolBrace:
      result = "partGroupSymbolBrace";
      break;
    case msrPartGroup::kPartGroupSymbolBracket:
      result = "partGroupSymbolBracket";
      break;
    case msrPartGroup::kPartGroupSymbolLine:
      result = "partGroupSymbolLine";
      break;
    case msrPartGroup::kPartGroupSymbolSquare:
      result = "partGroupSymbolSquare";
      break;
  } // switch

  return result;
}

string msrPartGroup::partGroupBarlineKindAsString (
  msrPartGroupBarlineKind partGroupBarlineKind)
{
  string result;
  
  switch (partGroupBarlineKind) {
    case msrPartGroup::kPartGroupBarlineYes:
      result = "partGroupBarlineYes";
      break;
    case msrPartGroup::kPartGroupBarlineNo:
      result = "partGroupBarlineNo";
      break;
  } // switch

  return result;
}

void msrPartGroup::print (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    ", line " << fInputLineNumber <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 25;
  
  os << left <<
    setw (fieldWidth) <<
    "partGroupPartGroupUplink" << " : ";

  if (fPartGroupPartGroupUplink) {
    // it may be empty
    os <<
      fPartGroupPartGroupUplink->
        getPartGroupCombinedName ();
  }
  else {
    os <<
      "none";
  }
  os <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl;

   os << left <<
    setw (fieldWidth) <<
    "partGroupPartGroupUplink" << " : ";
  if (fPartGroupPartGroupUplink) {
    os <<
      "\"" <<
      fPartGroupPartGroupUplink->
        getPartGroupCombinedName () <<
      "\"";
  }
  else {
    os <<
    "none";
  }
  os <<
    endl;

  os << left <<
   setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    fPartGroupNameDisplayText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupAccidentalText" << " : \"" <<
    fPartGroupAccidentalText <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "partGroupSymbolKind" << " : " <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    endl;
    
  os << left <<
    setw (fieldWidth) <<
    "partGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "partGroupBarline" << " : " <<
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind) <<
    endl;

  if (fPartGroupElements.size ()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin (),
      iEnd   = fPartGroupElements.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  gIndenter--;
}

void msrPartGroup::printSummary (ostream& os)
{
  os <<
    "PartGroup" " \"" << getPartGroupCombinedName () <<
    "\" (" <<
    singularOrPlural (
      fPartGroupPartsMap.size (), "part", "parts") <<
    ")" <<
    endl;
    
  gIndenter++;

  const int fieldWidth = 24;
  
  os << left <<
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    fPartGroupName <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    fPartGroupAbbreviation <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupSymbolDefaultX" << " : " <<
    fPartGroupSymbolDefaultX <<
      endl <<
    setw (fieldWidth) <<
    "partGroupSymbolKind" << " : \"" <<
    partGroupSymbolKindAsString (
      fPartGroupSymbolKind) <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupImplicit" << " : " <<
    partGroupImplicitKindAsString (
      fPartGroupImplicitKind) <<
    endl <<

    setw (fieldWidth) <<
    "partGroupBarline" << " : " <<
    partGroupBarlineKindAsString (
      fPartGroupBarlineKind) <<
    endl;

  if (fPartGroupElements.size ()) {
    os << endl;
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupElements.begin (),
      iEnd   = fPartGroupElements.end (),
      i      = iBegin;
      
    gIndenter++;
    for ( ; ; ) {
      (*i)->printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
    gIndenter--;
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPartGroup& elt)
{
  elt->print (os);
  return os;
}
          

}
