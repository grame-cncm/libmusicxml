%{

// =========================================================================
//	Semantique.Bison
//	J. Menu - http://cui.unige.ch/DI/cours/CompInterpretes
//
//	Version $Id$ du $Date$
// =========================================================================


#include "LanguesSyntaxe.h"
#include "LanguesSemantique.h"

#include "AlgebreBooleenne.h"
#include "Arithmetique.h"
#include "ChainesDeCaracteres.h"
#include "Comparaisons.h"
#include "EntreesSorties.h"
#include "Sequencements.h"
#include "Si.h"
#include "Iterations.h"

#include "SupportFlexBison.h"
#include "SupportFlexBisonLista.h"

#include "AnalyseurListaFlexBison.h"

#include <iostream>


/* Declarations necessaires */
/* ------------------------ */

extern	int								yylex ();
extern	int								yyerror (char * message);


// L'analyseur Lista */
/* ----------------- */

AnalyseurListaFlexBisonPTR				gAnalyseurListaFlexBison;
extern AnalyseurSemantiqueListaPTR	gAnalyseurSemantiqueLista;
													// le meme, vu plus specifiquement

%}


/* Description du terminal courant */
/* ------------------------------- */

%union {
	double									fNombre;
	DescrIdentificateurFlexBison		fDescrIdentificateurFlexBison;
	CharPTR									fChaine;

	FonctionUtilisateurPTR				fFonctionUtilisateur;
	DescrSemantiquePTR					fGrapheSemantique;
	}


/* Les terminaux du langage */
/* ------------------------ */

%token	<fNombre>						t_NOMBRE

%token	<fDescrIdentificateurFlexBison>	t_IDENT
%token	<fDescrIdentificateurFlexBison>	t_ITERATEUR
%token	<fDescrIdentificateurFlexBison>	t_SEQUENCEUR

%token	<fChaine>						t_CHAINE

%token										t_PAR_GAUCHE			t_PAR_DROITE
%token										t_EGALE					t_VIRGULE

%token										t_PLUS					t_MOINS
%token										t_FOIS					t_DIVISE
%token										t_CONCAT

%token										t_POINT_VIRGULE		t_INTERROGE


/* Les valeurs décrivant les notions non-terminales */
/* ------------------------------------------------ */

%type		<fFonctionUtilisateur>		EnteteDeFonction

%type		<fGrapheSemantique>			Expression
%type		<fGrapheSemantique>			Terme
%type		<fGrapheSemantique>			Facteur
%type		<fChaine>						FacteurChaine

%type		<fDescrIdentificateurFlexBison>	Identificateur

%type		<fGrapheSemantique>			Iteration
%type		<fGrapheSemantique>			Sequence
%type		<fGrapheSemantique>			SequenceDExpressions

%type		<fGrapheSemantique>			AppelDeFonction


/* L'axiome du langage */
/* ------------------- */

%start Programme


%%


/* Les notions non-terminales du langage */
/* ------------------------------------- */

Identificateur
	:	t_IDENT
	|	t_ITERATEUR
	|	t_SEQUENCEUR
	;


Programme
	:	DefinitionDeFonctionOuEvaluation
	|	Programme DefinitionDeFonctionOuEvaluation
	|	error
		{
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				IdentOuInterrogeAttendu () );
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
			{
			gAnalyseurListaFlexBison ->
				TraiterCorpsDefinitionDeFonction ($1, $3);
			}
			
		FinDefinitionDeFonction
			{
			gAnalyseurListaFlexBison ->
				TraiterFinDefinitionDeFonction ($1, $3);
			}
	;	/* DefinitionDeFonction */

SeparateurDefinitionDeFonction
	:	t_EGALE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					EgaleAttenduApresEnteteDeFonction () );
			}
	;

FinDefinitionDeFonction
	:	t_POINT_VIRGULE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					PointVirguleAttenduApresUneDefinitionDeFonction () );
			}
	;

EnteteDeFonction
	:	Identificateur
			{
			gAnalyseurListaFlexBison ->
				TraiterIdentFonction ($1);
			}

		Parametres
	
			{
			$$ = gAnalyseurListaFlexBison -> IdentFonction ();
			}

	;	/* EnteteDeFonction */


Parametres
	:	t_PAR_GAUCHE
			{
			gAnalyseurListaFlexBison ->
				TraiterDebutParametres ();
			}

		ParametresConcrets

		FinParametres
		
	|
		/* vide */
	;

FinParametres
	:	t_PAR_DROITE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresDesParametres () );
			}
	;

ParametresConcrets
	:	UnParametre
	|	ParametresConcrets t_VIRGULE UnParametre
	;

UnParametre:	
	Identificateur
			{
			gAnalyseurListaFlexBison ->
				TraiterParametre ($1);
			}
	;	/* UnParametre */


Evaluation
	:	t_INTERROGE
			{
			gAnalyseurListaFlexBison ->
				TraiterDebutEvaluation ();
			}

		SequenceDExpressions
	
		FinEvaluation
			{
			gAnalyseurListaFlexBison ->
				TraiterFinEvaluation ($3);
			}
	;	/* Evaluation */

FinEvaluation
	:	t_POINT_VIRGULE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					PointVirguleAttenduApresUneEvaluation () );
			}
	;


SequenceDExpressions
	/*
		recursion a DROITE pour construire directement la sequence comme 
		une 'liste', en une seule passe, sans la reparcourir après coup
	*/
	:	Expression

	|	Expression

		t_VIRGULE

		SequenceDExpressions
			{
			$$ =
				new Seq (
					gLANGUE_Semantique ->
						SeqImplicite (),
					$1,
					$3 );
			}
	;


