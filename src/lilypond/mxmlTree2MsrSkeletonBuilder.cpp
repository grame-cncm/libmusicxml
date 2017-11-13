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

#include <climits>      /* INT_MIN */

#include "messagesHandling.h"

#include "mxmlOptions.h"
#include "msrOptions.h"

#include "msr2Summary.h"

#include "mxmlTree2MsrSkeletonBuilder.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_mxmlPartGroupDescr mxmlPartGroupDescr::create (
  int            startInputLineNumber,
  int            partGroupNumber,
  S_msrPartGroup partGroup,
  int            startPosition)
{
  mxmlPartGroupDescr* o = new
    mxmlPartGroupDescr (
      startInputLineNumber,
      partGroupNumber,
      partGroup,
      startPosition);
  assert(o!=0);
  return o;
}

mxmlPartGroupDescr::mxmlPartGroupDescr (
  int            startInputLineNumber,
  int            partGroupNumber,
  S_msrPartGroup partGroup,
  int            startPosition)
{
  fStartInputLineNumber   = startInputLineNumber;
  fStopInputLineNumber   = -1;
  
  fPartGroupNumber = partGroupNumber;
  fPartGroup       = partGroup;
  
  fStartPosition   = startPosition;
  fStopPosition    = -1;
}

mxmlPartGroupDescr::~mxmlPartGroupDescr ()
{}

string mxmlPartGroupDescr::partGroupDescrAsString () const
{
  stringstream s;

  s <<
    "'" <<
    fPartGroupNumber <<
    "' -=> " <<
    fPartGroup->getPartGroupCombinedName ();

  if (gGeneralOptions->fTracePartGroups) {
    s <<
      ", positions " <<
      fStartPosition << ".." << fStopPosition;
  }

  s <<
    ", lines " <<
    fStartInputLineNumber << ".." << fStopInputLineNumber;

  return s.str ();
}

void mxmlPartGroupDescr::print (ostream& os) const
{
  const int fieldWidth = 14;
  
  os << left <<
    setw (fieldWidth) <<
    "partGroupNumber" << " : " <<
    fPartGroupNumber <<
    endl <<
    setw (fieldWidth) <<
    "partGroup" << " : " <<
    fPartGroup <<
    endl <<
    setw (fieldWidth) <<
    "startPosition" << " : " <<
    fStartPosition <<
    endl <<
    setw (fieldWidth) <<
    "startInputLineNumber" << " : " <<
    fStartInputLineNumber <<
    endl <<
    "stopPosition" << " : " <<
    fStopPosition <<
    endl <<
    setw (fieldWidth) <<
    "stopInputLineNumber" << " : " <<
    fStopInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_mxmlPartGroupDescr& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
mxmlTree2MsrSkeletonBuilder::mxmlTree2MsrSkeletonBuilder (
  indentedOstream& ios)
  : fLogOutputStream (ios)
{
  // the MSR score we're building
  fMsrScore =
    msrScore::create (0);

  // score handling
  fScoreNumberOfMeasures = 0;

  // part groups handling  
  fPartGroupsCounter = 0;
  fOnGoingPartGroupNameDisplay = false;

  // parts handling
  fCurrentNoIDPartNumber = 0;
  fCurrentPartsPosition = 0;
  fOnGoingPartNameDisplay = false;
  
  // staff handling
  fCurrentStaffNumber = -1;

  // voice handling
  fCurrentVoiceNumber = -1;

  // measures
  fPartNumberOfMeasures = 0;
    
  // lyrics handling
  fCurrentStanzaNumber = -1; // JMI

  // harmonies handling
 
  // figured bass handling
    
  // ongoing note
  fOnGoingNote = false;

  // note context
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;

  // create an empty list for part groups starting at 0,
  // i.e. for the implicit part group
  fPositionStartingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create an empty list for part groups stopping at 0,
  // that will remain empty
  fPositionStoppingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create the implicit part group
  createImplicitPartGroup ();
}

mxmlTree2MsrSkeletonBuilder::~mxmlTree2MsrSkeletonBuilder ()
{}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::browseMxmlTree (
  const Sxmlelement& mxmlTree)
{  
  if (mxmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*mxmlTree);
  }
}

