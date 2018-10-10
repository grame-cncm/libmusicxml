/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2guidovisitor__
#define __xml2guidovisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>

#include "exports.h"
#include "guido.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"
#include "rational.h"


#include "partlistvisitor.h"


namespace MusicXML2 
{

/*!
\ingroup visitors guido
@{
*/

//______________________________________________________________________________
typedef struct {
	S_movement_title		fTitle;
	std::vector<S_creator>	fCreators;
} scoreHeader;

    /*
typedef struct {
	S_part_name		fPartName;
} partHeader;
typedef std::map<std::string, partHeader> partHeaderMap;
*/

/*!
\brief A score visitor to produce a Guido representation.
*/
//______________________________________________________________________________
class EXP xml2guidovisitor :
    public partlistvisitor,
	public visitor<S_score_partwise>,
	public visitor<S_movement_title>,
	public visitor<S_creator>,
	public visitor<S_defaults>,
	public visitor<S_part>
{
	// the guido elements stack
	std::stack<Sguidoelement>	fStack;
	bool	fGenerateComments, fGenerateStem, fGenerateBars, fGeneratePositions;
	
	scoreHeader		fHeader;		// musicxml header elements (should be flushed at the beginning of the first voice)
	//partHeaderMap	fPartHeaders;	// musicxml score-part elements (should be flushed at the beginning of each part)
	std::string		fCurrentPartID;
	int				fCurrentStaffIndex;		// the index of the current guido staff

	void start (Sguidoelement& elt)		{ fStack.push(elt); }
	void add (Sguidoelement& elt)		{ if (fStack.size()) fStack.top()->add(elt); }
	void push (Sguidoelement& elt)		{ add(elt); fStack.push(elt); }
	void pop ()							{ fStack.pop(); }

	void flushHeader	 ( scoreHeader& header );
	void flushPartHeader ( partHeader& header );
    void flushPartGroup (std::string partID);

	protected:

		virtual void visitStart( S_score_partwise& elt);
		virtual void visitStart( S_movement_title& elt);
		virtual void visitStart( S_creator& elt);
		//virtual void visitStart( S_score_part& elt);
		virtual void visitStart( S_defaults& elt);
		virtual void visitStart( S_part& elt);

		Sguidoelement& current ()				{ return fStack.top(); }
    
        bool previousStaffHasLyrics;
    
    int  fCurrentAccoladeIndex;
    
    int  fPartNum;  // 0 (default) to parse all score-parts. 1 for "P1" only, etc.
    
    int defaultStaffDistance;   // xml staff-distance value in defaults
    int defaultGuidoStaffDistance;  // the above converted to Guido value
    
    /// multimap containing <staff-num, measureNum, position, clef type>
    //std::multimap<int, std::pair< rational, string > > staffClefMap;
    std::multimap<int,  std::pair< int, std::pair< rational, string > > > staffClefMap;

    /// Containing default-x positions on a fCurrentVoicePosition (rational) of measure(int)
    std::map< int, std::map< rational, std::vector<int> > > timePositions;


    public:
				 xml2guidovisitor(bool generateComments, bool generateStem, bool generateBar=true, int partNum = 0);
		virtual ~xml2guidovisitor() {}

		Sguidoelement convert (const Sxmlelement& xml);

		// this is to control exact positionning of elements when information is present
		// ie converts relative-x/-y into dx/dy attributes
		void generatePositions (bool state)		{ fGeneratePositions = state; }

    static void addPosition	 ( Sxmlelement elt, Sguidoelement& tag, int yoffset);
	static void addPosition	 ( Sxmlelement elt, Sguidoelement& tag, int yoffset, int xoffset);
    static void addPosY	( Sxmlelement elt, Sguidoelement& tag, int yoffset, int ymultiplier);
    static void addPlacement	( Sxmlelement elt, Sguidoelement& tag);
};


} // namespace MusicXML2


#endif
