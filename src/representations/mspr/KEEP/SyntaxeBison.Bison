%{

// =========================================================================
//	SyntaxeBison.Bison
//	J. Menu - http://cui.unige.ch/DI/cours/CompInterpretes
//
//	Version $Id$ du $Date$
// =========================================================================


//#include "TerminauxFlexLista.h"

#include "LanguesSyntaxe.h"

#include <iostream>


/* Declarations necessaires */
/* ------------------------ */

extern int							yylex ();
extern int							yyerror (char * message);


// L'analyseur Lista */
/* ----------------- */

bool									gModeVerbeux;
bool									gMontrerLesTerminaux;	// pour yylex ()
%}


/* Description du terminal courant */
/* ------------------------------- */

%union {
	double								fNombre;
	CharPTR								fIdent;
	CharPTR								fChaine;
	}


/* Les terminaux du langage */
/* ------------------------ */

%token								t_NOMBRE
%token								t_IDENT
%token								t_CHAINE

%token								t_PAR_GAUCHE			t_PAR_DROITE
%token								t_EGALE					t_VIRGULE

%token								t_PLUS					t_MOINS
%token								t_FOIS					t_DIVISE
%token								t_CONCAT

%token								t_POINT_VIRGULE		t_INTERROGE


/* L'axiome du langage */
/* ------------------- */

%start Programme


%%


/* Les notions non-terminales du langage */
/* ------------------------------------- */

Programme
	:	DefinitionDeFonctionOuEvaluation
	|	Programme DefinitionDeFonctionOuEvaluation
	|	error
		{
		std::cout <<
			std::endl <<
			gLANGUE_Syntaxe ->
				IdentOuInterrogeAttendu () <<
			std::endl << std::endl;
		}
	;


DefinitionDeFonctionOuEvaluation
	:	DefinitionDeFonction
	|	Evaluation
	;


DefinitionDeFonction
	:	EnteteDeFonction
		SeparateurDefinitionDeFonction
		SequenceDExpressions
		FinDefinitionDeFonction
			{
			if (gModeVerbeux)
				std::cout <<
					gLANGUE_Syntaxe ->
						TraceDUneDefinitionDeFonction () <<
					std::endl;
			}
	;

SeparateurDefinitionDeFonction
	:	t_EGALE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					EgaleAttenduApresEnteteDeFonction () <<
				std::endl;
			}

FinDefinitionDeFonction
	:	t_POINT_VIRGULE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					PointVirguleAttenduApresUneDefinitionDeFonction () <<
				std::endl;
			}
	;

EnteteDeFonction
	:	t_IDENT
		Parametres
	;

Parametres
	:	t_PAR_GAUCHE
		ParametresConcrets
		FinParametres
	|
		/* vide */
	;

FinParametres
	:	t_PAR_DROITE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresDesParametres () <<
				std::endl;
			}
	;

ParametresConcrets
	:	UnParametre
	|	ParametresConcrets t_VIRGULE UnParametre
	;

UnParametre:
	t_IDENT
	;


Evaluation
	:	t_INTERROGE
		SequenceDExpressions
		FinEvaluation
			{
			if (gModeVerbeux)
				std::cout <<
					gLANGUE_Syntaxe ->
						TraceDUneEvaluation () <<
					std::endl;
			}
	;

FinEvaluation
	:	t_POINT_VIRGULE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					PointVirguleAttenduApresUneEvaluation ()
				<< std::endl;
			}
	;


SequenceDExpressions
	:	Expression
	|	SequenceDExpressions
		t_VIRGULE
		Expression
	;


Expression
	:	t_MOINS Terme
	|	Expression t_PLUS Terme
	|	Expression t_MOINS Terme
	|	Terme
	;


Terme
	:	Terme t_FOIS Facteur
	|	Terme t_DIVISE Facteur
	|	Terme t_CONCAT Facteur
	|	Facteur
	;

Facteur
	:	t_NOMBRE
	|	FacteurChaine
	|	t_IDENT
	|	t_PAR_GAUCHE Expression FinExpressionParenthesee
/*	|	Iteration				pour faire des essais! */
	|	AppelDeFonction
	|	error
			{
			std::cout <<
				"--> " <<
				gLANGUE_Syntaxe ->
					FacteurMalForme () <<
				std::endl << std::endl;
			}
	;

FacteurChaine
	/* on concatene les chaines successives */
	:	FacteurChaine t_CHAINE
	|	t_CHAINE
	;

FinExpressionParenthesee
	:	t_PAR_DROITE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresExpressionParenthesee () <<
				std::endl;
			}
	;


/* productions inutilisees, pour memoire a titre d'exemple
Iteration
	:	t_IDENT
		t_PAR_GAUCHE
		t_IDENT
		SeparateurIteration
		Expression
		SeparateurIteration
		Expression
		SeparateurIteration
		Expression
		FinIteration
	;

SeparateurIteration
	:	t_VIRGULE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					VirguleAttendueDansUneIteration () <<
				std::endl;
			}
	;

FinIteration
	:	t_PAR_DROITE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					PointVirguleAttenduApresUneIteration () <<
				std::endl;
			}
	;
*/


AppelDeFonction
	:	t_IDENT
		t_PAR_GAUCHE
		Arguments
		FinAppelDeFonction
	;

FinAppelDeFonction
	:	t_PAR_DROITE
	|	error
			{
			std::cout <<
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresUnAppelDeFonction () <<
				std::endl;
			}
	;

Arguments
	:	ArgumentsConcrets
	|	/* vide */
	;


ArgumentsConcrets
	/* On accepte un sur-langage, pas de controle sur le nombre! */
	:	UnArgument
	|	ArgumentsConcrets t_VIRGULE UnArgument
	;

UnArgument:
		Expression
	;


%%


/* On doit fournir l'analyseur lexical */
/* ----------------------------------- */

#include "SyntaxeBison.yy.cc"


/* Autre code de service */
