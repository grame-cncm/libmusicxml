%{

/* déclarations globales */

// =========================================================================
//	Semantique.Flex
//	J. Menu - http://cui.unige.ch/DI/cours/CompInterpretes
//
//	Version $Id$ du $Date$
// =========================================================================


/* ---------------------------------------------------------------------- */
/* Inclusions */
/* ---------------------------------------------------------------------- */

#include "Langues.h"

#include "SupportFlexBison.h"	//	Fournit 'yywrap ()' et 'ErreurLexicale ()'

#include <iostream>

#include <cstdlib>
#include <cstdio>


/* ---------------------------------------------------------------------- */
/* Fonctions */
/* ---------------------------------------------------------------------- */

void									AppondreLeFragmentALaChaine (
											CharPTR leFragment );

TerminalFlexBison					EnregistrerTerminal (
											 TerminalFlexBison leTerminalFlexBison );

%}


/* ---------------------------------------------------------------------- */
/* Options */
/* ---------------------------------------------------------------------- */

%option	yylineno


/* ---------------------------------------------------------------------- */
/* Expressions régulières */
/* ---------------------------------------------------------------------- */

blancs								[ \t]+
finDeLigne							[\n]

lettre								[A-Za-zéèêàâòôùûî]
chiffre10							[0-9]

ident									{lettre}(_|{lettre}|{chiffre10})*
entier10								{chiffre10}+
exposant								[eE][+-]?{entier10}

guillemet							[\"]
tabulateur							[\t]
backSlash							[\\]


/* modes exclusif, pour que les autres productions ne s'appliquent pas */
%x										ACCEPTATION_D_UNE_CHAINE
%x										COMMENTAIRE_PARENTHESE
%x										COMMENTAIRE_SUR_UNE_LIGNE


%%	/* Les terminaux du langage et les actions correspondantes */


%{
/* Variables locales à "yylex" */
%}


"/*"									{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										BEGIN COMMENTAIRE_PARENTHESE;
										}

<COMMENTAIRE_PARENTHESE>[^*{finDeLigne}]*	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										/* on accepte tout caractere autre que '*' */
										}
<COMMENTAIRE_PARENTHESE>"*"+[^*/{finDeLigne}]*	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										/* on accepte toues les '*' non suivies d'un '/' */
										}
<COMMENTAIRE_PARENTHESE>"*"+"/"	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										BEGIN INITIAL;
										}
<COMMENTAIRE_PARENTHESE>.	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										}


"//"									{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										BEGIN COMMENTAIRE_SUR_UNE_LIGNE;
										}

<COMMENTAIRE_SUR_UNE_LIGNE>{finDeLigne}	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;

										CompterFinDeLigne ();

										BEGIN INITIAL;
										}
<COMMENTAIRE_SUR_UNE_LIGNE>.	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										/*
										on accepte tout caractere qui n'est pas
										une {finDeLigne}
										*/
										}


{guillemet}							{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										yylval.fChaine = CharPTR ("");
										BEGIN ACCEPTATION_D_UNE_CHAINE;
										}

<ACCEPTATION_D_UNE_CHAINE>{guillemet}	{
										AppondreAuSourceCumule ();

										BEGIN INITIAL;
										/* yylval.fChaine est deja garni */
										return EnregistrerTerminal (t_CHAINE);
										}

<ACCEPTATION_D_UNE_CHAINE>{backSlash}{guillemet}	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										AppondreLeFragmentALaChaine (CharPTR ("\""));
										}
<ACCEPTATION_D_UNE_CHAINE>{backSlash}n	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										AppondreLeFragmentALaChaine (CharPTR ("\n"));
										}
<ACCEPTATION_D_UNE_CHAINE>{backSlash}t	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										AppondreLeFragmentALaChaine (CharPTR ("\t"));
										}
<ACCEPTATION_D_UNE_CHAINE>{backSlash}{backSlash}	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										AppondreLeFragmentALaChaine (CharPTR ("\\"));
										}
