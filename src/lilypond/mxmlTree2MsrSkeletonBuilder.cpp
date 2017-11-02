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
  fOnGoingGroupNameDisplay = false;
  fPartGroupsCounter = 0;
  
  // parts handling
  fCurrentNoIDPartNumber = 0;
  fPartsCounter = 0;
  
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
S_msrPartGroup mxmlTree2MsrSkeletonBuilder::fetchPartGroupFromTheMap (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fPartGroupsMap.count (partGroupNumber)) {
    result =
      fPartGroupsMap [partGroupNumber];
  }

  return result;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsMap (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartGroupsMap:" <<
    endl;
    
  if (fPartGroupsMap.size ()) {
    gIndenter++;
    
    map<int, S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsMap.begin (),
      iEnd   = fPartGroupsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        "'" << (*i).first <<
        "' --> " <<
        (*i).second->getPartGroupCombinedName () <<
        ", instrument: \"" <<
        (*i).second->getPartGroupInstrumentName () <<
        "\"" <<   
        ", line " << inputLineNumber <<
        endl;
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
void mxmlTree2MsrSkeletonBuilder::showPartGroupsStartPositionsMap (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartGroupsStartPositionsMap:" <<
    endl;
    
  if (fPartGroupsStartPositionsMap.size ()) {
    gIndenter++;
    
    map<S_msrPartGroup, int>::const_iterator
      iBegin = fPartGroupsStartPositionsMap.begin (),
      iEnd   = fPartGroupsStartPositionsMap.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        (*i).first->getPartGroupCombinedName () <<
        " starts at position " <<
        (*i).second <<
        ", line " << inputLineNumber <<
        endl;
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
void mxmlTree2MsrSkeletonBuilder::showPendingPartGroupsToBeStoppedList (
  int inputLineNumber)
{
  fLogOutputStream <<
    "fPendingPartGroupsToBeStoppedList:" <<
    endl;
    
  if (fPendingPartGroupsToBeStoppedList.size ()) {
    gIndenter++;
    
    list<S_msrPartGroup>::const_iterator
      iBegin = fPendingPartGroupsToBeStoppedList.begin (),
      iEnd   = fPendingPartGroupsToBeStoppedList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        (*i)->getPartGroupCombinedName () <<
        " starts at position " <<
        fPartGroupsStartPositionsMap [(*i)] <<
        ", line " << inputLineNumber <<
        endl;
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
    "PartGroupsStack:" <<
    endl;

  if (! fPartGroupsStack.empty ()) {

    gIndenter++;

    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsStack.begin (),
      iEnd   = fPartGroupsStack.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        "v " <<
        (*i)->getPartGroupCombinedName () <<
        " starts at position " <<
        fPartGroupsStartPositionsMap [(*i)] <<
        ", line " << inputLineNumber <<
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
void mxmlTree2MsrSkeletonBuilder::showPartGroupsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartGroupsVector:" <<
    endl;
    
  gIndenter++;
  
  for (unsigned int i = 0; i < fPartGroupsVector.size (); i++) {
    fLogOutputStream <<
      i + 1 << ": " <<
      fPartGroupsVector [i]->getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  } // for
  
  fLogOutputStream <<
    "------------------" <<
    endl;
    
  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartsVector (
  int inputLineNumber)
{
  fLogOutputStream <<
    "PartsVector:" <<
    endl;
    
  gIndenter++;
  
  for (unsigned int i = 0; i < fPartsVector.size (); i++) {
    S_msrPart
      part = fPartsVector [i];
      
    fLogOutputStream <<
      i + 1 << ": " <<
      part->getPartCombinedName () <<
      ", uplink to " <<
      part->
        getPartPartGroupUplink ()->
          getPartGroupCombinedName () <<
      ", line " << inputLineNumber <<
      endl;
  } // for
  
  fLogOutputStream <<
    "------------------" <<
    endl;
    
  gIndenter--;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsData (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    endl <<
    context <<
    ", partsCounter = " << fPartsCounter <<
    ", line " << inputLineNumber <<
    ":" <<
    endl <<
    ">>> ================================================" <<
    endl;
    
  showPartGroupsMap (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPartsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPartGroupsVector (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPartGroupsStartPositionsMap (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPartGroupsStack (
    inputLineNumber);
  fLogOutputStream <<
    endl;
  
  showPendingPartGroupsToBeStoppedList (
    inputLineNumber);

  fLogOutputStream <<
    "<<< ================================================" <<
    endl <<
    endl;
}

/*
//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showStartedPartGroupsSet (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    endl <<
    "==> " << context <<
    ", line " << inputLineNumber <<
    ", fPartGroupsMap contains:" <<
    endl;
    
  if (fStartedPartGroupsSet.size ()) {
    gIndenter++;
    
    set<S_msrPartGroup>::const_iterator
      iBegin = fStartedPartGroupsSet.begin (),
      iEnd   = fStartedPartGroupsSet.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        (*i)->getPartGroupCombinedName () <<
        endl;
      if (++i == iEnd) break;
      // no endl here
    } // for
    
    gIndenter--;
  }
  
  else {
    fLogOutputStream <<
      gTab << "empty set" <<
      endl;
  }
      
  fLogOutputStream <<
    "------------------" <<
    endl;
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::showPartGroupsList (
  int    inputLineNumber,
  string context)
{
  fLogOutputStream <<
    endl <<
    "==> " << context <<
    ", line " << inputLineNumber <<
    ", fPartGroupsList contains:" <<
    endl;
    
  if (fPartGroupsList.size ()) {
    gIndenter++;
    
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLogOutputStream <<
        (*i)->getPartGroupCombinedName () <<
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
      
  fLogOutputStream <<
    "------------------" <<
    endl;
}
*/

//________________________________________________________________________
S_msrPartGroup mxmlTree2MsrSkeletonBuilder::fetchCurrentPartGroupFromStack ()
{
  S_msrPartGroup result;

  // the current part group is the top of the stack,
  // i.e. the front for the list used to implement it
  if (fPartGroupsStack.size () != 0) {
   result =
    fPartGroupsStack.front ();
  }

  return result;
}
  
//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::registerPartGroupInData (
  int            inputLineNumber,
  S_msrPartGroup partGroup)
{
  // register it temporarily in the part groups map
  // by its part gruop number, which may reused later by other part groups
  fPartGroupsMap [fCurrentPartGroupNumber] =
    partGroup;

  // register part group in groups vector, i.e. by appearance order
  fPartGroupsVector.push_back (
    partGroup);

  // register it part groups start positions map on first occurrence JMI ???
  if (! fPartGroupsStartPositionsMap.count (partGroup)) {
    fPartGroupsStartPositionsMap [partGroup] =
      fPartsCounter;
  }

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER registering part group " +
      partGroup->
        getPartGroupCombinedName () +
      " in the data");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::forgetPartGroupInData (
  int            inputLineNumber,
  S_msrPartGroup partGroup)
{
  // forget it in the part groups map         
  fPartGroupsMap.erase (
    partGroup->getPartGroupNumber ());
    
  // forget it in the part groups start positions map         

  fPartGroupsStartPositionsMap.erase (
    partGroup);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER forgetting part group " +
      partGroup->
        getPartGroupCombinedName () +
      " in the data");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStart (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePartGroupStart()");
  }

  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Creating part group with number '" <<
      fCurrentPartGroupNumber <<
      "' to contain part \"" << fCurrentPartID << "\"" <<
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
        fCurrentPartGroupDisplayText,
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

  // register it in the part groups data
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding part group " << fCurrentPartGroupNumber <<
      " to visitor's part groups data" <<
      ", partsCounter = " << fPartsCounter <<
      ", line " << inputLineNumber <<
      endl;
  }

  registerPartGroupInData (
    inputLineNumber,
    partGroupToBeStarted);

  // make it the new current part group
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing part group '" << fCurrentPartGroupNumber <<
      "' onto the part group stack" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsStack.push_front (
    partGroupToBeStarted);
  
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStart()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::stopPartGroup (
  int            inputLineNumber,
  S_msrPartGroup partGroup)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE stopPartGroup()");
  }

  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.front ();

  if (partGroup == currentPartGroup) {
    // partGroup is the current part group,
    // pop it currentPartGroup from the stack
    fPartGroupsStack.pop_front ();

    // fetch new current part group
    S_msrPartGroup
      newCurrentPartGroup =
        fetchCurrentPartGroupFromStack ();
  
    if (newCurrentPartGroup) {
      // partGroup is nested in the prior
      // next to top part group in the stack
  
      // set currentPartGroup's uplink to newCurrentPartGroup;
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Setting the uplink of part group " <<
          currentPartGroup->
            getPartGroupCombinedName () <<
          "' to " <<
          newCurrentPartGroup->
            getPartGroupCombinedName () << 
          ", line " << inputLineNumber <<
          endl;      
      }
    
      currentPartGroup->
        setPartGroupPartGroupUplink (
          newCurrentPartGroup);
    
      // appending currentPartGroup to newCurrentPartGroup
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Appending sub part group " <<
          currentPartGroup->
            getPartGroupCombinedName () <<
          "' to " <<
          newCurrentPartGroup->
            getPartGroupCombinedName () << 
          ", line " << inputLineNumber <<
          endl;      
      }
    
      newCurrentPartGroup->
        appendSubPartGroupToPartGroup (
          currentPartGroup);
    }
  
    else {
      // currentPartGroup is the top-most part group,
      // append it to thte MSR score
      if (gGeneralOptions->fTracePartGroups) {
        fLogOutputStream <<
          "Appending part group '" <<
          currentPartGroup->getPartGroupNumber () <<
          "' to MSR score" <<
          ", partsCounter = " << fPartsCounter <<
          ", line " << inputLineNumber <<
          endl;
      }
          
      fMsrScore->
        addPartGroupToScore (
          currentPartGroup);
    }

    // forget about the current part group,
    // since its number may be reused for another one
    forgetPartGroupInData (
      inputLineNumber,
      currentPartGroup);
  }

  else {
    // partGroup is not the current part group,
    // this 'stop' doesn't happen in strict reverse order

    // insert it in the part groups to be stopped list,
    // it will be handled at the next score-part
    // or at the end of the part-list
    insertPartGroupIntoToBeStoppedList (
      inputLineNumber,
      partGroup);
  }
        
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER stopPartGroup()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::insertPartGroupIntoToBeStoppedList (
  int            inputLineNumber,
  S_msrPartGroup partGroup)
{
  // this list is created in start position decreasing order,
  // so as to have the inner-most group appearing first
  
  if (fPendingPartGroupsToBeStoppedList.size () == 0) {
    fPendingPartGroupsToBeStoppedList.push_front (
      partGroup);
  }

  else {
    list<S_msrPartGroup>::iterator
      iBegin = fPendingPartGroupsToBeStoppedList.begin (),
      iEnd   = fPendingPartGroupsToBeStoppedList.end (),
      i      = iBegin;
  
    while (true) {
      if (i == iEnd) {
        stringstream s;
        s <<
          "part group " <<
          fCurrentPartGroupNumber <<
          " could not be inserted in part groups to be stopped list";
          
        msrInternalError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
        break;
      }
  
      if (
        fPartGroupsStartPositionsMap [partGroup]
          >=
        fPartGroupsStartPositionsMap [(*i)]) {
        // insert partGroup before (*i)
        fPendingPartGroupsToBeStoppedList.insert (
          i, partGroup);
        break;
      }
      
      i++;
    } // while
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePendingPartGroupsToBeStopped (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePendingPartGroupsToBeStopped()");
  }

  list<S_msrPartGroup>::iterator
    iBegin = fPendingPartGroupsToBeStoppedList.begin (),
    iEnd   = fPendingPartGroupsToBeStoppedList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      stringstream s;
      s <<
        "overlapping part groups 1";
/*
         <<
        fCurrentPartGroupNumber <<
        " could not be inserted in part groups to be stopped list";
  */
        
      msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    if (! fPartGroupsStack.size ()) {
      // the part groups stack is empty
      
      stringstream s;
      s <<
        "overlapping part groups 2";
/*
         <<
        fCurrentPartGroupNumber <<
        " could not be inserted in part groups to be stopped list";
  */
        
      msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
      break;
    }

    else {
      S_msrPartGroup
        partGroupToBeStopped =
          (*i),
        currentPartGroup =
          fPartGroupsStack.front ();
          
      // did the current part group start before partGroupToBeStopped?
      if (
        fPartGroupsStartPositionsMap [currentPartGroup]
          <=
        fPartGroupsStartPositionsMap [partGroupToBeStopped]) {
        // stop partGroupToBeStopped
        stopPartGroup (
          inputLineNumber,
          partGroupToBeStopped);

        // remove it from the part groups to be stopped list
        fPendingPartGroupsToBeStoppedList.erase (
          i);
        break;
      }
    }
    
    i++;
  } // while

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePendingPartGroupsToBeStopped()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::handlePartGroupStop (
  int inputLineNumber)
{
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "BEFORE handlePartGroupStop()");
  }

  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStopped =
      fetchPartGroupFromTheMap (
        fCurrentPartGroupNumber);

  if (! partGroupToBeStopped) {
    // no, but we should have fount it
    stringstream s;

    s <<
      "part group " << fCurrentPartGroupNumber <<
      " not found in the part groups map" <<
      ", line " << inputLineNumber <<
      endl;
      
    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  else {
    if (gGeneralOptions->fTracePartGroupsDetails) {
      fLogOutputStream <<
        "The part group to be stopped is " <<
        partGroupToBeStopped->getPartGroupCombinedName () <<
        ", line " <<inputLineNumber <<
        endl;
    }
  }

  // fetch current part group
  S_msrPartGroup
    currentPartGroup =
      fetchCurrentPartGroupFromStack ();

  if (! currentPartGroup) {
    // the stack is empty
    stringstream s;

    s <<
      "there is no current part group while handlling 'stop' on part group '" <<
      fCurrentPartGroupNumber << "'" <<
      " to";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  else {
    if (gGeneralOptions->fTracePartGroupsDetails) {
      fLogOutputStream <<
        "The current part group is " <<
        currentPartGroup->getPartGroupCombinedName () <<
        ", line " <<inputLineNumber <<
        endl;
    }
  }

  // is the part group to be stopped the current one,
  // i.e. the top of the stack?
  if (partGroupToBeStopped == currentPartGroup) {
    // yes, attach it to the current part group
    if (gGeneralOptions->fTracePartGroupsDetails) {
      fLogOutputStream <<
        "Stopping part group " <<
        partGroupToBeStopped->getPartGroupCombinedName () <<
        ", line " <<inputLineNumber <<
        endl;
    }

    stopPartGroup (
      inputLineNumber,
      partGroupToBeStopped);
  }

  else {
    // no, this 'stop' is not in the regular reverse order
    if (gGeneralOptions->fTracePartGroupsDetails) {
      fLogOutputStream <<
        "Part group " <<
        partGroupToBeStopped->getPartGroupCombinedName () <<
        " is not in the regular reverse order" <<
        ", line " <<inputLineNumber <<
        endl;
    }
    
    if (! fPartGroupsStack.size ()) {
      stringstream s;
  
      s <<
        "the part groups stack is empty while handlling 'stop' on part group " <<
        partGroupToBeStopped->
          getPartGroupCombinedName ();
        
      msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }
  
    else {
      // is the part group to be stopped below the current one in the stack?
      // this can happen because Finale for example can produce successive 'stop's
      // not in the order that nesting would imply
  /* JMI
      S_msrPartGroup
        cursor =
          fPartGroupsStack.begin () + 1;
    */      
    }
  }

  
/* JMI
  // remove the part group to be stopped from the start part groups set
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Removing part group " <<
      partGroupToBeStopped->getPartGroupNumber () <<
      " from visitor's started part groups set" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fStartedPartGroupsSet.erase (
    partGroupToBeStopped);

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER REMOVAL FROM LIST");
  }
*/

  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handlePartGroupStop()");
  }
}

//________________________________________________________________________
void mxmlTree2MsrSkeletonBuilder::createImplicitPartGroup (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  // sanity check
  msrAssert (
    fImplicitPartGroup == 0,
    "fImplicitPartGroup already exists");
  
  // create an implicit part group
  fCurrentPartGroupNumber = 0;
  
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Creating an implicit part group with number '" <<
      fCurrentPartGroupNumber <<
      "' to contain part \"" << fCurrentPartID << "\"" <<
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
      "",         // partGroupDisplayText
      "",         // partGroupAccidentalText
      "Impl.",    // partGroupAbbreviation
      msrPartGroup::k_NoPartGroupSymbol,
      0,          // partGroupSymbolDefaultX
      msrPartGroup::kPartGroupImplicitYes,
      msrPartGroup::kPartGroupBarlineYes,
      0,          // partGroupPartGroupUplink, 0 for top level part group 
      fMsrScore);

/* JMI
  // append it to the MSR score
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Appending implicit part group '" <<
      fImplicitPartGroup->getPartGroupNumber () <<
      "' to MSR score" <<
      ", partsCounter = " << fPartsCounter <<
      ", line " << inputLineNumber <<
      endl;
  }
      
  fMsrScore->
    addPartGroupToScore (
      fImplicitPartGroup);
   */
    
  // register implicit part groups in the part groups data
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding implicit part group '" <<
      fCurrentPartGroupNumber <<
      "' to the part groups data" <<
      ", partsCounter = " << fPartsCounter <<
      ", line " << inputLineNumber <<
      endl;
  }

  registerPartGroupInData (
    inputLineNumber,
    fImplicitPartGroup);

  // make it the current group
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing implicit part group '" <<
      fCurrentPartGroupNumber <<
      "' onto the stack" <<
      ", line " << inputLineNumber <<
      endl;
  }
  
  fPartGroupsStack.push_front (
    fImplicitPartGroup);

// JMI  fPartGroupsList.push_front (fImplicitPartGroup);

  if (gGeneralOptions->fTracePartGroups) {
    showPartGroupsData (
      inputLineNumber,
      "after creating fImplicitPartGroup");
  }
  
  fCurrentPartUsesImplicitPartGroup = true;
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

/*
  if (fImplicitPartGroup) {
    // force an implicit part group 'stop' on it
    handlePartGroupStop (
      inputLineNumber);

    // forget about the implicit part group // JMI ???
    fImplicitPartGroup = 0;
  }
  */
  
  if (fPartGroupsStack.size ()) {
    // force a 'stop' on the current part group
    handlePartGroupStop (
      inputLineNumber);
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
      fCurrentPartGroupDisplayText = "";
      fCurrentPartGroupAccidentalText = "";
      fCurrentPartGroupAbbreviation = "";
      fCurrentPartGroupSymbolKind = msrPartGroup::k_NoPartGroupSymbol;
      fCurrentPartGroupSymbolDefaultX = INT_MIN;
      fCurrentPartGroupBarlineKind = msrPartGroup::kPartGroupBarlineYes;
      break;
      
    case msrPartGroup::kStopPartGroupType:
      handlePartGroupStop (
        inputLineNumber);
      break;
      
    case msrPartGroup::k_NoPartGroupType:
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

  fOnGoingGroupNameDisplay = true;
}

void mxmlTree2MsrSkeletonBuilder::visitStart (S_display_text& elt)
{
  if (gMusicXMLOptions->fTraceMusicXMLTreeVisitors) {
    fLogOutputStream <<
      "--> Start visiting S_display_text" <<
      endl;
  }

  if (fOnGoingGroupNameDisplay)
    fCurrentPartGroupDisplayText = elt->getValue();
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
      ", partsCounter = " << fPartsCounter <<
      ", line " << inputLineNumber <<
      endl;
  }

  gIndenter++;
  
  // handle part group type
  switch (fCurrentPartGroupTypeKind) {
    
    case msrPartGroup::kStartPartGroupType:
      handlePartGroupStart (
        inputLineNumber);
      break;
      
    case msrPartGroup::kStopPartGroupType:
    case msrPartGroup::k_NoPartGroupType:
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

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
  fCurrentPartInstrumentAbbreviation = "";

  // are there pending part groups to be stopped?
  if (fPendingPartGroupsToBeStoppedList.size ()) {
    handlePendingPartGroupsToBeStopped (
      inputLineNumber);
  }
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
  
  string printObject = elt->getAttributeValue ("print-object"); // JMI
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

  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTraceParts)
   fLogOutputStream <<
    "--------------------------------------------" <<
    endl <<
    "Handling score part name \"" << fCurrentPartID << "\"" <<
      ", partsCounter = " << fPartsCounter <<
    ", line " << inputLineNumber <<
    endl;

  gIndenter++;

  // has a part group already been seen?
  if (! fPartGroupsMap.size ()) {
    // no, create an implicit one if needed
    createImplicitPartGroup (
      inputLineNumber);
  }

/* JMI
  // is this part already present in the current part group?
  S_msrPart
    part =
      currentPartGroup->
        fetchPartFromPartGroup (
          fCurrentPartID);
*/
   
  // fetch current part group
  S_msrPartGroup
    currentPartGroup =
      fetchCurrentPartGroupFromStack ();

  if (! currentPartGroup) {
    stringstream s;

    s <<
      "there is no current part group to append part \"" <<
      fCurrentPartID << "\"" <<
      " to";
      
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
  
  // create the part
  if (gGeneralOptions->fTraceParts)
   fLogOutputStream <<
    "--------------------------------------------" <<
    endl <<
    "Creating part \"" << fCurrentPartID << "\"" <<
    ", line " << inputLineNumber <<
    endl;

/*
  S_msrPart
    part =
      msrPart::create (
        inputLineNumber,
        fCurrentPartID,
        currentPartGroup);
*/

  // append it to the current started part groups
  S_msrPart
    part =
      currentPartGroup->
        appendPartToPartGroupByItsPartID (
          inputLineNumber,
          fCurrentPartID);

  // populate it
  part->
    setPartName (fCurrentPartName);
  part->
    setPartAbbreviation (fCurrentPartAbbreviation);
  part->
    setPartInstrumentName (fCurrentPartInstrumentName);
  part->
    setPartInstrumentAbbreviation (fCurrentPartInstrumentAbbreviation);

  // register part in the parts data
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Adding part " <<
      part->getPartCombinedName () <<
      " to the parts data" <<
      ", partsCounter = " << fPartsCounter <<
      ", line " << inputLineNumber <<
      endl;
  }

// JMI  fLogOutputStream << "fPartsCounter = " << fPartsCounter << endl;

  fPartsCounter++;
  fPartsVector.push_back (part);
    
  fPartsMap [partID] = part;
  
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "AFTER handling score part \"" + partID + "\"");
  }
  
  gIndenter--;
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

  if (! fCurrentPartID.size ()) {
    stringstream s;

    fCurrentNoIDPartNumber++;
    
    s <<
      "NO_ID_PART_" << fCurrentNoIDPartNumber;

    s >> fCurrentPartID;

    msrMusicXMLWarning (
      inputLineNumber,
      "part id is empty, creating one as '"
        +
      fCurrentPartID
        +
      "'");
  
    S_msrPartGroup partGroup;
  
    // has a part group already been seen?
    if (! fPartGroupsMap.size ()) {
      // no, create an implicit one if needed
      createImplicitPartGroup (
        inputLineNumber);
    }

/* JMI      
    if (gGeneralOptions->fTracePartGroupsDetails) {
      showPartGroupsData (
        inputLineNumber,
        "before creating the part groups");
    }

    // fetch current part group
    partGroup = fetchCurrentPartGroupFromStack ();        
  
    // create the part and add it to the current part group
    S_msrPart
      part =
        partGroup->
          appendPartToPartGroupByItsID (
            inputLineNumber,
            fCurrentPartID);

    // avoid empty part MSR name JMI ???
    part->
      setPartMsrName (fCurrentPartID);

    // register it in the parts map
    fPartsMap [fCurrentPartID] = part;
  */
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
      "\" is not registered in the part map";
    
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
        inputLineNumber, fCurrentStaffNumber);

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
          inputLineNumber, fCurrentNoteStaffNumber);
  
    if (gGeneralOptions->fTraceNotes && gGeneralOptions->fTraceVoices)
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
/*
    currentVoice = // ??? JMI
      createVoiceIfNotYetDone (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
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
    stringstream s;

    s <<
      "lyric number " << stanzaNumber <<
      " is empty";

    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      elt->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
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



/* JMI
  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStopped =
      fetchPartGroupFromTheMap (
        fCurrentPartGroupNumber);

  if (! partGroupToBeStopped) {
    stringstream s;

    s <<
      "part group '" << fCurrentPartGroupNumber << "'" <<
      " is not known in the map";

    msrMusicXMLError (
      gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  if (fPartGroupsStack.empty ()) {
    stringstream s;

    s <<
      "part group is empty, has '" << fCurrentPartGroupNumber << "'" <<
      " has already been stopped???";

    msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // sanity check
  msrAssert (
    fPartGroupsStack.size () != 0,
    "fPartGroupsStack is empty");
    
  S_msrPartGroup
    partGroupStackTop =
      fPartGroupsStack.top ();

  if (partGroupToBeStopped == partGroupStackTop) {
    // handle this 'stop'

    // pop the part group to be stopped off the part group stack
    fPartGroupsStack.pop ();

    // append the part group to be stopped to the new stack top if any
    if (fPartGroupsStack.size () != 0) {
      // the stopped part group is nested
      S_msrPartGroup
        containingPartGroup =
          fPartGroupsStack.top ();

      containingPartGroup->
        appendSubPartGroupToPartGroup (
          partGroupToBeStopped);
    }
    else {
      // the stoppped part group is the top-level one
    }

    if (gGeneralOptions->fTracePartGroupsDetails) {
      showPartGroupsData (
        inputLineNumber,
        "after popping part group '" + partGroupStackTop->getPartGroupCombinedName () + "'");
    }
  }

  else {
    stringstream s;

    showPartGroupsData (
      inputLineNumber,
      "before overlapping message");

    s <<
      "part groups '" << fCurrentPartGroupNumber << "'" <<
      " and " <<
      "part group '" << partGroupStackTop->getPartGroupNumber () << "'" <<
      " are overlapping: this is not supported";

 // JMI   msrMusicXMLError (
    msrMusicXMLWarning (
        gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }
*/





/* JMI
  // is the part group to be stopped known?
  S_msrPartGroup
    partGroupToBeStarted =
      fetchPartGroupFromTheMap (
        fCurrentPartGroupNumber);

  if (partGroupToBeStarted) {
    stringstream s;

    s <<
      "part group '" << fCurrentPartGroupNumber << "'" <<
      " is already known";
      
    msrMusicXMLError (
        gGeneralOptions->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // fetch containing part group if any
  S_msrPartGroup
    containingPartGroup =
      ! fPartGroupsStack.empty ()
        ? fPartGroupsStack.top ()
        : 0;
      
  // create the part group
  partGroupToBeStarted =
    msrPartGroup::create (
      inputLineNumber,
      fCurrentPartGroupNumber,
      fCurrentPartGroupName,
      fCurrentPartGroupDisplayText,
      fCurrentPartGroupAccidentalText,
      fCurrentPartGroupAbbreviation,
      fCurrentPartGroupSymbolKind,
      fCurrentPartGroupSymbolDefaultX,
      fCurrentPartGroupBarlineKind,
      containingPartGroup,
      fMsrScore);

  // register partGroupToBeStarted in the map
  fPartGroupsMap [fCurrentPartGroupNumber] =
    partGroupToBeStarted;

  // push it onto the part group stack of this visitor
  if (gGeneralOptions->fTracePartGroups) {
    fLogOutputStream <<
      "Pushing part group '" << fCurrentPartGroupNumber <<
      "' onto the part group stack" <<
      ", line " << inputLineNumber <<
      endl;
  }

  fPartGroupsStack.push (partGroupToBeStarted);
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsStack (
      inputLineNumber,
      "after pushing partGroupToBeStarted");
  }
  */


/* JMI
  // add it to the part group list of this visitor
  if (gGeneralOptions->fTracePartGroups)
    fLogOutputStream <<
      "Adding part group " << fCurrentPartGroupNumber <<
      " to visitor's part groups list" <<
      endl;

  if (! fPartGroupsList.size ())
  
    // insert first part group ahead of the list
    fPartGroupsList.push_front (partGroupToBeStarted);
    
  else {
    
    // place in the part groups list so as
    // to have them ordered by increasing order
    // (all of them are negative)
    list<S_msrPartGroup>::iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;

    while (true) {
      if (i == iEnd) {
        fPartGroupsList.push_back (partGroupToBeStarted);
        break;
      }

      // CAUTION: insert() inserts before the position
      // indicated by its iterator argument
      if (
          fCurrentPartGroupSymbolDefaultX
            <
          (*i)->getPartGroupSymbolDefaultX ()) {
        fPartGroupsList.insert (i, partGroupToBeStarted);
        break;
      }
      
      i++;
    } // while
  }
  */
  

/* JMI
  list<S_msrPartGroup>::iterator
    iBegin = fPartGroupsList.begin (),
    iEnd   = fPartGroupsList.end (),
    i      = iBegin;

  while (true) {
    if (i == iEnd) {
      stringstream s;
      s <<
        "part group " <<
        fCurrentPartGroupNumber <<
        " not found in part groups list";
        
      msrInternalError (
        inputLineNumber,
        s.str ());
      break;
    }

    if ((*i) == partGroupToBeStopped) {
      fPartGroupsList.erase (i);
      break;
    }
    
    i++;
  } // while
*/


/*
  if (partGroupToBeStopped != fImplicitPartGroup) {
    // take care of the part group to be stopped
    // in the part groups list
    
    if (! fPartGroupsList.size ()) {
      
      // we're just removed the only part group in the list:
      // append it to the MSR score
      if (gGeneralOptions->fTracePartGroups)
        fLogOutputStream <<
          "Appending part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " to MSR score" <<
          endl;
          
      fMsrScore->
        addPartGroupToScore (partGroupToBeStopped);
    }
  
    else {
  
      // the front element in the part group list is
      // the new current part group
      S_msrPartGroup
        newCurrentPartGroup = fPartGroupsList.front ();
  
      if (
        partGroupToBeStopped->getPartGroupNumber ()
          ==
        newCurrentPartGroup->getPartGroupNumber () ) {
        stringstream s;
        
        s <<
          "cannot append part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " as sub part group of itself";
          
        msrInternalError (
          inputLineNumber,
          s.str ());
      }
      
      // insert current group into future current group
      if (gGeneralOptions->fTracePartGroups)
        fLogOutputStream <<
          "Preending (sub-)part group " <<
          partGroupToBeStopped->getPartGroupNumber () <<
          " at the beginning of part group " <<
          newCurrentPartGroup->getPartGroupNumber () <<
          endl;
  
      newCurrentPartGroup->
        prependSubPartGroupToPartGroup (
          partGroupToBeStopped);
    }
  }
  *
  * */


/* JMI
  if (gGeneralOptions->fTracePartGroupsDetails) {
    showPartGroupsData (
      inputLineNumber,
      "before fetching part groups stack top");
  }

  // sanity check
  msrAssert (
    fPartGroupsStack.size () != 0,
    "fPartGroupsStack is empty");
    
  // fetch current part group
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsStack.top ();
        
  // sanity check
  msrAssert (
    fPartGroupsList.size () != 0,
    "fPartGroupsList is empty");
    
  // fetch current part group
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsList.front ();
   */


/*
  S_msrPartGroup
    partGroupToBeStarted =
      fetchPartGroupFromTheMap (
        fCurrentPartGroupNumber);

  if (partGroupToBeStarted) {
    stringstream s;

    s <<
      "part group '" << fCurrentPartGroupNumber << "'" <<
      " is already known";
      
    msrMusicXMLError (
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  // the current part group is either null
  // or the head of the part group list
  S_msrPartGroup
    currentPartGroup =
      fPartGroupsList.size ()
        ? fPartGroupsList.front ()
        : 0;
        
  if (! partGroupToBeStarted) {
    // no, create it
    partGroupToBeStarted =
      msrPartGroup::create (
        inputLineNumber,
        fCurrentPartGroupNumber,
        fCurrentPartGroupName,
        fCurrentPartGroupDisplayText,
        fCurrentPartGroupAccidentalText,
        fCurrentPartGroupAbbreviation,
        fCurrentPartGroupSymbolKind,
        fCurrentPartGroupSymbolDefaultX,
        fCurrentPartGroupBarlineKind,
        currentPartGroup,
        fMsrScore);

    // register it in the part groups data
    if (gGeneralOptions->fTracePartGroups) {
      fLogOutputStream <<
        "Adding part group " << fCurrentPartGroupNumber <<
        " to visitor's part groups data" <<
      ", partsCounter = " << fPartsCounter <<
        ", line " << inputLineNumber <<
        endl;
    }

    fPartGroupsVector [++fPartGroupsCounter] =
      partGroupToBeStarted;
*/

    /*
    fPartGroupsMap [fCurrentPartGroupNumber] =
      partGroupToBeStarted;

    // insert it into the started part group set of this visitor
    if (gGeneralOptions->fTracePartGroups) {
      fLogOutputStream <<
        "Inserting part group " << fCurrentPartGroupNumber <<
        " into visitor's part started part groups set" <<
        ", line " << inputLineNumber <<
        endl;
    }

    fStartedPartGroupsSet.insert (
      partGroupToBeStarted);
*/

