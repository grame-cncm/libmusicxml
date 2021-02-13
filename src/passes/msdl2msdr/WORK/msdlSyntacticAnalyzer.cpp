// =========================================================================
//	AnalyseurDescendantLista.cc
//	J. Menu - http://cui.unige.ch/DI/cours/CompInterpretes
//
//	Version $Id$ du $Date$
// =========================================================================


#include "AnalyseurDescendantLista.h"

#include "LanguesSyntaxe.h"


// --------------------------------------------------------------------------
//	AnalyseurDescendantLista
// --------------------------------------------------------------------------

AnalyseurDescendantLista :: AnalyseurDescendantLista (
	AnalyseurLexicalLista	* lAnalyseurLexical,
	OptionsSyntaxePTR			lesOptionsSyntaxe )
	{
	fAnalyseurLexical = lAnalyseurLexical;
	fOptionsSyntaxe = lesOptionsSyntaxe;
	
	fSourceCorrectSyntaxiquement = true;
	}


void 
AnalyseurDescendantLista :: Analyser ()
	{
	Avancer ();	//	on amorce la pompe
	
	Programme ();

	if (fAnalyseurLexical -> SourceCorrectLexicalement ())
		std::cout <<
			gLANGUE_Lexique ->
				CodeLexicalementCorrect () <<
			std::endl;
	else
		std::cout <<
			gLANGUE_Lexique ->
				CodeLexicalementIncorrect () <<
			std::endl;

	if (fSourceCorrectSyntaxiquement)
		std::cout <<
			gLANGUE_Syntaxe ->
				CodeSyntaxiquementCorrect () <<
			std::endl;
	else
		std::cout <<
			gLANGUE_Syntaxe ->
				CodeSyntaxiquementIncorrect () <<
			std::endl;
	}	// AnalyseurDescendantLista :: Analyser

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Programme ()
	{
	do
		{
		switch (fTerminalPCourant)
			{
			case t_INTERROGE :
				Evaluation ();
				break;
			case t_IDENT :
				DefinitionDeFonction ();
				break;
			default :
				std::cout << std::endl;
				ErreurSyntaxique (
					gLANGUE_Syntaxe ->
						IdentOuInterrogeAttendu ());
				std::cout << std::endl;
				Avancer ();
			} // switch
		}
	while (fTerminalPCourant != t_FIN);
	}	//	AnalyseurDescendantLista :: Programme

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: DefinitionDeFonction ()
	{
	EnteteDeFonction ();

	if (fTerminalPCourant != t_EGALE)
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				EgaleAttenduApresEnteteDeFonction () );
	else
		Avancer ();
	
	SequenceDExpressions ();

	if (fTerminalPCourant != t_POINT_VIRGULE)
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				PointVirguleAttenduApresUneDefinitionDeFonction () );
	Avancer ();

	if (fOptionsSyntaxe -> ModeVerbeux ())
		std::cout <<
			gLANGUE_Syntaxe ->
				TraceDUneDefinitionDeFonction () <<
			std::endl;
	}	//	AnalyseurDescendantLista :: DefinitionDeFonction

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: EnteteDeFonction ()
	{
	if (fTerminalPCourant != t_IDENT)
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				IdentAttenduCommeNomDeFonction () );
	else
		Avancer ();

	if (fTerminalPCourant == t_PAR_GAUCHE)
		{
		Avancer ();

		Parametres ();

		if (fTerminalPCourant != t_PAR_DROITE)
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					ParentheseAttendueApresDesParametres () );
		else
			Avancer ();
		}
	}	//	AnalyseurDescendantLista :: EnteteDeFonction

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Parametres ()
	{
	if (fTerminalPCourant != t_IDENT)
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				IdentAttenduCommeParametreDeFonction () );
	else
		Avancer ();

	while (fTerminalPCourant == t_VIRGULE)
		{
		Avancer ();

		if (fTerminalPCourant != t_IDENT)
			ErreurSyntaxique (
				gLANGUE_Syntaxe ->
					IdentAttenduCommeParametreDeFonction () );
		else
			Avancer ();
		}
	}	//	AnalyseurDescendantLista :: Parametres

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Evaluation ()
	{
	Avancer ();		//	consomme t_INTERROGE

	SequenceDExpressions ();

	if (fTerminalPCourant != t_POINT_VIRGULE)
		ErreurSyntaxique (
			gLANGUE_Syntaxe ->
				PointVirguleAttenduApresUneEvaluation () );
	else
		Avancer ();

	if (fOptionsSyntaxe -> ModeVerbeux ())
		std::cout <<
			gLANGUE_Syntaxe ->
				TraceDUneEvaluation () <<
			std::endl;
	}	//	AnalyseurDescendantLista :: Evaluation

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: SequenceDExpressions ()
	{
	Expression ();

	if (fTerminalPCourant == t_VIRGULE)
		{
		Avancer ();	//	on l'accepte
		
		SequenceDExpressions ();
		}	// while
	}

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Expression ()
	{
	if (fTerminalPCourant == t_MOINS)
		Avancer ();	//	on l'accepte

	Terme ();
	
	while (
		fTerminalPCourant == t_PLUS || fTerminalPCourant == t_MOINS
		)
		{
		Avancer ();

		Terme ();
		}
	}	//	AnalyseurDescendantLista :: Expression

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Terme ()
	{
	Facteur ();
	
	while (
		fTerminalPCourant == t_FOIS
			||
		fTerminalPCourant == t_DIVISE
			||
		fTerminalPCourant == t_CONCAT
		)
		{
		Avancer ();
		
		Facteur ();
		}
	}	//	AnalyseurDescendantLista :: Terme

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Facteur ()
	{
	switch (fTerminalPCourant)
		{
		case t_NOMBRE:
			Avancer ();	//	on l'accepte
			break;

		case t_CHAINE:
			Avancer ();	//	on l'accepte
			while (fTerminalPCourant == t_CHAINE)
				// on concatene les chaines successives
				Avancer ();
			break;

		case t_IDENT:
			Avancer ();

			if (fTerminalPCourant == t_PAR_GAUCHE)
				{
				Avancer ();

				AppelDeFonction ();

				if (fTerminalPCourant != t_PAR_DROITE)
					ErreurSyntaxique (
						gLANGUE_Syntaxe ->
							FormatParentheseAttendueApresUnAppelDeFonction () );
				else
					Avancer ();
				}
			else
				;	//	on reste sur ce terminal
			break;

		case t_PAR_GAUCHE:
			Avancer ();

			Expression ();

			if (fTerminalPCourant != t_PAR_DROITE)
				ErreurSyntaxique (
					gLANGUE_Syntaxe ->
						ParentheseAttendueApresExpressionParenthesee () );
			else
				Avancer ();
			break;
		
		default:
			{
			ErreurSyntaxique (
				gLANGUE_Syntaxe -> FacteurMalForme () );
			Avancer ();
			}
		
		}	//	switch
	}	//	AnalyseurDescendantLista :: Facteur

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: AppelDeFonction ()
	{
	//	IDENT PAR_GAUCHE ont ŽtŽ acceptŽs

	if (fTerminalPCourant != t_PAR_DROITE)
		{
		// certaines fonctions prŽdŽfinies n'ont pas d'arguments d'appel
		Arguments ();
		}
	}	//	AnalyseurDescendantLista :: AppelDeFonction

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: Arguments ()
	{
	Expression ();

	while (fTerminalPCourant == t_VIRGULE)
		{
		Avancer ();
		Expression ();
		}
	}	//	AnalyseurDescendantLista :: Arguments

// -------------------------------------------------------------------------

void 
AnalyseurDescendantLista :: ErreurSyntaxique (Chaine leMessage)
	{
	if (fOptionsSyntaxe -> ModeVerbeux ())
		std::cout <<
			MiseEnForme (
				gLANGUE_Lexique ->
					FormatLocalisationErreur (),
				gLANGUE_Syntaxe ->
					ErreurSyntaxique (),
				leMessage,
				fAnalyseurLexical -> DescriptionTerminalCourant (),
				fAnalyseurLexical -> DescriptionPositionCourante () ) <<
			std::endl;
	else
		std::cout <<
			MiseEnForme (
				gLANGUE_Lexique ->
					FormatLocalisationErreurAllege (),
				fAnalyseurLexical -> DescriptionPositionCourante (),
				gLANGUE_Syntaxe ->
					ErreurSyntaxique (),
				leMessage,
				fAnalyseurLexical -> DescriptionTerminalCourant () ) <<
			std::endl;

	fSourceCorrectSyntaxiquement = false;
	
	if (fOptionsSyntaxe -> RenoncerSurErreurSyntaxique ())
		SortieSurErreur (leMessage, 1);
	}	//	AnalyseurDescendantLista :: ErreurSyntaxique
