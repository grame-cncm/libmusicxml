/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrScores.h"

#include <iomanip>      // for 'setw()'

#include "msr.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "musicXMLOah.h"
#include "msrOah.h"


using namespace std;


namespace MusicXML2
{

//______________________________________________________________________________
S_msrScore msrScore::create (
  int inputLineNumber)
{
  msrScore* o =
    new msrScore (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // create the scaling
  fScaling =
    msrScaling::create (
      inputLineNumber);

  // create the page layout
  fPageLayout =
    msrPageLayout::create (
      inputLineNumber);

  // number of measures
  fScoreNumberOfMeasures = -1;

  // part group names max length
  fScorePartGroupNamesMaxLength = -1;

  // part names max length
  fScorePartNamesMaxLength = -1;

  // set instrument names max lengthes
  fScoreInstrumentNamesMaxLength      = -1;
  fScoreInstrumentAbbreviationsMaxLength = -1;

  // measures repeats replicas should be browsed by default
  fInhibitMeasuresRepeatReplicasBrowsing = false;

  // rest measures should be browsed by default
  fInhibitRestMeasuresBrowsing = false;
}

msrScore::~msrScore ()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceScore) {
    gLogOstream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber);

  // number of measures
  newbornClone->fScoreNumberOfMeasures =
    fScoreNumberOfMeasures;

  // part group names max length

  newbornClone->fScorePartGroupNamesMaxLength =
    fScorePartGroupNamesMaxLength;

  // part names max length

  newbornClone->fScorePartNamesMaxLength =
    fScorePartNamesMaxLength;

  // instrument names max lengthes

  newbornClone->fScoreInstrumentNamesMaxLength =
    fScoreInstrumentNamesMaxLength;

  newbornClone->fScoreInstrumentAbbreviationsMaxLength =
    fScoreInstrumentAbbreviationsMaxLength;

  // inhibiting browsing

  newbornClone->fInhibitMeasuresRepeatReplicasBrowsing =
    fInhibitMeasuresRepeatReplicasBrowsing;

  newbornClone->fInhibitRestMeasuresBrowsing =
    fInhibitRestMeasuresBrowsing;

  return newbornClone;
}

void msrScore::setScoreMasterVoice (
  S_msrVoice masterVoice)
{ fScoreMasterVoice = masterVoice; }

S_msrVoice msrScore::getScoreMasterVoice () const
{ return fScoreMasterVoice; }

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {
    stringstream s;

    s <<
      "part group '" <<
      partGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrInternalError (
      gOahOah->fInputSourceName,
      partGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fScorePartGroupsSet.insert (partGroup);
  fPartGroupsList.push_back (partGroup);
}

void msrScore::appendCreditToScore (S_msrCredit credit)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceCredits) {
    gLogOstream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
#endif

  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;

#ifdef TRACE_OAH
  if (gTraceOah->fTracePartGroupsDetails) {
    gLogOstream <<
      "fetchPartFromScoreByItsPartID(" << partID << "), fPartGroupsList contains:" <<
      endl;

    gIndenter++;

    for (
      list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
        i != fPartGroupsList.end ();
        i++
      ) {
      gLogOstream <<
        (*i)->getPartGroupCombinedName () <<
        ", " <<
        (*i)->getPartGroupName () <<
        endl;
    } // for

    gIndenter--;

    gLogOstream <<
      "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
      endl <<
      endl;
  }
#endif

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++
  ) {
    S_msrPart
      part =
        (*i)->
          fetchPartFromPartGroupByItsPartID (
            inputLineNumber,
            partID);

    if (part) {
      result = part;
      break;
    }
  } // for

  return result;
}

