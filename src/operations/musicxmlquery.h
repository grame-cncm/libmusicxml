//
//  musicxmlquery.hpp
//  GuidoKit
//
//  Created by Arshia Cont on 23/12/16.
//  Copyright © 2016 Antescofo SAS. All rights reserved.
//

#ifndef musicxmlquery_hpp
#define musicxmlquery_hpp

#include <stdio.h>
#include <map>
#include <vector>

#include "exports.h"
#include "typedefs.h"
#include "notevisitor.h"
#include "transposevisitor.h"


namespace MusicXML2
{
    
    /*!
     \addtogroup visitors
  @{
     */
    
    /*!
     \brief A general musicXML visitor to access properties through a class.
     
     An extendible class that allows querying and fetching desired elements in MusicXML
     
     \todo transposing ornaments elements
     */
    class EXP musicxmlQuery :
    public notevisitor,
    public transposevisitor,
    public visitor<S_rehearsal>,    // for rehearsal marks
    public visitor<S_part>
    {
    public:
        musicxmlQuery();
        virtual ~musicxmlQuery();
        
        void init();
        
        /// Query methods
        
        /*! Provides Instrument Transposer of the score in Chromatic Scale
         
            The transpose element represents what must be added to the written pitch to get the correct sounding pitch.
         
            See (MusicXML Attribute Description)[http://www.musicxml.com/tutorial/the-midi-compatible-part/attributes/]
         
         \return steps as Int in chromatic steps count
         */
        int  getTransposeInstrumentChromatic ()	;        
        std::string  getTransposeInstrumentName ();

        
    protected:
        
        float beatCum;
        
//        virtual void visitStart ( S_note& elt );
//        virtual void visitStart ( S_rehearsal& elt );

        virtual void visitEnd ( S_note& elt );
        virtual void visitEnd ( S_rehearsal& elt );

    };
    
    
}


#endif /* musicxmlquery_hpp */
