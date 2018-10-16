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
set (VERSION 3.1.2)
set (STRVERS "v.3.12")
set (SSTRVERS "3.12")
set (SOVERS 3)


#######################################
option ( FMWK 	"Generates a framework on MacOS by default"	true )


#######################################
set (CMAKE_CXX_STANDARD 11)


if(UNIX)
	add_definitions(-Wall -DGCC -DTRACE_OPTIONS -O3 -Wno-overloaded-virtual)
endif()

set (libtype SHARED)

#########################################
# mac os specific part
#########################################
if(APPLE)
	if ( IOS )
		message (STATUS "Generates project for iOS - Use -DIOS=no to change.")		
		set (CMAKE_XCODE_EFFECTIVE_PLATFORMS "iPhoneOS")
		set (CMAKE_OSX_ARCHITECTURES "arm64") # unsupported on iOS 11.4.0: armv7 armv7s
		set (CMAKE_OSX_SYSROOT "iphoneos")
		set (libtype STATIC)
	endif()

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
set (LXML 		${CMAKE_CURRENT_SOURCE_DIR}/..)
set (LXMLSRC 	${LXML}/src)
set (LXMLSAMPLE ${LXML}/samples)
set (SRCFOLDERS  factory files interface elements guido lib operations parser visitors lilypond)

foreach(folder ${SRCFOLDERS})
	set(SRC ${SRC} "${LXMLSRC}/${folder}/*.cpp")			# add source files
endforeach(folder)
file (GLOB CORESRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SRC})

foreach(folder ${SRCFOLDERS})
	set(HEADERS ${HEADERS} "${LXMLSRC}/${folder}/*.h")		# add header files
endforeach(folder)
file (GLOB COREH RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${HEADERS})

foreach(folder ${SRCFOLDERS})
	set(INCL ${INCL} "${LXMLSRC}/${folder}")				# add include folders
endforeach(folder)


#######################################
# set includes
include_directories( ${INCL})
set_source_files_properties (${COREH} PROPERTIES HEADER_FILE_ONLY TRUE)


#######################################
# set library target
# if(APPLE)
# 	if (IOS)
# 		set(target musicxml2)
# 	else()
# 		set(target libmusicxml2)
# 	endif()
# elseif (MSVC)
# 	set(target libmusicxml2)
# else()
# 	set(target musicxml2)
# endif()

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
			PUBLIC_HEADER "${COREH}"
			DEFINE_SYMBOL LIBMUSICXML_EXPORTS
			XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
)

#set_target_properties( ${target}  PROPERTIES OUTPUT_NAME libmusicxml2)


if (FMWK AND APPLE AND NOT IOS)
message ("set FMWK - ${FMWK}") 
set_target_properties (${target} PROPERTIES 
			FRAMEWORK ${FMWK}
			FRAMEWORK_VERSION ${VERSION}
			MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${STRVERS}
			MACOSX_FRAMEWORK_BUNDLE_VERSION ${SSTRVERS}
			MACOSX_FRAMEWORK_IDENTIFIER ${target}
)
endif()

#######################################
# set sample targets
set (SAMPLES xmlversion RandomMusic xmlfactory partsummary xml2guido xml2ly xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)
set (TOOLS  xml2guido xml2ly xmlread xmltranspose)

if(NOT IOS )
foreach(sample ${SAMPLES})
	add_executable( ${sample} ${LXMLSAMPLE}/${sample}.cpp )
	target_link_libraries( ${sample} ${target})
	add_dependencies(${sample} ${target})
endforeach(sample)
endif()


#######################################
# install setup
if (NOT IOS)
install ( TARGETS ${target} ${TOOLS} 
	RUNTIME DESTINATION bin 
    	LIBRARY DESTINATION /usr/lib 
    	ARCHIVE DESTINATION lib 
    	FRAMEWORK DESTINATION "/Library/Frameworks" CONFIGURATIONS Release
    	PUBLIC_HEADER DESTINATION include/libmusicxml
)
endif()