//______________________________________________________________________________
S_mxmlPartGroupDescr mxmlTree2MsrSkeletonBuilder::fetchStartedPartGroupDescr (
  int partGroupNumber)
{
  S_mxmlPartGroupDescr result;
  
  if (fStartedPartGroupDescrsMap.count (partGroupNumber)) {
    result =
      fStartedPartGroupDescrsMap [partGroupNumber];
  }

  return result;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showAllPartGroupDescrsMap (
  int inputLineNumber)
{
  fLogOutputStream <<
    "AllPartGroupDescrsMap:" <<
    endl;
    
  if (fAllPartGroupDescrsMap.size ()) {
    gIndenter++;
    
    map<int, S_mxmlPartGroupDescr>::const_iterator
      iBegin = fAllPartGroupDescrsMap.begin (),
      iEnd   = fAllPartGroupDescrsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_mxmlPartGroupDescr
        partGroupDescr = (*i).second;
        
      S_msrPartGroup
        partGroup =
          partGroupDescr->getPartGroup ();

      fLogOutputStream <<
        partGroupDescr->partGroupDescrAsString () <<
        endl;

        gIndenter++;
        
        partGroup->
          printPartGroupParts (
            inputLineNumber,
            fLogOutputStream);

        gIndenter--;
          
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty map" <<
      endl;
  }
      
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showStartedPartGroupDescrsMap (
  int inputLineNumber)
{
  fLogOutputStream <<
    "StartedPartGroupDescrsMap:" <<
    endl;
    
  if (fStartedPartGroupDescrsMap.size ()) {
    gIndenter++;
    
    map<int, S_mxmlPartGroupDescr>::const_iterator
      iBegin = fStartedPartGroupDescrsMap.begin (),
      iEnd   = fStartedPartGroupDescrsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_mxmlPartGroupDescr
        partGroupDescr = (*i).second;
        
      S_msrPartGroup
        partGroup =
          partGroupDescr->getPartGroup ();

      fLogOutputStream <<
        partGroupDescr->partGroupDescrAsString () <<
        endl;

        gIndenter++;
        
        partGroup->
          printPartGroupParts (
            inputLineNumber,
            fLogOutputStream);

        gIndenter--;
          
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty map" <<
      endl;
  }
      
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsStack (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartGroupsDescrStack:" <<
    endl;

  if (! fPartGroupsDescrStack.empty ()) {
    gIndenter++;

    list<S_mxmlPartGroupDescr>::const_iterator
      iBegin = fPartGroupsDescrStack.begin (),
      iEnd   = fPartGroupsDescrStack.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        "v " <<
        (*i)->partGroupDescrAsString () <<
        endl;
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty stack" <<
      endl;
  }
      
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupDescrsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartGroupDescrsVector:" <<
    endl;
    
  gIndenter++;
  
  for (unsigned int i = 0; i < fPartGroupDescsVector.size (); i++) {
    S_mxmlPartGroupDescr
      partGroupDescr = fPartGroupDescsVector [i];
      
    S_msrPartGroup
      partGroup =
        partGroupDescr->getPartGroup ();

    fLogOutputStream <<
      i + 1 << ": " <<
        partGroupDescr->partGroupDescrAsString () <<
      ", line " << inputLineNumber <<
      endl;
  } // for
  
  fLogOutputStream <<
    "------------------" <<
    endl;
    
  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPositionStartingPartGroupDescrsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PositionStartingPartGroupDescrsVector:" <<
    endl;
    
  if (fPositionStartingPartGroupDescrsVector.size ()) {
    gIndenter++;
    
    for (unsigned int k = 0; k < fPositionStartingPartGroupDescrsVector.size (); k++) {
      fLogOutputStream <<
        k << ": " <<
        endl;
  
      list<S_mxmlPartGroupDescr>&
        startingPartGroupDescrsList =
          fPositionStartingPartGroupDescrsVector [k];
  
      if (startingPartGroupDescrsList.size ()) {
        gIndenter++;
  
        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = startingPartGroupDescrsList.begin (),
          iEnd   = startingPartGroupDescrsList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);
            
          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();
      
          fLogOutputStream <<
              partGroupDescr->partGroupDescrAsString () <<
            ", line " << inputLineNumber <<
            endl;
    
          if (++i == iEnd) break;
          // no endl here
        } // for

        gIndenter--;
      }

      else {
        fLogOutputStream <<
          gTab << "empty list" <<
          endl;
      }
    }
      
    gIndenter--;
  }
  
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPositionStoppingPartGroupDescrsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PositionStoppingPartGroupDescrsVector:" <<
    endl;
    
  if (fPositionStoppingPartGroupDescrsVector.size ()) {
    gIndenter++;
    
    for (unsigned int k = 0; k < fPositionStoppingPartGroupDescrsVector.size (); k++) {
      fLogOutputStream <<
        k << ": " <<
        endl;
  
      list<S_mxmlPartGroupDescr>&
        theList =
          fPositionStoppingPartGroupDescrsVector [k];
  
      if (theList.size ()) {
        gIndenter++;
  
        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = theList.begin (),
          iEnd   = theList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);
            
          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();
      
          fLogOutputStream <<
              partGroupDescr->partGroupDescrAsString () <<
            ", line " << inputLineNumber <<
            endl;
    
          if (++i == iEnd) break;
          // no endl here
        } // for

        gIndenter--;
      }

      else {
        fLogOutputStream <<
          gTab << "empty list" <<
          endl;
      }
    }
      
    gIndenter--;
  }
  
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartsVector:" <<
    endl;
    
  if (! fPartsVector.empty ()) {
    gIndenter++;
    
    for (unsigned int i = 0; i < fPartsVector.size (); i++) {
      S_msrPart
        part = fPartsVector [i];
        
      fLogOutputStream <<
        i + 1 << ": " <<
        part->getPartCombinedName () <<
        ", uplink to: ";

      S_msrPartGroup
        partPartGroupUplink =
          part->
            getPartPartGroupUplink ();

      if (partPartGroupUplink) {
        fLogOutputStream <<
          partPartGroupUplink->
            getPartGroupCombinedName ();
      }
      else {
        fLogOutputStream <<
          "not yet established";
      }
        
      fLogOutputStream <<
        ", line " << inputLineNumber <<
        endl;
    } // for
      
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty vector" <<
      endl;
  }
  
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsData (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    endl <<
    context <<
    ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
    ", line " << inputLineNumber <<
    ":" <<
    endl <<
    ">>> ================================================" <<
    endl;
    
  showAllPartGroupDescrsMap (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showStartedPartGroupDescrsMap (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPartGroupDescrsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;
    
  showPositionStartingPartGroupDescrsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;
    
  showPositionStoppingPartGroupDescrsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;
    
  showPartsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;

  showPartGroupsStack (
    inputLineNumber);
  fLogOutputStream <<
    endl;

  fLogOutputStream <<
    "<<< ================================================" <<
    endl <<
    endl;
}

//________________________________________________________________________
S_mxmlPartGroupDescr mxmlTree2MsrSkeletonBuilder::fetchPartGroupDescrStackTop ()
{
  S_mxmlPartGroupDescr result;

  // the current part group is the top of the stack,
  // i.e. the front for the list used to implement it
  if (fPartGroupsDescrStack.size () != 0) {
   result =
    fPartGroupsDescrStack.front ();
  }

  return result;
}
  
//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPartGroupDescrAsStarted (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Registering part group descr '" <<
      partGroupDescr->getPartGroupCombinedName () <<
      " as started" <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE registering part group " +
      partGroupDescr->
        partGroupDescrAsString () +
      " in the data");
  }

  // register part group in part groups vector, i.e. by appearance order
  fPartGroupDescsVector.push_back (
    partGroupDescr);

  // register it in fAllPartGroupDescrsMap under its position as part group,
  // since its part group number may reused later by other part groups
  fAllPartGroupDescrsMap [fPartGroupsCounter] =
    partGroupDescr;

  // register it temporarily in fStartedPartGroupDescrsMap
  // under its part group number,
  int partGroupNumber =
    partGroupDescr->
      getPartGroup ()->
        getPartGroupNumber ();
    
  fStartedPartGroupDescrsMap [partGroupNumber] =
    partGroupDescr;

/* JMI
  // register it as starting at fCurrentPartsPosition
  registerPartGroupDescrAsStartingAtCurrentPosition (
    inputLineNumber,
    partGroupDescr);
*/

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER registering part group " +
      partGroupDescr->
        partGroupDescrAsString () +
      " in the data");
  }
}

