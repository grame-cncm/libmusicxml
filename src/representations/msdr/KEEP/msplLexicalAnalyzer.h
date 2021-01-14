/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msplLexicalAnalyzer__
#define __msplLexicalAnalyzer__

#include "msprBasicTypes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahAtomsCollection.h"


namespace MusicXML2
{

//______________________________________________________________________________
// the MPSL tokens

enum mpslTokenKind {
  k_NoToken,
	t_FIN,					//	0, sera retourne par 'yylex ()' lorsque 'yywrap ()'
								//	retournera lui-meme une valeur non-nulle

	t_NOMBRE,				t_IDENT,					t_CHAINE,

	t_PAR_GAUCHE,			t_PAR_DROITE,
	t_EGALE,					t_VIRGULE,

	t_PLUS,					t_MOINS,					t_FOIS,					t_DIVISE,
	t_CONCAT,

	t_POINT_VIRGULE,		t_INTERROGE
};

//______________________________________________________________________________
// the MPSL token description

class msplTokenDescription
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msplLexicalAnalyzer> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msplLexicalAnalyzer ();

    virtual               ~msplLexicalAnalyzer ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getIntegerValue () const
                              { return fIntegerValue; }

  public:

    int   							  fIntegerValue;
    double							  fDoubleValue;

    string							  fStringValue;

    rational              fRationalValue;
};

//______________________________________________________________________________
// the MPSL lexical analyzer

class msplLexicalAnalyzer : smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msplLexicalAnalyzer> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msplLexicalAnalyzer ();

    virtual               ~msplLexicalAnalyzer ();

  public:

    // set and get
    // ------------------------------------------------------

    mpslTokenKind         getCurrentTokenKind () const
                              { return fCurrentTokenKind; }

    int                   getIntegerValue () const
                              { return fIntegerValue; }

    double                getCurrentDoubleValue () const
                              { return fCurrentDoubleValue; }

    string                getfCurrentDoubleValue () const
                              { return ffCurrentDoubleValue; }

    rational              getRationalValue () const
                              { return fRationalValue; }


  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    // the current token
    mpslTokenKind         fCurrentTokenKind;

    // the values associated with it
    int   							  fCurrentIntegerValue;
    double							  fCurrentDoubleValue;

    string							  fCurrentStringValue;

    rational              fCurrentRationalValue;
};
typedef SMARTP<msplLexicalAnalyzer> S_msplLexicalAnalyzer;
EXP ostream& operator<< (ostream& os, const S_msplLexicalAnalyzer& elt);


}


#endif /* __TerminauxFlexLista__ */