<ACCEPTATION_D_UNE_CHAINE>{finDeLigne}	{
										AppondreAuSourceCumule ();

										CompterFinDeLigne ();

										AppondreLeFragmentALaChaine (yytext);
										}
<ACCEPTATION_D_UNE_CHAINE>.	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										AppondreLeFragmentALaChaine (yytext);
										}


{blancs}								{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										/* on ignore ces separateurs */
										}

{finDeLigne}						{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;

										CompterFinDeLigne ();
										/* on ignore ce separateur */
										}

{entier10}"."{entier10}({exposant})?	|
{entier10}({exposant})?			{
										AppondreAuSourceCumule ();

										yylval.fNombre = atof (yytext);
										return EnregistrerTerminal (t_NOMBRE);
										}

{ident}								{
										AppondreAuSourceCumule ();

										// en cas de traduction, la production de
										// l'identificateur traduit est faite dans
										// InteractionLexiqueSemantique ()

										yylval.fDescrIdentificateurFlexBison.fNom =
											SauvegarderChaine (yytext);

										GenreIdentificateurFlexBison
													leLeGenreIdentificateurFlexBison =
														gAnalyseurListaFlexBison ->
															InteractionLexiqueSemantique (
																yylval.fDescrIdentificateurFlexBison );
														//	garnit yylval.fIdentificateurPTR

										switch (leLeGenreIdentificateurFlexBison)
											{
											case kIterateurFlexBison :
												return EnregistrerTerminal (t_ITERATEUR);
											case kSequenceurFlexBison :
												return EnregistrerTerminal (t_SEQUENCEUR);
											case kAutreIdentificateurFlexBison :
												return EnregistrerTerminal (t_IDENT);
											}	// switch
										}


"("									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_PAR_GAUCHE);
										}
")"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_PAR_DROITE);
										}

"="									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_EGALE);
										}
","									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_VIRGULE);
										}

"+"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_PLUS);
										}
"-"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_MOINS);
										}
"*"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_FOIS);
										}
"/"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_DIVISE);
										}
"!!"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_CONCAT);
										}

";"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_POINT_VIRGULE);
										}
"?"									{
										AppondreAuSourceCumule ();

										return EnregistrerTerminal (t_INTERROGE);
										}


.										{
										AppondreAuSourceCumule ();

										ErreurLexicale (
											MiseEnForme (
												gLANGUE_Lexique ->
													FormatCaractereIllegal(),
												yytext [0]) );
										}


%%

/* ---------------------------------------------------------------------- */
/*	Le code de service */
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
//	AppondreLeFragmentALaChaine
/* ---------------------------------------------------------------------- */

void
AppondreLeFragmentALaChaine (CharPTR leFragment)
	{
	yylval.fChaine =
		(CharPTR) FConcatChaines (yylval.fChaine, leFragment);


/* JMI
<COMMENTAIRE_PARENTHESE>{finDeLigne}	{
										AppondreAuSourceCumule ();

										if (gTraduire)
											ECHO;
										CompterFinDeLigne ();
										}
*/

	}


/* ---------------------------------------------------------------------- */
//	EnregistrerTerminal
/* ---------------------------------------------------------------------- */

TerminalFlexBison
EnregistrerTerminal (TerminalFlexBison leTerminalFlexBison)
	{
	if (gAnalyseurListaFlexBison -> MontrerLesTerminaux ())
		std::cout << "\t" <<
		TerminalSousFormeTextuelle (leTerminalFlexBison) << std::endl;

	if (gTraduire)
		{
		switch (leTerminalFlexBison)
			{
			case t_IDENT :
			case t_ITERATEUR :
			case t_SEQUENCEUR :
				break;
			default :
				ECHO;
			}	// switch
		}	// if

	gTerminalCourantFlexBison = leTerminalFlexBison;
	gPremierTerminalDejaAccepte = true;

	return leTerminalFlexBison;
	}
