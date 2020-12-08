/*
  MusicXML Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __liboptionsvector__
#define __liboptionsvector__

// this file requires the use of C++

namespace MusicXML2
{

//_______________________________________________________________________________
/*
  A type to pass options and their values if any to the functions in the interfaces

  In the pair, first is the name of the option, such as '-x',
  and second is the value of the option
*/

typedef std::vector<std::pair<std::string, std::string> > optionsVector;


}


#endif
