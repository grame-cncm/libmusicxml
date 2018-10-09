/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrContexts___
#define ___lpsrContexts___

#include <iostream>
#include <string>
#include <list>

#include "lpsrElements.h"

#include "msr.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrContext : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrContextTypeKind {
      kChordNames, kFiguredBass };
    
    static string contextTypeKindAsString (
      lpsrContextTypeKind contextTypeKind);
      
    enum lpsrContextExistingKind {
      kExistingContextYes, kExistingContextNo};
    
    static string contextExistingKindAsString (
      lpsrContextExistingKind contextExistingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      lpsrContextTypeKind     contextTypeKind,
      string                  contextName);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      lpsrContextTypeKind     contextTypeKind,
      string                  contextName);
      
    virtual ~lpsrContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrContextExistingKind
                          getContextExistingKind () const
                              { return fContextExistingKind; }

    lpsrContextTypeKind   getContextTypeKind () const
                              { return fContextTypeKind; }

    string                getContextName () const
                              { return fContextName; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    string                getContextTypeKindAsString () const
                              {
                                return
                                  contextTypeKindAsString (
                                    fContextTypeKind);
                              }

    string                getContextExistingKindAsString () const
                              {
                                return
                                  contextExistingKindAsString (
                                    fContextExistingKind);
                              }

    virtual void          print (ostream& os);

  protected:
  
    // fields
    // ------------------------------------------------------

    lpsrContextExistingKind
                          fContextExistingKind;
    lpsrContextTypeKind   fContextTypeKind;
    string                fContextName;

    list<S_msrElement>    fContextElementsList;
};
typedef SMARTP<lpsrContext> S_lpsrContext;
EXP ostream& operator<< (ostream& os, const S_lpsrContext& elt);

//______________________________________________________________________________
class lpsrChordNamesContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrChordNamesContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrVoice              contextVoice);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrChordNamesContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrVoice              contextVoice);
      
    virtual ~lpsrChordNamesContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getContextVoice () const
                              { return fContextVoice; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrVoice            fContextVoice;
};
typedef SMARTP<lpsrChordNamesContext> S_lpsrChordNamesContext;
EXP ostream& operator<< (ostream& os, const S_lpsrChordNamesContext& elt);

//______________________________________________________________________________
class lpsrFiguredBassContext : public lpsrContext
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrFiguredBassContext> create (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrStaff              contextStaff);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrFiguredBassContext (
      int                     inputLineNumber,
      lpsrContextExistingKind contextExistingKind,
      string                  contextName,
      S_msrStaff              contextStaff);
      
    virtual ~lpsrFiguredBassContext ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getContextStaff () const
                              { return fContextStaff; }

    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStaff            fContextStaff;
};
typedef SMARTP<lpsrFiguredBassContext> S_lpsrFiguredBassContext;
EXP ostream& operator<< (ostream& os, const S_lpsrFiguredBassContext& elt);


}


#endif
