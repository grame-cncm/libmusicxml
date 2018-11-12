#/bin/bash

ALL_ARGUMENTS=$*
#echo "ALL_ARGUMENTS = ${ALL_ARGUMENTS}"

RESULT=""

for FILE in ${ALL_ARGUMENTS}; do
#  echo "--> ${FILE}"
  
  echo ${FILE} | grep "braille" > /dev/null
  RETURN_VALUE=$?
  
  if [ $RETURN_VALUE -eq 0 ]; then
    RESULT="${RESULT} ${FILE}"
  fi
done

echo ${RESULT}
