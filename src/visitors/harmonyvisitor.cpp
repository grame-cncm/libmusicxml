/*
 MusicXML Library
 Copyright (C) Grame 2006-2013

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.

 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 */

#include "harmonyvisitor.h"

using namespace std;

namespace MusicXML2 {

    //________________________________________________________________________
    // transpose element suppport
    //________________________________________________________________________
    void harmonyvisitor::reset ()
    {
        fText.clear();
        fRoot.clear();
        fAlteration.clear();
    }

    void harmonyvisitor::visitStart ( S_harmony& elt )
    {
        reset();
    }

    void harmonyvisitor::visitStart ( S_root& elt )		{
         reset();
    }
    void harmonyvisitor::visitStart ( S_root_alter& elt ) {
        int accs = (int)(*elt);
        for(int i = accs; i > 0; i--) {
            fAlteration += "#";
        }
        for(int i = accs; i < 0; i++) {
            fAlteration += "b";
        }
    }

    void harmonyvisitor::visitStart ( S_kind& elt )		{
        fText = elt->getAttributeValue ("text");
    }
    
    void harmonyvisitor::visitStart ( S_root_step& elt ) {
        fRoot = elt->getValue();
    }
    
}
