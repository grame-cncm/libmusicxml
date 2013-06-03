/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/


#ifndef __xml2antescofovisitor__
#define __xml2antescofovisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>

#include "exports.h"
#include "antescofotree.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"
#include "antescofowriter.h"

namespace MusicXML2 
{
/*!
\ingroup visitors antescofo
@{
*/

//______________________________________________________________________________
typedef struct {
	S_movement_title		fTitle;
	std::vector<S_creator>	fCreators;
} scoreHeader;

typedef struct {
	S_part_name		fPartName;
} partHeader;
typedef std::map<std::string, partHeader> partHeaderMap;


/*!
\brief A score visitor to produce a antescofo representation.
*/
//______________________________________________________________________________
class EXP xml2antescofovisitor : 
	public visitor<S_score_partwise>,
	public visitor<S_movement_title>,
	public visitor<S_creator>,
	public visitor<S_score_part>,
	public visitor<S_part_name>,
	public visitor<S_part>
{
	// the antescofo elements stack
	std::stack<Santescofoelement>	fStack;
	bool	fGenerateComments, fGenerateStem, fGenerateBars, fGeneratePositions;

	scoreHeader	fHeader;	// musicxml header elements (should be flushed at the beginning of the first voice)
	partHeaderMap	fPartHeaders;	// musicxml score-part elements (should be flushed at the beginning of each part)
	std::string	fCurrentPartID;
	int		fCurrentStaffIndex;		// the index of the current antescofo staff

	void start (Santescofoelement& elt)		{ fStack.push(elt); }
	void add (Santescofoelement& elt)		{ fStack.top()->add(elt); }
	void push (Santescofoelement& elt)		{ add(elt); fStack.push(elt); }
	void pop ()							{ fStack.pop(); }


	void flushHeader ( scoreHeader& header );
	void flushPartHeader ( partHeader& header );

	protected:

	virtual void visitStart( S_score_partwise& elt);
	virtual void visitStart( S_movement_title& elt);
	virtual void visitStart( S_creator& elt);
	virtual void visitStart( S_score_part& elt);
	virtual void visitStart( S_part_name& elt);
	virtual void visitStart( S_part& elt);
	Santescofoelement& current ()	{ return fStack.top(); }

	public:
	xml2antescofovisitor(antescofowriter& w, bool generateComments, bool generateStem, bool generateBar=true);
	virtual ~xml2antescofovisitor() {}

	Santescofoelement convert (const Sxmlelement& xml);

	antescofowriter& w;

	// this is to control exact positionning of elements when information is present
	// ie converts relative-x/-y into dx/dy attributes
	void generatePositions (bool state)		{ fGeneratePositions = state; }

	static void addPosition	 ( Sxmlelement elt, Santescofoelement& tag, int yoffset);
};


} // namespace MusicXML2


#endif
