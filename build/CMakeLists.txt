#######################################
# CMAKE libmusicxml2
#######################################
project(libmusicxml2)
cmake_minimum_required(VERSION 2.4)
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()

#######################################
# versions management
set (XMLVERSION 3.1)

set (VERSION 3.1.7)
macro (get_major_minor_patch version)
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\1"  VERSION_MAJOR ${version} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\2"  VERSION_MINOR ${version} )
	string( REGEX REPLACE "([0-9]*)\\.([0-9]*)\\.([0-9]*)" "\\3"  VERSION_PATCH ${version} )
endmacro()
get_major_minor_patch( ${VERSION} )
set (SOVERS ${VERSION_MAJOR})
set (STRVERS "v.3.17")
set (SSTRVERS "3.17")
message (STATUS "Configuring version ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")



#######################################
option ( FMWK 		"Generates a framework on MacOS by default"	on )
option ( UNIVERSAL 	"Generates universal binaries"	off )
option ( GDB 		"Activates ggdb3 option"	off )
option ( LILY 		"Include lilypond part"	on )

#######################################
get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set (DOC    ${ROOT}/doc/presentation/libmusicxml2.pdf)
set (DTDS   ${ROOT}/dtds/${XMLVERSION})
set (SCHEMA ${ROOT}/schema/${XMLVERSION})

