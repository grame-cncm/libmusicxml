
# Cd to where you want the git folder to be located

  cd <somePath>
  
# Get the libmusixcml2/xml2lilypond code:

  git clone https://github.com/dfober/libmusicxml.git libmusicxml-git
  cd libmusicxml-git

# Check-out the desired branch

  BRANCH=lilypond
  git checkout ${BRANCH}


# Build everyting with either:

  cd cmake
  cmake .
  make

# or:

  ./Build_libmusicxml.bash cmake

# (some warning are issued for the preliminary versions )


# Check that everything works fine

  ls -sal

  ./xml2lilypond -hs