/* JMI
//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPartGroupDescrAsStartingAtCurrentPosition (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
  // this list is created in end position decreasing order,
  // so as to have the outer-most part group descr appearing first

  // fetch the list of part group descrs starting at fCurrentPartsPosition
  list<S_mxmlPartGroupDescr>&
    startingPartGroupDescrsList =
      fPositionStartingPartGroupDescrsVector [
        fCurrentPartsPosition];

      // JMI ???
      
  if (fPositionStartingPartGroupDescrsVector.size () == 0) {
    // first element, insert it directly
    startingPartGroupDescrsList.push_front (
      partGroupDescr);
  }

  else {
    // insert the element at the right place in the list
    list<S_mxmlPartGroupDescr>::iterator
      iBegin = startingPartGroupDescrsList.begin (),
      iEnd   = startingPartGroupDescrsList.end (),
      i      = iBegin;
  
    while (true) {
      if (i == iEnd) {
        // append partGroupDescr to the list 
        if (gGeneralOptions->fTracePartGroups) {
          fLogOutputStream <<
            "Appending part group descr " <<
            partGroupDescr->
              partGroupDescrAsString () <<
            "' at the end of the list of part group descrs starting at position " <<
            fCurrentPartsPosition <<
            ", line " << inputLineNumber <<
            endl;
        }

        startingPartGroupDescrsList.push_back (
          partGroupDescr);
        / *
        stringstream s;
        s <<
          "part group descr " <<
          partGroupDescr->partGroupDescrAsString () <<
          " could not be inserted in part groups to be stopped list";
          
        msrInternalError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
          * /
        break;
      }

      // fetch current element in the list
      S_mxmlPartGroupDescr
        currentPartGroupDescr = (*i);
        
      if (
        partGroupDescr->getStopPosition ()
          >
        currentPartGroupDescr->getStopPosition ()) {

        // insert partGroupDescr before currentPartGroupDescr
        if (gGeneralOptions->fTracePartGroups) {
          fLogOutputStream <<
            "Inserting part group descr " <<
            currentPartGroupDescr->
              partGroupDescrAsString () <<
            "' in pending list before" <<
            partGroupDescr->
              partGroupDescrAsString () <<
            ", line " << inputLineNumber <<
            endl;
        }

        startingPartGroupDescrsList.insert (
          i, partGroupDescr);
        break;
      }
      
      i++;
    } // while
  }
}
*/

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::insertPartGroupDescInStartingList (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr,
  list<S_mxmlPartGroupDescr>&
                       startingPartGroupDescrsList)
{
  // this list is created in stop position decreasing order,
  // so as to have the outer-most part group descr appearing first
  list<S_mxmlPartGroupDescr>::iterator
    iBegin = startingPartGroupDescrsList.begin (),
    iEnd   = startingPartGroupDescrsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append partGroupDescr to the list 
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Appending part group descr " <<
          partGroupDescr->
            partGroupDescrAsString () <<
          "' at the end of the list of part group descrs starting at position " <<
          fCurrentPartsPosition <<
          ", line " << inputLineNumber <<
          endl;
      }

      startingPartGroupDescrsList.push_back (
        partGroupDescr);
      /*
      stringstream s;
      s <<
        "part group descr " <<
        partGroupDescr->partGroupDescrAsString () <<
        " could not be inserted in part groups to be stopped list";
        
      msrInternalError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxmlPartGroupDescr
      currentPartGroupDescr = (*i);
      
    if (
      partGroupDescr->getStopPosition ()
        >
      currentPartGroupDescr->getStopPosition ()) {
      // insert partGroupDescr before currentPartGroupDescr
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Inserting part group descr " <<
          currentPartGroupDescr->
            partGroupDescrAsString () <<
          "' in pending list before" <<
          partGroupDescr->
            partGroupDescrAsString () <<
          ", line " << inputLineNumber <<
          endl;
      }

      startingPartGroupDescrsList.insert (
        i, partGroupDescr);
      break;
    }
    
    i++;
  } // while
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::insertPartGroupDescInStoppingList (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr,
  list<S_mxmlPartGroupDescr>&
                       stoppingPartGroupDescrsList)
{
  // this list is created in start position decreasing order,
  // so as to have the inner-most part group descr appearing first
  list<S_mxmlPartGroupDescr>::iterator
    iBegin = stoppingPartGroupDescrsList.begin (),
    iEnd   = stoppingPartGroupDescrsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      // append partGroupDescr to the list 
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Appending part group descr " <<
          partGroupDescr->
            partGroupDescrAsString () <<
          "' at the end of the list of part group descrs stopping at position " <<
          fCurrentPartsPosition <<
          ", line " << inputLineNumber <<
          endl;
      }

      stoppingPartGroupDescrsList.push_back (
        partGroupDescr);
      /*
      stringstream s;
      s <<
        "part group descr " <<
        partGroupDescr->partGroupDescrAsString () <<
        " could not be inserted in part groups to be stopped list";
        
      msrInternalError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
        */
      break;
    }

    // fetch current element in the list
    S_mxmlPartGroupDescr
      currentPartGroupDescr = (*i);
      
    if (
      partGroupDescr->getStartPosition ()
        >
      currentPartGroupDescr->getStartPosition ()) {
      // insert partGroupDescr before currentPartGroupDescr
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Inserting part group descr " <<
          currentPartGroupDescr->
            partGroupDescrAsString () <<
          "' in pending list before" <<
          partGroupDescr->
            partGroupDescrAsString () <<
          ", line " << inputLineNumber <<
          endl;
      }

      stoppingPartGroupDescrsList.insert (
        i, partGroupDescr);
      break;
    }
    
    i++;
  } // while
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPartGroupDescrAsStoppingAtCurrentPosition (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
  // fetch the list of part group descrs stopping at
  // the same position as partGroupDescr
  list<S_mxmlPartGroupDescr>&
    stoppingPartGroupDescrsList =
      fPositionStoppingPartGroupDescrsVector [
        fCurrentPartsPosition];

  if (stoppingPartGroupDescrsList.size () == 0) {
    // first element, insert it directly
    stoppingPartGroupDescrsList.push_back (
      partGroupDescr);
  }

  else {
    // insert the element at the right place in the stopping list
    insertPartGroupDescInStoppingList (
      inputLineNumber,
      partGroupDescr,
      stoppingPartGroupDescrsList);
  }
    
  // fetch the list of part group descrs starting at
  // the same position as partGroupDescr
  list<S_mxmlPartGroupDescr>&
    startingPartGroupDescrsList =
      fPositionStartingPartGroupDescrsVector [
        partGroupDescr->
          getStartPosition ()];
      
  if (startingPartGroupDescrsList.size () == 0) {
    // first element, insert it directly
    startingPartGroupDescrsList.push_back (
      partGroupDescr);
  }

  else {
    // insert partGroupDescr at the right place in the starting list
    insertPartGroupDescInStartingList (
      inputLineNumber,
      partGroupDescr,
      startingPartGroupDescrsList);
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPartGroupDescrAsStopped (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescr)
{
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Registering part group descr '" <<
      partGroupDescr->getPartGroupCombinedName () <<
      " as stopped" <<
      ", line " << inputLineNumber <<
      endl;
  }

  // register part group descr as stopping at current position
  registerPartGroupDescrAsStoppingAtCurrentPosition (
    inputLineNumber,
    partGroupDescr);
  
  // forget it in fStartedPartGroupDescrsMap,
  // it remains in fAllPartGroupDescrsMap   
  fStartedPartGroupDescrsMap.erase (
    partGroupDescr->
      getPartGroup ()->
        getPartGroupNumber ());
    
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER forgetting part group descr " +
        partGroupDescr->
          partGroupDescrAsString () +
        " in the data");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStart (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Creating part group with number '" <<
      fCurrentPartGroupNumber <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsCounter++;

  // create the part group,
  // with the current part group as part group uplink
  S_msrPartGroup
    partGroupToBeStarted =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        fPartGroupsCounter,
        fCurrentPartGroupName,
        fCurrentPartGroupNameDisplayText,
        fCurrentPartGroupAccidentalText,
        fCurrentPartGroupAbbreviation,
        fCurrentPartGroupSymbolKind,
        fCurrentPartGroupSymbolDefaultX,
        msrPartGroup::kPartGroupImplicitNo,
        fCurrentPartGroupBarlineKind,
        0, // partGroupPartGroupUplink will be set upon 'stop'
        fMsrScore);

  // partGroupToBeStarted will be appended to the MSR score
  // upon 'stop', once it is complete

  // create the part group descr
  S_mxmlPartGroupDescr
    partGroupDescr =
      mxmlPartGroupDescr::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        partGroupToBeStarted,
        fCurrentPartsPosition) ;
             
  // register it in the part groups data
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding part group descr for '" << fCurrentPartGroupNumber <<
      "' to visitor's part groups data" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }

  registerPartGroupDescrAsStarted (
    inputLineNumber,
    partGroupDescr);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStart()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStop (
  int inputLineNumber)
{
  // is the part group to be stopped started?
  S_mxmlPartGroupDescr
    partGroupDescrToBeStopped =
      fetchStartedPartGroupDescr (
        fCurrentPartGroupNumber);

  if (! partGroupDescrToBeStopped) {
    // no, but we should have found it
    stringstream s;

    s <<
      "no part group '" << fCurrentPartGroupNumber <<
      "' has not been started, it cannot be stopped";
      
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // set the stopping position
  partGroupDescrToBeStopped->
    setStopPosition (
      inputLineNumber,
      fCurrentPartsPosition);

  // register partGroupDescrToBeStopped as stopped
  registerPartGroupDescrAsStopped (
    inputLineNumber,
    partGroupDescrToBeStopped);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStop()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::doNestPartGroupDescrInItsContainer (
  int                  inputLineNumber,
  S_mxmlPartGroupDescr partGroupDescrToBeStopped,
  S_mxmlPartGroupDescr containingPartGroupDescr)
{
  // get part groups from descrs
  S_msrPartGroup
    partGroupToBeStopped =
      partGroupDescrToBeStopped->
        getPartGroup (),
    containingPartGroup =
      containingPartGroupDescr->
        getPartGroup ();
        
  // set currentPartGroup's uplink to containingPartGroupDescr
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Setting the uplink of part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;      
  }

  partGroupToBeStopped->
    setPartGroupPartGroupUplink (
      containingPartGroup);

  // appending currentPartGroup to containingPartGroupDescr
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Appending sub part group " <<
      partGroupToBeStopped->
        getPartGroupCombinedName () <<
      "' to " <<
      containingPartGroup->
        getPartGroupCombinedName () << 
      ", line " << inputLineNumber <<
      endl;      
  }
  
  containingPartGroup->
    appendSubPartGroupToPartGroup (
      partGroupToBeStopped);
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::createImplicitPartGroup ()
{
  // an implicit part group has to be created to contain everything,
  // since there can be parts out of any part group

  // sanity check
  msrAssert (
    fImplicitPartGroup == 0,
    "fImplicitPartGroup already exists");
  
  int inputLineNumber = 0;
    // this occurs independantly from the MusicXML data

  // create an implicit part group
  fCurrentPartGroupNumber = 0;
  
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Creating an implicit part group with number '" <<
      fCurrentPartGroupNumber <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsCounter++;

  fImplicitPartGroup =
    msrPartGroup::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fPartGroupsCounter,
      "Implicit", // partGroupName
      "",         // PartGroupNameDisplayText
      "",         // partGroupAccidentalText
      "Impl.",    // partGroupAbbreviation
      msrPartGroup::k_NoPartGroupSymbol,
      0,          // partGroupSymbolDefaultX
      msrPartGroup::kPartGroupImplicitYes,
      msrPartGroup::kPartGroupBarlineYes,
      0,          // partGroupPartGroupUplink, 0 for top level part group 
      fMsrScore);

  // append it to the MSR score
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Appending implicit part group '" <<
      fImplicitPartGroup->getPartGroupNumber () <<
      "' to MSR score" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }
      
  fMsrScore->
    addPartGroupToScore (
      fImplicitPartGroup);

  // create the part group descr for the implicit part group
  fImplicitPartGroupDescr =
    mxmlPartGroupDescr::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fImplicitPartGroup,
      fCurrentPartsPosition);
      
  // register it in the part groups data
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding implicit part group descr for '" <<
      fCurrentPartGroupNumber <<
      "' to the part groups data" <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }

  registerPartGroupDescrAsStarted (
    inputLineNumber,
    fImplicitPartGroupDescr);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER creating fImplicitPartGroup");
  }
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::doPartGroupsNestingAndPartsAllocation (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "doPartGroupsNestingAndPartsAllocation:" <<
      endl;
  }

  // this is actually a partial subpass of pass_2a,
  // since we run through the contents of <part-list />,
  // stored in the data we've built, a second time
  
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE doPartGroupsNestingAndPartsAllocation");
  }

  // handle each position in turn
  for (int k = 0; k <= fCurrentPartsPosition; k++) {

    if (k > 0) {
      // parts actual positions start at 1
      // append part to current part group, i.e. to the top of the stack
      S_msrPart
        part =
          fPartsVector [k - 1];
          
      // fetch the part group descr stack top
      S_mxmlPartGroupDescr
        partGroupsDescrStackTop =
          fetchPartGroupDescrStackTop ();
  
      if (! partGroupsDescrStackTop) {
        stringstream s;
        
        s <<
          "Cannot append part " <<
          part->getPartCombinedName () <<
          " to any part group " <<
          " at position " << k <<
          ", since the stack is empty";
          
        msrMusicXMLError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());            
      }

      // append part to current part group
      S_msrPartGroup
        currentPartGroup =
          partGroupsDescrStackTop->getPartGroup ();

      currentPartGroup->
        appendPartToPartGroup (
          part);
    }
    
    // handle the part groups descrs stopping at position k
    if (fPositionStoppingPartGroupDescrsVector.size ()) {
      list<S_mxmlPartGroupDescr>&
        stoppingPartGroupDescrsList =
          fPositionStoppingPartGroupDescrsVector [k];
  
      if (stoppingPartGroupDescrsList.size ()) {
        gIndenter++;
  
        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = stoppingPartGroupDescrsList.begin (),
          iEnd   = stoppingPartGroupDescrsList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);

          int stopInputLineNumber =
            partGroupDescr->getStopInputLineNumber ();
            
          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();
      
          // fetch the part group descr stack top
          S_mxmlPartGroupDescr
            partGroupsDescrStackTop =
              fetchPartGroupDescrStackTop ();

          if (! partGroupsDescrStackTop) {
            stringstream s;
            
            s <<
              "Cannot 'stop' part group descr " <<
              partGroupDescr->getPartGroupCombinedName () <<
              " at position " << k <<
              ", since the stack is empty";
              
            msrMusicXMLError (
              gGeneralOptions->fInputSourceName,
              stopInputLineNumber,
              __FILE__, __LINE__,
              s.str ());            
          }
  
          if (partGroupsDescrStackTop == partGroupDescr) {
            // pop partGroupDescrToBeStopped from the stack
            S_mxmlPartGroupDescr
              partGroupsDescrStackTop =
                fPartGroupsDescrStack.front ();

            if (gGeneralOptions->fTracePartGroups) {
              fLogOutputStream <<
                "Popping part group " <<
                partGroupDescr->
                  getPartGroupCombinedName () <<
                "' from the stack" <<
                ", line " << stopInputLineNumber <<
                endl;      
            }
        
            fPartGroupsDescrStack.pop_front ();

            // the implicit part group isn't contained in any other
            if (partGroupDescr != fImplicitPartGroupDescr) {
              // fetch new current part group
              S_mxmlPartGroupDescr
                newPartGroupDescrStackTop =
                  fetchPartGroupDescrStackTop ();
          
              if (! newPartGroupDescrStackTop) {
                stringstream s;
            
                s <<
                  "there is no part group in the stack to nest part group descr " <<
                  partGroupDescr->partGroupDescrAsString () <<
                  fCurrentPartID << "\"" <<
                  " into";
                  
                msrInternalError (
                  gGeneralOptions->fInputSourceName,
                  stopInputLineNumber,
                  __FILE__, __LINE__,
                  s.str ());
              }
      
              // partGroupDescr is nested in newPartGroupDescrStackTop,
              // do the nesting
              doNestPartGroupDescrInItsContainer (
                stopInputLineNumber,
                partGroupDescr,
                newPartGroupDescrStackTop);
            }
          }

          else {
            showPartGroupsData (
              stopInputLineNumber,
              "UPON overlapping part groups");

            // fetch the positions in the intersection
            int
              startOne =
                partGroupDescr->getStartPosition (),
              startTwo =
                partGroupsDescrStackTop->getStartPosition (),
              stopOne =
                partGroupDescr->getStopPosition (),
              stopTwo =
                partGroupsDescrStackTop->getStopPosition ();

            int firstCommonPosision = startOne;
            if (startTwo > startOne)
              firstCommonPosision = startTwo;
              
            int lastCommonPosision = stopOne;
            if (stopTwo < stopOne)
              lastCommonPosision = stopTwo;
              
            stringstream s;
              
            s <<
              endl <<
              "There are overlapping part groups, namely: " <<
              endl <<
              gTab << partGroupDescr->partGroupDescrAsString () <<
              endl <<
              "and" <<
              endl <<
              gTab << partGroupsDescrStackTop->partGroupDescrAsString () <<
              endl <<
              endl <<
              "The parts they share are:" <<
              endl;

            if (gGeneralOptions->fTracePartGroups) {
              s <<
                "(positions range is " <<
                firstCommonPosision << ".." << lastCommonPosision <<
                ")" <<
                endl;
            }

            for (int m = firstCommonPosision; m < lastCommonPosision; m++) {
              S_msrPart
                part =
                  fPartsVector [m];
                
              s <<
                gTab <<
                part->getPartCombinedName () <<
                ", line " << part->getInputLineNumber () <<
                endl;
            }

            s <<
              endl <<
R"(Please contact the maintainers of xml2ly (see '-c, -contact'):
  either you found a bug in the translator,
  or this MusicXML data is the first-ever real-world case
  of a score exhibiting overlapping part groups)";
              
            msrMusicXMLError (
              gGeneralOptions->fInputSourceName,
              stopInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
    
          if (++i == iEnd) break;
          // no endl here
        } // for

        gIndenter--;
      }
    }
        
    // handle the part groups descrs starting at position k
    if (fPositionStartingPartGroupDescrsVector.size ()) {
      list<S_mxmlPartGroupDescr>&
        startingPartGroupDescrsList =
          fPositionStartingPartGroupDescrsVector [k];
  
      if (startingPartGroupDescrsList.size ()) {
        gIndenter++;
  
        list<S_mxmlPartGroupDescr>::const_iterator
          iBegin = startingPartGroupDescrsList.begin (),
          iEnd   = startingPartGroupDescrsList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_mxmlPartGroupDescr
            partGroupDescr = (*i);
            
          S_msrPartGroup
            partGroup =
              partGroupDescr->getPartGroup ();
      
          // make it the new current part group
          if (gGeneralOptions->fTracePartGroups) {
            fLogOutputStream <<
              "Pushing part group descr '" <<
              partGroupDescr->partGroupDescrAsString () <<
              "' onto the part group stack" <<
              ", line " << inputLineNumber <<
              endl;
          }
        
          fPartGroupsDescrStack.push_front (
            partGroupDescr);

          if (++i == iEnd) break;
          // no endl here
        } // for

        gIndenter--;
      }
    }
        
    if (gGeneralOptions->fTracePartGroupsDetails) {
      showPartGroupsData (
        inputLineNumber,
        "AT position " + k);
    }
  
  } // for
      
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER doPartGroupsNestingAndPartsAllocation");
  }
}

