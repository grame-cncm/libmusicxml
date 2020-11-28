/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

//#include <iomanip>      // setw, setprecision, ...
//#include <cmath>
//#include <string>

#include <sstream>

#include "mxmlTree.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
string mxmlElementAsString (Sxmlelement elem)
{
  int                 elemType = elem->getType ();
  const std::string&  elemName = elem->getName ();
  const std::vector<Sxmlattribute>&
                      elemAttributes = elem->attributes();

  stringstream s;

  s <<
    "mxmlElement " <<
    ", elemType: " << elemType <<
    ", elemName: " << elemName <<
    ", elemAttributes.size ():" << elemAttributes.size ();

  return s.str ();
}

//------------------------------------------------------------------------
Sxmlelement createMxmlElement (int type, const string& value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

Sxmlelement createMxmlIntegerElement (int type, int value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

Sxmlelement createMxmlFloatElement (int type, float value)
{
	Sxmlelement result = factory::instance().create(type);

	result->setValue (value);

	return result;
}

//------------------------------------------------------------------------
Sxmlattribute createMxmlAttribute (const string& name, const string& value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

Sxmlattribute createMxmlIntegerAttribute (const string& name, int value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

Sxmlattribute createMxmlFloatAttribute (const string& name, float value)
{
	Sxmlattribute result = xmlattribute::create();

	result->setName(name);
	result->setValue(value);

	return result;
}

//------------------------------------------------------------------------
SXMLFile createMxmlFile ()
{
	SXMLFile result = TXMLFile::create ();

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
	result->set (xmlDecl);

	TDocType * docType = new TDocType ("score-partwise");
	result->set (docType);

	return result;
}

//------------------------------------------------------------------------
Sxmlelement createMxmlScorePartWiseElement ()
{
  Sxmlelement result = factory::instance().create(k_score_partwise);

  Sxmlattribute versionAttribute = createMxmlAttribute("version", "3.1");
  result->add (versionAttribute);

  return result;
}


} // namespace

