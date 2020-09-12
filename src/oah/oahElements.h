/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___oahElements___
#define ___oahElements___

using namespace std;

namespace MusicXML2
{

// layout settings
//______________________________________________________________________________
const int K_OAH_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_OAH_FIELD_WIDTH = 40;

// PRE-declarations for class dependencies
//______________________________________________________________________________
class oahElement;
typedef SMARTP<oahElement> S_oahElement;

class oahValuedAtom;
typedef SMARTP<oahValuedAtom> S_oahValuedAtom;

class EXP oahHandler;
typedef SMARTP<oahHandler> S_oahHandler;

// data types
// ------------------------------------------------------

enum oahElementValueExpectedKind {
  kElementValueExpectedYes,
  kElementValueExpectedNo,
  kElementValueExpectedOptional };

string elementValueExpectedKindAsString (
  oahElementValueExpectedKind elementValueExpectedKind);

enum oahElementVisibilityKind {
  kElementVisibilityWhole,
  kElementVisibilityHeaderOnly,
  kElementVisibilityNone };

string elementVisibilityKindAsString (
  oahElementVisibilityKind elementVisibilityKind);

enum oahElementIsPureHelpKind {
    kElementIsPureHelpYes,
    kElementIsPureHelpNo };

static string elementIsPureHelpKindAsString (
  oahElementIsPureHelpKind elementIsPureHelpKind);

//______________________________________________________________________________
class oahElement : public smartable
{
  public:

/* JMI
    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
      string                      shortName,
      string                      longName,
      string                      description,
      oahElementValueExpectedKind elementValueExpectedKind,
      oahElementVisibilityKind    elementVisibilityKind);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string                      shortName,
      string                      longName,
      string                      description,
      oahElementValueExpectedKind elementValueExpectedKind,
      oahElementVisibilityKind    elementVisibilityKind);

    virtual ~oahElement ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    void                  setHandlerUpLink (
                            S_oahHandler handlerUpLink)
                              { fHandlerUpLink = handlerUpLink; }

    S_oahHandler          getHandlerUpLink () const
                              { return fHandlerUpLink; }

    string                getShortName () const
                              { return fShortName; }

    string                getLongName () const
                              { return fLongName; }

    string                getDescription () const
                              { return fDescription; }

    void                  setElementValueExpectedKind (
                            oahElementValueExpectedKind elementValueExpectedKind)
                              { fElementValueExpectedKind = elementValueExpectedKind; }

    oahElementValueExpectedKind
                          getElementValueExpectedKind () const
                              { return fElementValueExpectedKind; }

    void                  setElementVisibilityKind (
                            oahElementVisibilityKind elementVisibilityKind)
                              { fElementVisibilityKind = elementVisibilityKind; }

    oahElementVisibilityKind
                          getElementVisibilityKind () const
                              { return fElementVisibilityKind; }

    void                  setIsHidden ()
                              { /* JMI fIsHidden = true; */ }

    bool                  getIsHidden () const
                              { return fIsHidden; }

    void                  setMultipleOccurrencesAllowed ()
                              { fMultipleOccurrencesAllowed = true; }

    bool                  getMultipleOccurrencesAllowed () const
                              { return fMultipleOccurrencesAllowed; }

    void                  setOahElementIsPureHelpKind (
                            oahElementIsPureHelpKind elementIsPureHelpKind)
                              {
                                fOahElementIsPureHelpKind = elementIsPureHelpKind;
                              }

    bool                  getOahElementIsPureHelpKind () const
                              { return fOahElementIsPureHelpKind; }

  public:

    // services
    // ------------------------------------------------------

    string                fetchNames () const;
    string                fetchNamesInColumns (
                            int subGroupsShortNameFieldWidth) const;

    string                fetchNamesBetweenParentheses () const;
    string                fetchNamesInColumnsBetweenParentheses (
                            int subGroupsShortNameFieldWidth) const;

    virtual int           fetchVariableNameLength () const
                              { return 0; }

    S_oahElement          fetchOptionByName (
                            string name);

    virtual S_oahValuedAtom
                          handleOptionUnderName (
                            string   optionName,
                            ostream& os);

    S_oahElement          aPropos (string theString);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asShortNamedOptionString () const;
    virtual string        asActualLongNamedOptionString () const;

    string                asLongNamedOptionString () const
                              {
                                if (fLongName.size ()) {
                                  return asActualLongNamedOptionString ();
                                }
                                else {
                                  return asShortNamedOptionString ();
                                }
                              }

    string                asString () const;

    virtual void          printOptionHeader (ostream& os) const;

    virtual void          printOptionEssentials (
                            ostream& os,
                            int      fieldWidth) const;

    virtual void          print (ostream& os) const;

    virtual void          printHelp (ostream& os);

  protected:

    // fields
    // ------------------------------------------------------

    // uplink
    S_oahHandler          fHandlerUpLink;

    string                fShortName;
    string                fLongName;
    string                fDescription;

    oahElementValueExpectedKind
                          fElementValueExpectedKind;

    oahElementVisibilityKind
                          fElementVisibilityKind;

    bool                  fIsHidden;

    bool                  fMultipleOccurrencesAllowed;

    oahElementIsPureHelpKind
                          fOahElementIsPureHelpKind;

};
typedef SMARTP<oahElement> S_oahElement;
EXP ostream& operator<< (ostream& os, const S_oahElement& elt);

/*
Because the set needs a comparison functor to work with. If you don't specify one, it will make a default-constructed one. In this case, since you're using a function-pointer type, the default-constructed one will be a null pointer, which can't be called; so instead, you have to provide the correct function pointer at run time.

A better approach might be to use a function class type (a.k.a. functor type); then the function call can be resolved at compile time, and a default-constructed object will do the right thing.
*/
struct compareOahElements {
  bool operator() (
    const S_oahElement firstElement,
    const S_oahElement secondElement) const;
};

/* JMI
//______________________________________________________________________________
template <typename T> class oahBrowser : public browser<T>
{
  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:

    oahBrowser (basevisitor* v) : fVisitor (v)
    {}

    virtual ~oahBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }

    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);

                              leave (t);
                            }
};
*/

template <typename T> class oahBrowser : public browser <T>
{
  public:

    oahBrowser (basevisitor* v) : fVisitor (v) {}

    virtual ~oahBrowser () {}

  public:

    virtual void set (basevisitor* v) { fVisitor = v; }

    virtual void browse (T& t) {
/* JMI
#ifdef TRACE_OAH
      if (gGlobalOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> browse()" <<
          endl;
      }
#endif
*/

      enter (t);

      t.browseData (fVisitor);

      leave (t);
    }

  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) {
/* JMI
#ifdef TRACE_OAH
      if (gGlobalOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> enter()" <<
          endl;
      }
#endif
*/

      t.acceptIn  (fVisitor);
    }
    virtual void leave (T& t) {
/* JMI
#ifdef TRACE_OAH
      if (gGlobalOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> leave()" <<
          endl;
      }
#endif
*/

      t.acceptOut (fVisitor);
    }
};


}


#endif