//______________________________________________________________________________
S_msrStaff mxmlTree2MsrSkeletonBuilder::createStaffIfNotYetDone (
  int            inputLineNumber,
  int            staffNumber)
{    
  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) {
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaff::kRegularStaff,
          staffNumber);
  }

  return staff;
}  

//______________________________________________________________________________
S_msrVoice mxmlTree2MsrSkeletonBuilder::createVoiceIfNotYetDone (
  int inputLineNumber,
  int staffNumber,
  int voiceNumber)
{
  // the voice number is relative to a part,
  // we'll call it its part-relative ID

  // create the staff if not yet done
  S_msrStaff
    staff =
      createStaffIfNotYetDone (
        inputLineNumber,
        staffNumber);

  // is voice already present in staff?
  S_msrVoice
    voice =
      staff->
        fetchVoiceFromStaffByItsPartRelativeID (
          inputLineNumber,
          voiceNumber);
  
  if (! voice) {
    // create the voice and append it to the staff
    voice =
      staff->
        createVoiceInStaffByItsPartRelativeID (
          inputLineNumber,
          msrVoice::kRegularVoice,
          voiceNumber,
          fCurrentMeasureNumber);
  }
  
  return voice;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_score_partwise& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_partwise" <<
      endl;
  }

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analysing the score partwise" <<
      endl;

  fScoreNumberOfMeasures = 0;
}
/*
  <part-list>
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P2">
*/

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_score_partwise& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_score_partwise" <<
      endl;
  }

  fMsrScore->
    setScoreNumberOfMeasures (
      fScoreNumberOfMeasures);
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_list" <<
      endl;
  }

