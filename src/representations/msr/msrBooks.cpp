/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // for 'setw()'

#include "msr.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "mxmlTree2msrOah.h"
#include "msrOah.h"


using namespace std;


namespace MusicXML2
{

//______________________________________________________________________________
S_msrBook msrBook::create (
  int inputLineNumber)
{
  msrBook* o =
    new msrBook (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

msrBook::msrBook (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // number of measures
  fBookNumberOfMeasures = 0;

  // part group names max length
  fBookElementNamesMaxLength = 0;

  // part names max length
  fBookElementNamesMaxLength = 0;

  // set instrument names max lengthes
  fBookInstrumentNamesMaxLength         = 0;
  fBookInstrumentAbbreviationsMaxLength = 0;

  // grace notes groups before should be browsed by default
  fInhibitGraceNotesGroupsBeforeBrowsing = false;
  fInhibitGraceNotesGroupsAfterBrowsing  = false;

  // measures repeats replicas should be browsed by default
  fInhibitMeasuresRepeatReplicasBrowsing = false;

  // rest measures should be browsed by default
  fInhibitRestMeasuresBrowsing = false;
}

msrBook::~msrBook ()
{}

S_msrBook msrBook::createBookNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBooks ()) {
    gLogStream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

  S_msrBook
    newbornClone =
      msrBook::create (
        fInputLineNumber);

  // number of measures
  newbornClone->fBookNumberOfMeasures =
    fBookNumberOfMeasures;

  // part group names max length

  newbornClone->fBookElementNamesMaxLength =
    fBookElementNamesMaxLength;

  // part names max length

  newbornClone->fBookElementNamesMaxLength =
    fBookElementNamesMaxLength;

  // instrument names max lengthes

  newbornClone->fBookInstrumentNamesMaxLength =
    fBookInstrumentNamesMaxLength;

  newbornClone->fBookInstrumentAbbreviationsMaxLength =
    fBookInstrumentAbbreviationsMaxLength;

  // inhibiting browsing

  newbornClone->fInhibitGraceNotesGroupsBeforeBrowsing =
    fInhibitGraceNotesGroupsBeforeBrowsing;
  newbornClone->fInhibitGraceNotesGroupsAfterBrowsing =
    fInhibitGraceNotesGroupsAfterBrowsing;

  newbornClone->fInhibitMeasuresRepeatReplicasBrowsing =
    fInhibitMeasuresRepeatReplicasBrowsing;

  newbornClone->fInhibitRestMeasuresBrowsing =
    fInhibitRestMeasuresBrowsing;

  return newbornClone;
}

void msrBook::setBookMasterVoice (
  S_msrVoice masterVoice)
{ fBookMasterVoice = masterVoice; }

S_msrVoice msrBook::getBookMasterVoice () const
{ return fBookMasterVoice; }

void msrBook::registerVoiceInBookAllVoicesList (
  S_msrVoice voice)
{
  // register voice in this staff
  fBookAllVoicesList.push_back (voice);
}

void msrBook::addBookElementToBook (
  S_msrBookElement bookElement)
{
  if (fBookElementsSet.count (bookElement)) {
    stringstream s;

    s <<
      "part group '" <<
      bookElement <<
      "' already exists in this score";

    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      bookElement->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fBookElementsSet.insert (bookElement);
  fBookElementsList.push_back (bookElement);
}

void msrBook::appendCreditToBook (S_msrCredit credit)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceCredits ()) {
    gLogStream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
#endif

  fCreditsList.push_back (credit);
}

void msrBook::fetchIdentificationFromCreditsIfAny ( // THROW AWAY JMI ???
  int inputLineNumber)
{
/* JMI
  if (
    fIdentification->getWorkTitle ().size ()
      &&
    gGlobalMxmlTree2msrOahGroup->getUseFilenameAsWorkTitle ()
  ) {
    string
      inputSourceName =
        gGlobalOahOahGroup->getInputSourceName ();

    if (inputSourceName == "-") {
      inputSourceName = "Standard_input";
    }
  }
*/

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
              ++topCreditsCounter;

              switch (topCreditsCounter) {
                case 1:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTraceOahGroup->getTraceCredits ()) {
                    gLogStream <<
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
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTraceOahGroup->getTraceCredits ()) {
                    gLogStream <<
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
              ++bottomCreditsCounter;

              switch (bottomCreditsCounter) {
                case 1:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTraceOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as composer" <<
                      endl;
              }
#endif

                  fIdentification->
                    appendComposer (
                      inputLineNumber,
                      creditWordsContents);
                  break;

                case 2:
#ifdef TRACING_IS_ENABLED
                  if (gGlobalTraceOahGroup->getTraceCredits ()) {
                    gLogStream <<
                      "Using credit words '" <<
                      creditWordsContents <<
                      "' as poet" <<
                      endl;
              }
#endif

                  fIdentification->
                    appendPoet (
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

/*
S_msrBookElement msrBook::fetchBookElement (
  int partGroupNumber)
{
  S_msrBookElement result;

  if (fBookElementsMap.count (partGroupNumber)) {
    result = fBookElementsMap [partGroupNumber];
  }

  return result;
}
*/

void msrBook::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBook::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBook::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBook>*
    p =
      dynamic_cast<visitor<S_msrBook>*> (v)) {
        S_msrBook elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrBook::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBook::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrBook::browseData ()" <<
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

  if (fSystemLayout) {
    // browse system layout
    msrBrowser<msrSystemLayout> browser (v);
    browser.browse (*fSystemLayout);
  }

  if (fStaffLayout) {
    // browse staff layout
    msrBrowser<msrStaffLayout> browser (v);
    browser.browse (*fStaffLayout);
  }

  if (fAppearance) {
    // browse appearance
    msrBrowser<msrAppearance> browser (v);
    browser.browse (*fAppearance);
  }

  for (
    list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    ++i
  ) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrBookElement>::const_iterator i = fBookElementsList.begin ();
    i != fBookElementsList.end ();
    ++i
  ) {
    // browse the part group
    msrBrowser<msrBookElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrBook::browseData ()" <<
      endl;
  }
}

void msrBook::print (ostream& os) const
{
  os <<
    "MSR book" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 38;

  unsigned int bookElementsListSize =
    fBookElementsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "scoreNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scorePartGroupNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scorePartNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsBeforeBrowsing) <<
    endl<<
    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsAfterBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsAfterBrowsing) <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMeasuresRepeatReplicasBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMeasuresRepeatReplicasBrowsing) <<
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
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
  }