void msrScore::fetchIdentificationFromCreditsIfAny (
  int inputLineNumber)
{
  if (
    ! fIdentification->getWorkTitle ()
      &&
    gMusicXMLOah->fUseFilenameAsWorkTitle
  ) {
    string
      inputSourceName =
        gOahOah->fInputSourceName;

    if (inputSourceName == "-") {
      inputSourceName = "Standard_input";
    }
  }

  /*
  <credit page="1">
    <credit-words default-x="548" default-y="1382" font-family="FreeSerif" font-size="26" font-weight="bold" justify="center" valign="top" xml:space="preserve">"R E Q U I E M"    from    D E A T H N O T E</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="548" default-y="1331" font-family="FreeSerif" font-size="16" font-style="italic" justify="center" valign="top">Theme from L's death. For SATB choir.</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="1046" default-y="1253" font-family="FreeSerif" font-size="12" justify="right" valign="bottom">Yoshihisa Hirano &amp; Hideki Taniuchi</credit-words>
  </credit>
  <credit page="1">
    <credit-words default-x="51" default-y="1253" font-family="FreeSerif" font-size="12" valign="bottom" xml:lang="fr">arrangement and lyrics by Andrés Rojas</credit-words>
  </credit>

  */

  // credits on top of page one can be used as identification
  if (fCreditsList.size () >= 1) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;

    int topCreditsCounter    = 0;
    int bottomCreditsCounter = 0;

    for ( ; ; ) {
      S_msrCredit credit = (*i);

      if (credit->getCreditPageNumber () == 1) {
        const vector<S_msrCreditWords>&
          creditWordsVector =
            credit->
              getCreditWordsList ();

        if (creditWordsVector.size () >= 1) {
          S_msrCreditWords
            creditWords =
              creditWordsVector.front ();

          string
            creditWordsContents =
              creditWords->
                getCreditWordsContents ();

          switch (creditWords->getCreditWordsVerticalAlignmentKind ()) {
            case kVerticalAlignmentNone:
              break;

            case kVerticalAlignmentTop:
              topCreditsCounter++;

              switch (topCreditsCounter) {
                case 1:
#ifdef TRACE_OAH
                  if (gTraceOah->fTraceCredits) {
                    gLogOstream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as score title" <<
                      endl;
              }
#endif

                  fIdentification->
                    setWorkTitle (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef TRACE_OAH
                  if (gTraceOah->fTraceCredits) {
                    gLogOstream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as movement title" <<
                      endl;
              }
#endif

                  fIdentification->
                    setMovementTitle (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                default:
                  ;
              } // switch
              break;

            case kVerticalAlignmentMiddle:
              break;

            case kVerticalAlignmentBottom:
              bottomCreditsCounter++;

              switch (bottomCreditsCounter) {
                case 1:
#ifdef TRACE_OAH
                  if (gTraceOah->fTraceCredits) {
                    gLogOstream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as composer" <<
                      endl;
              }
#endif

                  fIdentification->
                    addComposer (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef TRACE_OAH
                  if (gTraceOah->fTraceCredits) {
                    gLogOstream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as poet" <<
                      endl;
              }
#endif

                  fIdentification->
                    addPoet (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                default:
                  ;
              } // switch
              break;
          } // switch
        }
      }

      if (++i == iEnd) break;
//      s << ", ";
    } // for
  }
}

/* JMI
void msrScore::setHeaderFromOptionsIfAny (
  int inputLineNumber)
{
  // should we use lyricists as poets? JMI
  if (gMusicXMLOah->fUseLyricistsAsPoets) {
    S_msrVarValsListAssoc
      lyricists =
        fIdentification->getLyricists ();

    const list<string>&
      lyricistsValuesList =
        lyricists->getVariableValuesList ();

    if (lyricistsValuesList.size ()) {
      S_msrVarValsListAssoc
        poets =
          fIdentification->getPoets ();

      list<string>::const_iterator
        iBegin = lyricistsValuesList.begin (),
        iEnd   = lyricistsValuesList.end (),
        i      = iBegin;

      for ( ; ; ) {
        poets->addAssocVariableValue (*i);
        if (++i == iEnd) break;
      } // for
    }
  }
}
*/

void msrScore::collectScorePartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  S_msrPart result;

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++
  ) {
    S_msrPartGroup
      partGroup = (*i);
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
  } // for
}

/*
S_msrPartGroup msrScore::fetchScorePartGroup (
  int partGroupNumber)
{
  S_msrPartGroup result;

  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrScore::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrScore::browseData ()" <<
      endl;
  }

  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fScaling) {
    // browse scaling
    msrBrowser<msrScaling> browser (v);
    browser.browse (*fScaling);
  }

  if (fPageLayout) {
    // browse page layout
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

  for (
    list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    i++
  ) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++
  ) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% <== msrScore::browseData ()" <<
      endl;
  }
}

void msrScore::print (ostream& os) const
{
  os <<
    "MSR Score" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();

  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scorePartGroupNamesMaxLength" <<  " : " <<
    fScorePartGroupNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scorePartNamesMaxLength" <<  " : " <<
    fScorePartNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout;
  }

  // print the credits if any
  int creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << endl;
    gIndenter++;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os << endl;

  // print the part groups if any
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }

  gIndenter--;
}

void msrScore::printSummary (ostream& os)
{
  os <<
    "MSR component" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the implicit part group contents
  // it is the only element in fPartGroupsList JMI single variable
  if (partGroupsListSize) {
    os <<
      "Parts and part groups structure:" <<
      endl;

    gIndenter++;

    fPartGroupsList.front () ->
      printPartGroupParts (
        fInputLineNumber,
        os);

    gIndenter--;

    os << endl;
  }

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling;
  }

  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;
  }

  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print (os);
  return os;
}


}