Expression
	:	t_MOINS
		Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$2 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );

			$$ = new MoinsUnaire ("-", $2);
			}

	|	Expression
			{
			gAnalyseurSemantiqueLista -> 
				TesterLeTypeAttendu (
					gTypeNombre,
					$1 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Expression () );
			}
		t_PLUS
		Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$4 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );

			$$ = new Plus ("+", $1, $4);
			}

	|	Expression
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$1 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Expression () );
			}
		t_MOINS
		Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$4 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );

			$$ = new Moins ("-", $1, $4);
			}

	|	Terme
	;


Terme
	:	Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$1 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );
			}
		t_FOIS
		Facteur
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$4 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Facteur () );

			$$ = new Fois ("*", $1, $4);
			}

	|	Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$1 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );
			}
		t_DIVISE
		Facteur
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeNombre,
					$4 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Facteur () );

			$$ = new DivisePar ("/", $1, $4);
			}

	|	Terme
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeChaine,
					$1 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Terme () );
			}
		t_CONCAT
		Facteur
			{
			gAnalyseurSemantiqueLista ->
				TesterLeTypeAttendu (
					gTypeChaine,
					$4 -> TypeLogique (),
					gLANGUE_Syntaxe ->
						Facteur () );

			$$ = new ConcatChaines ("!!", $1, $4);
			}

	|	Facteur
	;


Facteur
	:	t_NOMBRE
			{ $$ = new ValeurNombre ($1); }

	|	FacteurChaine
			{ $$ = new ValeurChaine ($1); }

	|	t_IDENT
			{
			$$ = gAnalyseurListaFlexBison ->
				TraiterFacteurIdentSimple ($1);
			}

	|	t_PAR_GAUCHE Expression FinExpressionParenthesee
			{ $$ = $2; }

	|	Iteration

	|	Sequence

	|	AppelDeFonction

	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					FacteurMalForme () );

			$$ = gDescrSemantiqueInconnue;
			}
	;

FacteurChaine
	/* on concatene les chaines successives */
	:	FacteurChaine t_CHAINE
		{
		Chaine	nouvelleChaine = FConcatChaines ($1, $2);

		delete $1;
		$$ = (CharPTR) nouvelleChaine.c_str ();
		}
	|	t_CHAINE
	;

FinExpressionParenthesee
	:	t_PAR_DROITE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresExpressionParenthesee () );
			}
	;


Iteration
	:	t_ITERATEUR
			{
			gAnalyseurListaFlexBison ->
				TraiterDebutIteration ($1);
				/*	empile une description d'appel */
			}

		t_PAR_GAUCHE

		Identificateur

		SeparateurIteration

		Expression
			{
			gAnalyseurListaFlexBison ->
				TraiterBorneInf ($6);
				/*	utilise la description d'appel */
			}

		SeparateurIteration

		Expression
			{
			gAnalyseurListaFlexBison ->
				TraiterBorneSup ($9);
				/*	utilise la description d'appel */
			}

		SeparateurIteration

			{
			gAnalyseurListaFlexBison ->
				TraiterIndiceIteration ($4);
				/* empile un nouveau niveau de déclaration */
				/*	utilise la description d'appel */
			}

		Expression
			{
			gAnalyseurListaFlexBison ->
				TraiterExprIteree ($13);
				/* utilise le nouveau niveau de déclaration */
				/*	utilise la description d'appel */
			}

		FinIteration
			{
			$$ = gAnalyseurListaFlexBison ->
				TraiterFinIteration ();
				/* désempile le nouveau niveau de déclaration */
				/*	désempile la description d'appel */
			}
	;

SeparateurIteration
	:	t_VIRGULE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					VirguleAttendueDansUneIteration () );
			}
	;

FinIteration
	:	t_PAR_DROITE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresUneIteration () );
			}
	;


Sequence
	:	t_SEQUENCEUR

		t_PAR_GAUCHE
		
		Expression

		t_VIRGULE

		SequenceDExpressions

		FinSequence
			{
			$$ = gAnalyseurListaFlexBison ->
				TraiterSequence ($1, $3, $5);
			}
	;

FinSequence
	:	t_PAR_DROITE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresUneSequence () );
			}
	;


AppelDeFonction
	:	t_IDENT
			{
			gAnalyseurListaFlexBison ->
				TraiterDebutAppelFonction ($1);
				/*	empile une description d'appel */
			}

		t_PAR_GAUCHE

		Arguments
				/*	utilise la description d'appel */

		FinAppelDeFonction
			{
			$<fGrapheSemantique>$ =
				gAnalyseurListaFlexBison ->
					TraiterFinAppelFonction ();
				/*	désempile la description d'appel */
			}
	;

FinAppelDeFonction
	:	t_PAR_DROITE
	|	error
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresUnAppelDeFonction () );
			}
	;

Arguments
	:	ArgumentsConcrets
	|	/* vide */
	;

ArgumentsConcrets
	:	UnArgument
	|	ArgumentsConcrets t_VIRGULE UnArgument
	;

UnArgument:
			{
			gAnalyseurListaFlexBison ->
				TraiterDebutArgument ();
			}
				
		Expression

			{
			gAnalyseurListaFlexBison ->
				TraiterFinArgument ($2);
			}
	;	/* UnArgument */


%%

/* Code de service */

#include "Semantique.yy.cc"			// fournit l'analyseur lexical