/*
  <part-list>
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P2">
*/

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Analysing part list" <<
      endl;

  gIndenter++;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_list& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_list" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  gIndenter--;

  // set implicit part group descr end position
  fImplicitPartGroupDescr->
    setStopPosition (
      inputLineNumber, // JMI ???
      fCurrentPartsPosition);

  // register implicit part group descr as stopped
  registerPartGroupDescrAsStopped (
    inputLineNumber,
    fImplicitPartGroupDescr);

  // do the job...
  doPartGroupsNestingAndPartsAllocation (
    inputLineNumber);

  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsData (
      inputLineNumber,
      "Part groups data gathered for score skeleton");
  }

  if (gMsrOptions->fDisplayPartGroups) {
    fLogOutputStream <<
      endl <<
      "The implicit part group contains:" <<
      endl;

    gIndenter++;
    
    fImplicitPartGroup->
      printPartGroupParts (
        inputLineNumber,
        fLogOutputStream);

    gIndenter--;
  }
}

//________________________________________________________________________
/*
    The part-group element indicates groupings of parts in the
    score, usually indicated by braces and brackets. Braces
    that are used for multi-staff parts should be defined in
    the attributes element for that part. The part-group start
    element appears before the first score-part in the group.
    The part-group stop element appears after the last
    score-part in the group.
    
    The number attribute is used to distinguish overlapping
    and nested part-groups, not the sequence of groups. As
    with parts, groups can have a name and abbreviation.
    Formatting attributes for group-name and group-abbreviation
    are deprecated in Version 2.0 in favor of the new
    group-name-display and group-abbreviation-display elements.
    Formatting specified in the group-name-display and
    group-abbreviation-display elements overrides formatting
    specified in the group-name and group-abbreviation
    elements, respectively.

    The group-symbol element indicates how the symbol for
    a group is indicated in the score. Values include none,
    brace, line, bracket, and square; the default is none.
    The group-barline element indicates if the group should
    have common barlines. Values can be yes, no, or
    Mensurstrich. The group-time element indicates that the
    displayed time signatures should stretch across all parts
    and staves in the group. Values for the child elements
    are ignored at the stop of a group.

    A part-group element is not needed for a single multi-staff
    part. By default, multi-staff parts include a brace symbol
    and (if appropriate given the bar-style) common barlines.
    The symbol formatting for a multi-staff part can be more
    fully specified using the part-symbol element, defined in
    the attributes.mod file.
*/

