// =========================================================================
//	AnalyseurDescendantLista.h
//	J. Menu - http://cui.unige.ch/DI/cours/CompInterpretes
//
//	Version $Id$ du $Date$
// =========================================================================


#ifndef __AnalyseurDescendantLista__
#define __AnalyseurDescendantLista__


#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "DecodageDOptionsSyntaxe.h"

 std::string name;

  std::cout << "Please, enter your full name: ";
  std::getline (std::cin,name);
  std::cout << "Hello, " << name << "!\n";


// --------------------------------------------------------------------------
//	DescriptionTerminal
// --------------------------------------------------------------------------

struct DescriptionTerminal
	// pas une union, qui ne pourrait contenir des objets
	{
	double							fNombre;
	Chaine							fIdent;
	Chaine							fChaine;
	};


// --------------------------------------------------------------------------
//	AnalyseurDescendantLista
// --------------------------------------------------------------------------

class AnalyseurDescendantLista
	{
public:

										AnalyseurDescendantLista (
											AnalyseurLexicalLista	* lAnalyseurLexical,
											OptionsSyntaxePTR			lesOptionsSyntaxe );

	void								Analyser ();

private:

	AnalyseurLexicalLista		* fAnalyseurLexical;

	OptionsSyntaxePTR				fOptionsSyntaxe;

	bool								fSourceCorrectSyntaxiquement;

	TerminalP						fTerminalPCourant;
	void								Avancer ();

	void								Programme ();

	void								DefinitionDeFonction ();
	void								EnteteDeFonction ();
	void								Parametres ();

	void								Evaluation ();

	void								SequenceDExpressions ();

	void								Expression ();
	void								Terme ();
	void								Facteur ();

	void								AppelDeFonction ();
	void								Arguments ();

	void								ErreurSyntaxique (Chaine leMessage);

	};	//	AnalyseurDescendantLista


inline
void
AnalyseurDescendantLista :: Avancer ()
	{
	fTerminalPCourant = fAnalyseurLexical -> AccepterUnTerminal ();
	}


#endif /* __AnalyseurDescendantLista__ */
