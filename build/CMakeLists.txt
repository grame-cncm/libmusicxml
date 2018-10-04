#######################################
# CMAKE libmusicxml2
#######################################
project(libmusicxml2)
cmake_minimum_required(VERSION 2.4)
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Debug Release)
endif()

#######################################
# mimimal tools versions check
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 4.9.2
#    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9.2)
#        message(FATAL_ERROR "GCC version must be at least 4.9.2 to build libmusicxml2!")
#    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 3.3
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
        message(FATAL_ERROR "Clang version must be at least 3.3 to build libmusicxml2!")
    endif()
else()
    message(WARNING "Building libmusicxml2 has only been tested with Clang and GCC.")
endif()

###try_compile(HAVE_PLWID ${CMAKE_BINARY_DIR} cmake/compilerVersionCheck.cpp
###    CMAKE_FLAGS -Wall -DGCC -std=c++11 -ggdb3)

#######################################
# versions management
set (VERSION 3.11)
set (STRVERS "v.3.11")
set (SSTRVERS "3.11")

#######################################
set (CMAKE_CXX_STANDARD 11)

if(UNIX)
	add_definitions(-Wall -DGCC -Wno-overloaded-virtual)
endif()

set (FMWK "true")
set (libtype SHARED)

#########################################
# mac os specific part
#########################################
if(APPLE)
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
#		set (CMAKE_OSX_ARCHITECTURES "armv6 armv7")
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (libtype STATIC)
	endif()

#########################################
# windows specific part
#########################################
elseif(WIN32)
 add_definitions(-DWINVER=0x0400 -DWIN32)
 if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")
  add_definitions(-DVC6)
 elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 8 2005")
  add_definitions(-DVC2005)
 endif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 6")

#########################################
# assume linux host
#########################################
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()


#######################################
# set directories, src and headers.
set (LXML        ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (LXMLSRC     ${LXML}/src)
set (LXMLSAMPLES ${LXML}/samples)
set (SRCFOLDERS  factory files interface elements guido lilypond lib operations parser visitors)

foreach(folder ${SRCFOLDERS})
  set(SRC ${SRC} "${LXMLSRC}/${folder}/*.cpp")       # add source files
endforeach(folder)
file (GLOB CORESRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})

foreach(folder ${SRCFOLDERS})
  set(HEADERS ${HEADERS} "${LXMLSRC}/${folder}/*.h") # add header files
endforeach(folder)
file (GLOB COREH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

foreach(folder ${SRCFOLDERS})
  set(INCL ${INCL} "${LXMLSRC}/${folder}")           # add include folders
endforeach(folder)

if(WIN32)
  set(SRC ${SRC} "${LXML}/win32/iconv/win_iconv.c")       # add source files
  set(INCL ${INCL} "${LXML}/win32/iconv")           # add include folders
endif()


#######################################
# set includes
include_directories( ${INCL})
set_source_files_properties (${COREH} PROPERTIES HEADER_FILE_ONLY TRUE)


#######################################
# set library target
if(APPLE)
  if (IOS STREQUAL "yes")
    set(target musicxml2)
  else()
    set(target libmusicxml2)
  endif()
	if (IOS)
		set(target musicxml2)
	else()
		set(target libmusicxml2)
	endif()
elseif(MSVC)
  set(target libmusicxml2)
else()
  set(target musicxml2)
endif()

if(MSVC)
  enable_language(RC)
  set(LIBCONTENT ${CORESRC} ${COREH} ${LXML}/win32/libmusicxml/libmusicxml.rc)
else()
  set(LIBCONTENT ${CORESRC} ${COREH})
endif()

add_library(${target} ${libtype} ${LIBCONTENT})
set_target_properties (${target} PROPERTIES 
      VERSION ${VERSION}
      FRAMEWORK_VERSION ${VERSION}
      PUBLIC_HEADER "${COREH}"
      DEFINE_SYMBOL LIBMUSICXML_EXPORTS
      MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
      MACOSX_FRAMEWORK_BUNDLE_VERSION ${SSTRVERS}
)
if ( IOS STREQUAL "no" )
  set_target_properties (${target} PROPERTIES  FRAMEWORK TRUE )
endif()

#######################################
# set sample targets
#set (SAMPLES RandomMusic xmlfactory partsummary xml2guido xml2ly msrStreamsExample xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)
set (SAMPLES RandomMusic xmlfactory partsummary xml2guido xml2ly xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)

if(NOT APPLE OR NOT IOS )
foreach(sample ${SAMPLES})
  add_executable( ${sample} ${LXMLSAMPLES}/${sample}.cpp )
  target_link_libraries( ${sample} ${target})
  add_dependencies(${sample} ${target})
endforeach(sample)
endif()


#######################################
# install setup
if(NOT APPLE)
install ( TARGETS ${target} ${TOOLS} 
	RUNTIME DESTINATION bin 
    	LIBRARY DESTINATION /usr/lib 
    	ARCHIVE DESTINATION lib 
#    	FRAMEWORK DESTINATION "/Library/Frameworks" CONFIGURATIONS Release
    	PUBLIC_HEADER DESTINATION include/libmusicxml
)
endif()