/*
  There is no hierarchy implied in part-group elements.
  All that matters is the sequence of part-group elements
  relative to score-part elements.
  The sequencing of two consecutive part-group elements does not matter.
  It is the default-x attribute that indicates
  the left-to-right ordering of the group symbols.

  <part-group number="1" type="start">
  <group-name>Trombones</group-name>
  <group-abbreviation>Trb.</group-abbreviation>
  <group-symbol default-x="-12">brace</group-symbol>
  <group-barline>yes</group-barline>
  </part-group>

    <part-group number="3" type="start">
      <group-name>Trumpet in B flat</group-name>
      <group-name-display>
        <display-text>Trumpet in B</display-text>
        <accidental-text>flat</accidental-text>
      </group-name-display>
      <group-abbreviation>Trp. in B flat</group-abbreviation>
      <group-abbreviation-display>
        <display-text>Trp. in B</display-text>
        <accidental-text>flat</accidental-text>
      </group-abbreviation-display>
      <group-symbol default-x="-10">square</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
*/

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_group& elt)
{  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_group" <<
      endl;
  }

/*
<!--
  The part-group element indicates groupings of parts in the
  score, usually indicated by braces and brackets. Braces
  that are used for multi-staff parts should be defined in
  the attributes element for that part. The part-group start
  element appears before the first score-part in the group.
  The part-group stop element appears after the last 
  score-part in the group.
  
  The number attribute is used to distinguish overlapping
  and nested part-groups, not the sequence of groups. As
  with parts, groups can have a name and abbreviation.
  Formatting attributes for group-name and group-abbreviation
  are deprecated in Version 2.0 in favor of the new
  group-name-display and group-abbreviation-display elements.
  Formatting specified in the group-name-display and
  group-abbreviation-display elements overrides formatting
  specified in the group-name and group-abbreviation
  elements, respectively.

  The group-symbol element indicates how the symbol for
  a group is indicated in the score. Values include none,
  brace, line, bracket, and square; the default is none.
  The group-barline element indicates if the group should
  have common barlines. Values can be yes, no, or
  Mensurstrich. The group-time element indicates that the
  displayed time signatures should stretch across all parts
  and staves in the group. Values for the child elements
  are ignored at the stop of a group. 

  A part-group element is not needed for a single multi-staff
  part. By default, multi-staff parts include a brace symbol
  and (if appropriate given the bar-style) common barlines.
  The symbol formatting for a multi-staff part can be more
  fully specified using the part-symbol element, defined in
  the attributes.mod file.
-->
<!ELEMENT part-group (group-name?, group-name-display?,
  group-abbreviation?, group-abbreviation-display?,
  group-symbol?, group-barline?, group-time?, %editorial;)>
<!ATTLIST part-group
    type %start-stop; #REQUIRED
    number CDATA "1"
>

<!ELEMENT group-name (#PCDATA)>
<!ATTLIST group-name
    %print-style;
    %justify;
>
<!ELEMENT group-name-display
  ((display-text | accidental-text)*)>
<!ATTLIST group-name-display
    %print-object;
>
<!ELEMENT group-abbreviation (#PCDATA)>
<!ATTLIST group-abbreviation
    %print-style;
    %justify;
>
<!ELEMENT group-abbreviation-display
  ((display-text | accidental-text)*)>
<!ATTLIST group-abbreviation-display
    %print-object;
>

<!ELEMENT group-symbol (#PCDATA)>
<!ATTLIST group-symbol
    %position;
    %color;
>

<!ELEMENT group-barline (#PCDATA)>
<!ATTLIST group-barline
    %color;
>
<!ELEMENT group-time EMPTY>

*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  // part group number
  
  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);
    
  // part group type
  
  string partGroupType = elt->getAttributeValue ("type");

  fCurrentPartGroupTypeKind = msrPartGroup::k_NoPartGroupType;
    
  if      (partGroupType == "start")
    fCurrentPartGroupTypeKind = msrPartGroup::kStartPartGroupType;
  else if (partGroupType == "stop")
    fCurrentPartGroupTypeKind = msrPartGroup::kStopPartGroupType;
  else {
    stringstream s;

    s <<
      "unknown part group type \"" <<
       partGroupType <<
        "\"";

    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // part group print style

  string printStyle = elt->getAttributeValue ("print-style"); // JMI
  
  // part group print object

  string printObject = elt->getAttributeValue ("print-object"); // JMI

  // handle part group type
  switch (fCurrentPartGroupTypeKind) {
    
    case msrPartGroup::kStartPartGroupType:
      fCurrentPartGroupName = "";
      fCurrentPartGroupNameDisplayText = "";
      fCurrentPartGroupAccidentalText = "";
      fCurrentPartGroupAbbreviation = "";
      fCurrentPartGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
      fCurrentPartGroupSymbolDefaultX = INT_MIN;
      fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineYes;
      break;
      
    case msrPartGroup::kStopPartGroupType:
      break;
      
    case msrPartGroup::k_NoPartGroupType:
      // should not occur
      break;
  } // switch
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name" <<
      endl;
  }

  fCurrentPartGroupName = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_name_display" <<
      endl;
  }

  fOnGoingPartGroupNameDisplay = true;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_group_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_group_name_display" <<
      endl;
  }

  fOnGoingPartGroupNameDisplay = false;
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_display_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_text" <<
      endl;
  }

/*
    <score-part id="P2">
      <part-name>Part name</part-name>
      <part-name-display><display-text>Overridden Part Name</display-text></part-name-display>
      <part-abbreviation>abbrv.</part-abbreviation>
      <part-abbreviation-display><display-text>Overr.abbrv.</display-text></part-abbreviation-display>
    </score-part>
*/

  if (fOnGoingPartGroupNameDisplay)
    fCurrentPartGroupNameDisplayText = elt->getValue();
  else if (fOnGoingPartNameDisplay)
    fCurrentPartNameDisplayText = elt->getValue();
  else if (fOnGoingPartAbbreviationDisplay)
    fCurrentPartAbbreviationDisplayText = elt->getValue();
  else {
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      "<display-text /> is out of context");
  }
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_accidental_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_accidental_text" <<
      endl;
  }

    fCurrentPartGroupAccidentalText = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_abbreviation" <<
      endl;
  }

  fCurrentPartGroupAbbreviation = elt->getValue ();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_group_symbol& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_symbol" <<
      endl;
  }

  string groupSymbol = elt->getValue ();

  fCurrentPartGroupSymbolKind =
    msrPartGroup::k_NoPartGroupSymbol; // default value
   
  if      (groupSymbol == "brace")
    fCurrentPartGroupSymbolKind = msrPartGroup::kBracePartGroupSymbol;
    
  else if (groupSymbol == "bracket")
    fCurrentPartGroupSymbolKind = msrPartGroup::kBracketPartGroupSymbol;
    
  else if (groupSymbol == "line")
    fCurrentPartGroupSymbolKind = msrPartGroup::kLinePartGroupSymbol;
    
  else if (groupSymbol == "square")
    fCurrentPartGroupSymbolKind = msrPartGroup::kSquarePartGroupSymbol;
    
  else if (groupSymbol == "none")
    fCurrentPartGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
    
  else {
    if (groupSymbol.size ()) {
      // part group type may be absent
      stringstream s;

      s <<
        "unknown part group symbol \"" + groupSymbol + "\"";
        
      msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
        elt->getInputLineNumber (),
        __FILE__, __LINE__,
        s.str ());
    }
  }

  fCurrentPartGroupSymbolDefaultX =
    elt->getAttributeIntValue ("default-x", 0);
}