set (BINDIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set (LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)


#######################################
set (CMAKE_CXX_STANDARD 11)

if(UNIX)
  if (GDB)
    add_definitions(-Wall -DGCC -ggdb3 -Wno-overloaded-virtual)
  else()
    add_definitions(-Wall -DGCC -O3 -Wno-overloaded-virtual)
  endif()
endif()

set (libtype SHARED)

#########################################
# mac os specific part
#########################################
if(APPLE)
  if ( IOS )
    message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
    set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
    set (CMAKE_OSX_ARCHITECTURES "arm64  armv7 armv7s") # unsupported on iOS 11.4.0: armv7 armv7s
    set (CMAKE_OSX_SYSROOT "iphoneos")
    set (CMAKE_CXX_FLAGS "-miphoneos-version-min=9.0")
    set (libtype STATIC)
  elseif( UNIVERSAL )
    message (STATUS "Generates universal binaries - Use -DUNIVERSAL=no to change. .")
    set (CMAKE_OSX_ARCHITECTURES "x86_64 i386")
    set (CMAKE_OSX_DEPLOYMENT_TARGET 10.9)
  endif()
  # to debug:
  add_definitions(-O0 -g) # JMI
  # for speed
#  add_definitions(-O3) # JMI

#########################################
# windows specific part
#########################################
elseif(WIN32)
  add_definitions(-DWINVER=0x0400 -DWIN32)
  if(MSVC)
    add_definitions(-DMSVC)
  endif()
endif()


#######################################
# set directories, src and headers.
set (LXML     ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (LXMLSRC  ${LXML}/src)
set (LXMLSAMPLE ${LXML}/samples)
set (SRCFOLDERS  factory files elements guido lib operations parser visitors)
if (LILY)
	set (SRCFOLDERS  ${SRCFOLDERS}  interface lilypond lilypond/msr lilypond/lpsr lilypond/bsr)
endif()

foreach(folder ${SRCFOLDERS})
  set(SRC ${SRC} "${LXMLSRC}/${folder}/*.cpp")      # add source files
endforeach(folder)

foreach(folder ${SRCFOLDERS})
  set(HEADERS ${HEADERS} "${LXMLSRC}/${folder}/*.h")    # add header files
endforeach(folder)

if (NOT LILY)
	message (STATUS "Lilypond part is not included, use -DLILY=yes to change")		
	set(SRC ${SRC}  "${LXMLSRC}/interface/libmusicxml.cpp" "${LXMLSRC}/interface/musicxml2guido.cpp")
	set(HEADERS $HEADERS "${LXMLSRC}/interface/libmusicxml.h" "${LXMLSRC}/interface/musicxml2guido.h")
	set (SRCFOLDERS  ${SRCFOLDERS}  interface)
endif()
file (GLOB CORESRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})
file (GLOB COREH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

foreach(folder ${SRCFOLDERS})
  set(INCL ${INCL} "${LXMLSRC}/${folder}")        # add include folders
endforeach(folder)


#######################################
# set includes
include_directories( ${INCL})
set_source_files_properties (${COREH} PROPERTIES HEADER_FILE_ONLY TRUE)


#######################################
if(WIN32 OR MSYS)
  enable_language(RC)
  set(LIBCONTENT ${CORESRC} ${COREH} ${LXML}/win32/libmusicxml/libmusicxml.rc)
else()
  set(LIBCONTENT ${CORESRC} ${COREH})
endif()

set(target musicxml2)
add_library(${target} ${libtype} ${LIBCONTENT})
set_target_properties (${target} PROPERTIES
			VERSION ${VERSION}
			SOVERSION ${SOVERS}
			ARCHIVE_OUTPUT_DIRECTORY  ${LIBDIR}
			ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY ${LIBDIR}
			LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIBDIR}
			PUBLIC_HEADER "${COREH}"
			DEFINE_SYMBOL LIBMUSICXML_EXPORTS
			XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
)
if (MSVC)
  set_target_properties( ${target}  PROPERTIES OUTPUT_NAME libmusicxml2)
endif()

if (FMWK AND APPLE AND NOT IOS)
set_target_properties (${target} PROPERTIES
      OUTPUT_NAME libmusicxml2
      FRAMEWORK ${FMWK}
      FRAMEWORK_VERSION ${VERSION}
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
      MACOSX_FRAMEWORK_BUNDLE_VERSION ${SSTRVERS}
      MACOSX_FRAMEWORK_IDENTIFIER ${target}
)
endif()

#######################################
# set sample targets
set (SAMPLES xmlversion RandomMusic RandomChords xmlfactory partsummary xml2guido xml2ly xml2brl xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)
set (TOOLS  xml2guido xml2ly xml2brl xmlread xmltranspose xmlversion)
if (LILY)
	set (SAMPLES ${SAMPLES} xml2ly)
	set (TOOLS  ${TOOLS} xml2ly)
endif()

if(NOT IOS )
foreach(sample ${SAMPLES})
	add_executable( ${sample} ${LXMLSAMPLE}/${sample}.cpp )
	target_link_libraries( ${sample} ${target})
	set_target_properties (${sample} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY  ${BINDIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${BINDIR})
	add_dependencies(${sample} ${target})
endforeach(sample)
endif()


#######################################
# install setup
if (NOT IOS)
	message (STATUS "Install location is ${CMAKE_INSTALL_PREFIX}")		

if (APPLE)
install ( TARGETS ${target} 
	RUNTIME DESTINATION bin 
    LIBRARY DESTINATION bin 
    ARCHIVE DESTINATION bin 
    FRAMEWORK DESTINATION lib CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION include/libmusicxml
)
else ()
install ( TARGETS ${target} 
	RUNTIME DESTINATION lib 
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib 
    FRAMEWORK DESTINATION lib CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION include/libmusicxml
)
endif()

install ( TARGETS ${TOOLS}
  RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)


####################################
# install misc files
set (DOCDIR  ${CMAKE_INSTALL_PREFIX}/share/libmusicxml)
install (
    DIRECTORY ${DTDS} DESTINATION ${DOCDIR}/dtds
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN ".DS_Store" EXCLUDE
)

install (
    DIRECTORY ${DTDS} ${SCHEMA} DESTINATION ${DOCDIR}/schema
    FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN ".DS_Store" EXCLUDE
)

install (
    FILES ${DOC} DESTINATION ${DOCDIR}/doc
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

install (
    FILES ${ROOT}/CHANGELOG.txt  DESTINATION ${DOCDIR}
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)


####################################
# cpack section
####################################
set (CPACKDEST libMusicXML-${VERSION})
set (CPACK_PACKAGE_NAME   "libMusicXML")
set (CPACK_PACKAGE_VENDOR "GRAME")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "libMusicXML - a library and tools to support the MusicXML format.")
set (CPACK_PACKAGE_VERSION   ${VERSION})
set (CPACK_PACKAGE_VERSION_MAJOR   ${VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR   ${VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH   ${VERSION_PATCH})

set (CPACK_MONOLITHIC_INSTALL true)
set (CPACK_RESOURCE_FILE_LICENSE  ${ROOT}/license.txt)
set (CPACK_PACKAGE_INSTALL_DIRECTORY  ${CPACKDEST})

####################################
# NSIS specific section
set (CPACK_NSIS_INSTALL_ROOT "C:\\\\Program Files")
set (CPACK_NSIS_MODIFY_PATH on)
#set (CPACK_NSIS_MUI_ICON "${ROOT}/Resources/Images/FaustLiveIcon.ico")

include (CPack)


endif()
