/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __harmonyVisitor__
#define __harmonyVisitor__

#include "typedefs.h"
#include "visitor.h"

namespace MusicXML2 
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief key element data
*/
class EXP harmonyvisitor :
	public visitor<S_harmony>,
	public visitor<S_root>,
    public visitor<S_root_step>,
    public visitor<S_root_alter>,
	public visitor<S_kind>
{
	protected:
                void reset();
		virtual void visitStart ( S_harmony& elt );
		virtual void visitStart ( S_root& elt );
		virtual void visitStart ( S_kind& elt );
        virtual void visitStart ( S_root_alter& elt );
        virtual void visitStart ( S_root_step& elt );

	public:
				 harmonyvisitor() { reset(); }
		virtual ~harmonyvisitor() {}

		std::string	fText;
        std::string	fRoot;
        std::string	fAlteration;
};


/*! @} */

}

#endif