void mxmlTree2MsrSkeletonBuilder::visitStart ( S_group_barline& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_group_barline" <<
      endl;
  }

  string groupBarline = elt->getValue ();
  
  // check part group barline
  if      (groupBarline == "yes")
    fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineYes;
  else if (groupBarline == "no")
    fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineNo;
  else {
    stringstream s;

    s <<
      "unknown part group barline \"" + groupBarline + "\"";
      
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_group& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_group" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Handling part group '" <<
      fCurrentPartGroupNumber <<
      "', type: \"" <<
      msrPartGroup::partGroupTypeKindAsString (
        fCurrentPartGroupTypeKind) << "\""  <<
      ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter++;
  
  // handle part group type
  switch (fCurrentPartGroupTypeKind) {
    
    case msrPartGroup::kStartPartGroupType:
      // handle the part group start
      handlePartGroupStart (
        inputLineNumber);
      break;
      
    case msrPartGroup::kStopPartGroupType:
      // handle the part group stop
      handlePartGroupStop (
        inputLineNumber);
      break;

    case msrPartGroup::k_NoPartGroupType:
      // should not occur
      break;
  } // switch

  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_score_part" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fTraceParts)
    fLogOutputStream <<
      "Found part name \"" << fCurrentPartID << "\"" <<
      " in part list" <<
      ", line " << inputLineNumber <<
      endl;

  fCurrentPartName = "";
  fCurrentPartNameDisplayText = "";
  
  fCurrentPartAbbreviation = "";
  fCurrentPartAbbreviationDisplayText = "";
  
  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_name" <<
      endl;
  }

/*
<!--
  The part-name indicates the full name of the musical part.
  The part-abbreviation indicates the abbreviated version of
  the name of the musical part. The part-name will often
  precede the first system, while the part-abbreviation will
  precede the other systems. The formatting attributes for
  these elements are deprecated in Version 2.0 in favor of
  the new part-name-display and part-abbreviation-display
  elements. These are defined in the common.mod file as they
  are used in both the part-list and print elements. They
  provide more complete formatting control for how part names
  and abbreviations appear in a score.
-->
<!ELEMENT part-name (#PCDATA)>
<!ATTLIST part-name
    %print-style;
    %print-object;
    %justify;
>
<!ELEMENT part-abbreviation (#PCDATA)>
<!ATTLIST part-abbreviation
    %print-style;
    %print-object;
    %justify;
>
*/

  fCurrentPartName = elt->getValue ();
  
  string printStyle = elt->getAttributeValue ("print-style"); // JMI
  if (printStyle == "JMI") {
  }
  
  string printObject = elt->getAttributeValue ("print-object"); // JMI
  if (printObject == "JMI") {
  }
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_name_display" <<
      endl;
  }

  fOnGoingPartNameDisplay = true;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_name_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_name_display" <<
      endl;
  }

  fOnGoingPartNameDisplay = false;
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_abbreviation" <<
      endl;
  }

  fCurrentPartAbbreviation = elt->getValue ();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_part_abbreviation_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part_abbreviation_display" <<
      endl;
  }

  fOnGoingPartAbbreviationDisplay = true;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part_abbreviation_display& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part_abbreviation_display" <<
      endl;
  }

  fOnGoingPartAbbreviationDisplay = false;
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_instrument_name& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_name" <<
      endl;
  }

  fCurrentPartInstrumentName = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_instrument_abbreviation& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_instrument_abbreviation" <<
      endl;
  }

  fCurrentPartInstrumentAbbreviation = elt->getValue();
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_score_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_score_part" <<
      endl;
  }

  fCurrentPartsPosition++;

  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceParts)
   fLogOutputStream <<
    "--------------------------------------------" <<
    endl <<
    "Handling score part name \"" << fCurrentPartID << "\"" <<
    ", fCurrentPartsPosition = " << fCurrentPartsPosition <<
    ", line " << inputLineNumber <<
    endl;

  gIndenter++;

  // create the part
  if (gGeneralOptions->fTraceParts)
   fLogOutputStream <<
    "--------------------------------------------" <<
    endl <<
    "Creating part \"" << fCurrentPartID << "\"" <<
    ", line " << inputLineNumber <<
    endl;

  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        fCurrentPartID,
        0); // partPartGroupUplink will be set later
        
  // populate it
  part->
    setPartName (fCurrentPartName);
  part->
    setPartNameDisplayText (fCurrentPartNameDisplayText);

  part->
    setPartAbbreviation (fCurrentPartAbbreviation);
  part->
    setPartAbbreviationDisplayText (fCurrentPartAbbreviationDisplayText);

  part->
    setPartInstrumentName (fCurrentPartInstrumentName);
  part->
    setPartInstrumentAbbreviation (fCurrentPartInstrumentAbbreviation);

  // register part in the parts data
  registerPart (
    inputLineNumber,
    fCurrentPartsPosition,
    part);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handling score part \"" + partID + "\"");
  }
  
  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPart (
  int       inputLineNumber,
  int       partPosition,
  S_msrPart part)
{
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Resitering part " <<
      part->getPartCombinedName () <<
      " in the parts data" <<
      ", partPosition = " << partPosition <<
      ", line " << inputLineNumber <<
      endl;
  }

  // register it parts vector
  fPartsVector.push_back (part);
    
  // register it parts map
  fPartsMap [part->getPartID ()] = part;

  // create an empty list for part groups starting at partPosition  
  fPositionStartingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());

  // create an empty list for part groups stopping at partPosition  
  fPositionStoppingPartGroupDescrsVector.push_back (
    list<S_mxmlPartGroupDescr> ());
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_part" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fTraceParts || gGeneralOptions->fTraceGeneral) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << fCurrentPartID << "\"" <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }

  if (! fCurrentPartID.size ()) {
    if (fPartsMap.size () == 1) {
      // there's only one part in the part list,
      // assume this is the one
      fCurrentPartID =
        (*fPartsMap.begin ()).second->
          getPartID ();

      stringstream s;

      s <<
        "part id is empty, using '" <<
        fCurrentPartID <<
        "' since it is the only part in the <part-list />";

      msrMusicXMLWarning (
        inputLineNumber,
        s.str ());
    }

    else {
      fCurrentNoIDPartNumber++;
      
      stringstream s;
  
      s <<
        "NO_ID_PART_" << fCurrentNoIDPartNumber;
  
      s >> fCurrentPartID;
  
      msrMusicXMLWarning (
        inputLineNumber,
        "part 'id' is empty, creating one as '" + fCurrentPartID + "'");
    }
  }
    
  // is this part already known?
  if (fPartsMap.count (fCurrentPartID)) {
    fCurrentPart = // used thoughoutfRepeatHasBeenCreatedForCurrentPart
      fPartsMap [fCurrentPartID];
  }
      
  else {
    stringstream s;

    s <<
      "part \"" <<
      fCurrentPartID <<
      "\" is not known in the <part-list />";
    
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (gGeneralOptions->fTraceParts) {
    fLogOutputStream <<
      endl <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;
  }

  gIndenter++;

  // measures
  fPartNumberOfMeasures = 0;
    
  // staves and voices
  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_part& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_part" <<
      endl;
  }

  gIndenter--;

  if (gGeneralOptions->fTraceParts) {
    fLogOutputStream <<
      "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
      endl <<
      "--------------------------------------------" <<
      endl <<
      endl;
  }

  // handle current part number of measures
  if (fScoreNumberOfMeasures == 0) {
    fScoreNumberOfMeasures =
      fPartNumberOfMeasures;
  }
  else {
    // is the part number of measures consistent?
    if (fPartNumberOfMeasures != fScoreNumberOfMeasures) {
      stringstream s;

      s <<
        "part " << fCurrentPart->getPartCombinedName () <<
        " has " << fPartNumberOfMeasures <<
        " while the other ones have " << fScoreNumberOfMeasures;
        
      msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
        elt->getInputLineNumber (),
      __FILE__, __LINE__,
        s.str ());
    }
  }
  
  // set current part's number of measures
  fCurrentPart->
    setPartNumberOfMeasures (
      fPartNumberOfMeasures);
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_staves& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_direction" <<
      endl;
  }

  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceStaves) {
    switch (stavesNumber) {
      case 0:
        fLogOutputStream <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;
        
      case 1:
        fLogOutputStream <<
          "There is 1 staff";
        break;
        
      default:
        fLogOutputStream <<
          "There are " << stavesNumber << " staves";
    } // switch
    
    fLogOutputStream <<
      " in part " << fCurrentPart->getPartCombinedName() <<
      endl;
  }

  if (stavesNumber > 1) {
    // add n-1 staves to current part since 1 already exists JMI
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber,
          msrStaff::kRegularStaff,
          n);
      n++;
    } // while
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_staff& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff" <<
      endl;
  }

  fCurrentStaffNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  // the staff number should be positive
  if (fCurrentStaffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentStaffNumber <<
      " is not positive";
      
    msrAssert (false, s.str ());
  }
  
  S_msrStaff
    staff =
      createStaffIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber);

  if (gGeneralOptions->fTraceStaves) {
    fLogOutputStream <<
      "--> S_staff, fCurrentStaffNumber = " <<
      fCurrentStaffNumber << endl <<
    "--> S_staff, current staff name  = " <<
      staff->getStaffName() <<
      endl;
  }

  if (fOnGoingNote) {
    // regular staff indication in note/rest
    fCurrentNoteStaffNumber = fCurrentStaffNumber;
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_voice& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_staff_details" <<
      endl;
  }

  fCurrentVoiceNumber = int(*elt);
  
  int inputLineNumber =
    elt->getInputLineNumber ();
  
  if (fOnGoingNote) {
    // regular voice indication in note/rest
    fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffIfNotYetDone (
          inputLineNumber,
          fCurrentNoteStaffNumber);
  
    if (gGeneralOptions->fTraceNotes && gGeneralOptions->fTraceVoices) {
      fLogOutputStream <<
        "--> fCurrentNoteVoiceNumber        = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
        "--> S_voice, fCurrentNoteStaffNumber = " <<
        fCurrentNoteStaffNumber <<
        endl <<
        "--> S_voice, current staff name  = " <<
        staff->getStaffName() <<
        endl;
    }

    S_msrVoice
      currentVoice = // ??? JMI
        createVoiceIfNotYetDone (
          inputLineNumber,
          fCurrentStaffNumber,
          fCurrentVoiceNumber);
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_measure" <<
      endl;
  }

