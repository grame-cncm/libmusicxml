#!/bin/bash
#

function kname() {
  echo k_$1 | tr '-' '_'
}

case $3 in
  'constants')
    # copy license and and the factory preamble
    # to standard output

    cat $2/license.txt $2/elements.txt
    for a in $(cat $1 | cut -d' ' -f1 | sort -u )
    do
      echo "  $(kname $a),"
    done

    # generate epiloque
    cat << ! 
  kEndElement
};

}
#endif

!
    ;;


  'map')
    # copy license and and the factory class preamble
    # to standard output
    cat $2/license.txt $2/factory.txt

    # generate fMap entries for the markups found in the DTD
   for a in $(cat $1 | cut -d' ' -f1 | sort -u)
    do
      echo "  fMap[\"$a\"]  = new newElementFunctor<$(kname $a)>;"
    done
    echo

    # generate fType2Name entries for the markups found in the DTD
    for a in $(cat $1 | cut -d' ' -f1 | sort -u)
    do
      echo "  fType2Name[$(kname $a)]   = \"$a\";"
    done

    # generate epiloque
    echo '}'
    echo '}'
    ;;


  'typemap') #  UNUSED JMI ???
    # copy license and and the fMap preamble
    # to standard output
    cat $2/license.txt $2/typeMap.txt

    # generate fMap entries for the markups found in the DTD
    for a in $(cat $1 | cut -d' ' -f1 | sort -u)
    do
      echo "   fMap[$(kname $a)]  = \"$a\";"
    done

    # generate epiloque
    echo "}"
    echo "}"
    ;;


  'types')
    # copy license and typedefs for S_comment and S_processing_instruction
    # to standard output
    cat $2/license.txt $2/typedefs.txt

    # generate typedefs for the markups found in the DTD
    for a in $(cat $1 | cut -d' ' -f1 | sort -u)
    do
      echo "typedef SMARTP<musicxml<$(kname $a), \"$a\"> >    S_$(echo $a | tr '-' '_');"
    done

    # generate epiloque
    echo '}'
    echo '#endif'
    ;;


  *)
    echo "usage: elements.bash eltsfile templatedir [constants | map | types]"
    exit 1
    ;;
esac  

exit 0