  os << endl;

  // print the credits if any
  unsigned int creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os << endl;

  // print all the voices if any
  unsigned int scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }

  // print the part groups if any
  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
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

  --gIndenter;
}

void msrBook::printShort (ostream& os) const
{
  os <<
    "MSR book, short version" <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 38;

  unsigned int bookElementsListSize =
    fBookElementsList.size ();

  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "scoreNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl;

/*
  os << left <<
    setw (fieldWidth) <<
    "scorePartGroupNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scorePartNamesMaxLength" <<  " : " <<
    fBookElementNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsBeforeBrowsing) <<
    endl<<
    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsAfterBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsAfterBrowsing) <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMeasuresRepeatReplicasBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMeasuresRepeatReplicasBrowsing) <<
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
*/

  os << endl;

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
  }

/*
  // print the credits if any
  unsigned int creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrCredit credit = (*i);

      os << credit;
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os << endl;

  // print all the voices if any
  unsigned int scoreAllVoicesListSize = fBookAllVoicesList.size ();

  os <<
    setw (fieldWidth) <<
    "BookAllVoicesList";
  if (scoreAllVoicesListSize) {
    os << endl;
    ++gIndenter;

    list<S_msrVoice>::const_iterator
      iBegin = fBookAllVoicesList.begin (),
      iEnd   = fBookAllVoicesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoice voice = (*i);

      os << voice->getVoiceName () << endl;
      if (++i == iEnd) break;
      // os << endl;
    } // for
    os << endl;

    --gIndenter;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
*/

  // print the part groups if any
  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      // no endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }

  --gIndenter;
}

void msrBook::printSummary (ostream& os) const
{
  os <<
    "MSR component" << // JMI summary ???
    endl << endl;

  ++gIndenter;

  const unsigned int fieldWidth = 38;

  unsigned int bookElementsListSize =
    fBookElementsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    "bookElementsListSize" << " : " <<
    bookElementsListSize <<
    endl <<

    setw (fieldWidth) <<
    "scoreNumberOfMeasures" << " : " <<
    fBookNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fBookInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fBookInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsBeforeBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsBeforeBrowsing) <<
    endl<<
    setw (fieldWidth) <<
    "inhibitGraceNotesGroupsAfterBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitGraceNotesGroupsAfterBrowsing) <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMeasuresRepeatReplicasBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMeasuresRepeatReplicasBrowsing) <<
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
      fIdentification <<
      endl;
  }

  // print the scaling if any
  if (fScaling) {
    os <<
      fScaling <<
      endl;
  }

  // print the page layout if any
  if (fPageLayout) {
    os <<
      fPageLayout <<
      endl;
  }

  // print the system layout if any
  if (fSystemLayout) {
    os <<
      fSystemLayout <<
      endl;
  }

  // print the staff layout if any
  if (fStaffLayout) {
    os <<
      fStaffLayout <<
      endl;
  }

  // print the appearance if any
  if (fAppearance) {
    os <<
      fAppearance <<
      endl;
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

  if (bookElementsListSize) {
    list<S_msrBookElement>::const_iterator
      iBegin = fBookElementsList.begin (),
      iEnd   = fBookElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrBook& elt)
{
  elt->print (os);
  return os;
}


}