/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  fCurrentMeasureNumber = // JMI local variable???
    elt->getAttributeValue ("number");

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceGeneral) {
    fLogOutputStream <<
      endl <<
      "<!--=== measure " << fCurrentMeasureNumber <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }

  // take this measure into account
  fPartNumberOfMeasures++;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd (S_measure& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_measure" <<
      endl;
  }
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_note& elt ) 
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_note" <<
      endl;
  }
  
  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;
    
  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent

  fOnGoingNote = true;
}

void mxmlTree2MsrSkeletonBuilder::visitEnd ( S_note& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_note" <<
      endl;
  }
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  // store voice and staff numbers in MusicXML note data
  fCurrentNoteStaffNumber = fCurrentStaffNumber;
  fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);
  
  fOnGoingNote = false;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart (S_lyric& elt )
{
  /*
      <note default-x="143">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>4</octave>
        </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem default-y="-5">up</stem>
        <beam number="1">begin</beam>
        
        <lyric default-y="-80" justify="left" number="1">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">1.</text>
          <elision> </elision>
          <syllabic>begin</syllabic>
          <text font-family="FreeSerif" font-size="11">A</text>
        </lyric>
      </note>
  */
  
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_lyric" <<
      endl;
  }

  string stanzaNumber =
    elt->getAttributeValue ("number");
  
  if (stanzaNumber.size () == 0) {
    msrMusicXMLWarning (
      elt->getInputLineNumber (),
      "lyric number is empty");
  }
  
  else {
    if (gGeneralOptions->fTraceLyrics)
      fLogOutputStream <<
        "--> setting fCurrentStanzaNumber to " <<
        stanzaNumber <<
        ", line " << elt->getInputLineNumber () <<
        endl;
        
    // register it as current stanza number, JMI
    // that remains set another positive value is met,
    // thus allowing a skip syllable to be generated
    // for notes without lyrics
    fCurrentStanzaNumber = stanzaNumber;
  }
}

void mxmlTree2MsrSkeletonBuilder::visitEnd ( S_lyric& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> End visiting S_lyric" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceLyrics) {
    fLogOutputStream <<
      endl <<
      "visitEnd ( S_lyric& )" <<
      ", line = " << inputLineNumber <<
      ", with:" <<
      endl;

    gIndenter++;

    fLogOutputStream <<
      "Lyric data:" <<
      endl;

    {
      gIndenter++;

      const int fieldwidth = 28;
  
      fLogOutputStream << left <<
        setw (fieldwidth) <<
        "fCurrentStanzaNumber" << " = " << fCurrentStanzaNumber <<
        endl;
          
      gIndenter--;
    }
    
    gIndenter--;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      createVoiceIfNotYetDone (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  // fetch stanzaNumber in current voice
  S_msrStanza
    stanza =
      currentVoice->
        createStanzaInVoiceIfNotYetDone (
          inputLineNumber,
          fCurrentStanzaNumber);
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_harmony& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_harmony" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();

  // append a harmony staff and voice to the current part
  fCurrentPart->
    createPartHarmonyStaffAndVoiceIfNotYetDone (
      inputLineNumber);
}

//______________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::visitStart ( S_figured_bass& elt )
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_figured_bass" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  // append a figured bass staff and voice to the current part
  fCurrentPart->
    createPartFiguredStaffAndVoiceIfNotYetDone (
      inputLineNumber);
}


} // namespace
