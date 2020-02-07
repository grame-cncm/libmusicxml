/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___optionsElements___
#define ___optionsElements___

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

enum oahElementVisibilityKind {
  kElementVisibilityAlways,
  kElementVisibilityHiddenByDefault };

string optionVisibilityKindAsString (
  oahElementVisibilityKind optionVisibilityKind);

//______________________________________________________________________________
class oahElement : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<oahElement> create (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    oahElement (
      string                   shortName,
      string                   longName,
      string                   description,
      oahElementVisibilityKind optionVisibilityKind);

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

    oahElementVisibilityKind
                          getElementVisibilityKind () const
                              { return fElementVisibilityKind; }

    void                  setIsHidden ()
                              { fIsHidden = true; }

    bool                  getIsHidden () const
                              { return fIsHidden; }

    void                  setMultipleOccurrencesAllowed ()
                              { fMultipleOccurrencesAllowed = true; }

    bool                  getMultipleOccurrencesAllowed () const
                              { return fMultipleOccurrencesAllowed; }

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

    virtual void          printOptionHeader (ostream& os) const; // virtual ??? JMI

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

    oahElementVisibilityKind
                          fElementVisibilityKind;

    bool                  fIsHidden;

    bool                  fMultipleOccurrencesAllowed;
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
#ifdef TRACE_OAH
  //    if (gOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> browse()" <<
          endl;
  //    }
#endif

      enter (t);

      t.browseData (fVisitor);

      leave (t);
    }

  protected:

    basevisitor*  fVisitor;

    virtual void enter (T& t) {
#ifdef TRACE_OAH
  //    if (gOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> enter()" <<
          endl;
  //    }
#endif

      t.acceptIn  (fVisitor);
    }
    virtual void leave (T& t) {
#ifdef TRACE_OAH
  //    if (gOahOah->fTraceOahVisitors) {
        cout <<
          endl <<
          ".\\\" --> leave()" <<
          endl;
  //    }
#endif

      t.acceptOut (fVisitor);
    }
};


}


#endif
