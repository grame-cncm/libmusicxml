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
set (VERSION 3.1.4)
set (STRVERS "v.3.14")
set (SSTRVERS "3.14")
set (SOVERS 3)
set (XMLVERSION 3.1)


#######################################
option ( FMWK 		"Generates a framework on MacOS by default"	on )
option ( UNIVERSAL 	"Generates universal binaries"	off )
option ( GDB 		"Activates ggdb3 option"	off )

#######################################
get_filename_component(ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
set (DOC    ${ROOT}/doc/presentation/libmusicxml2.pdf)
set (DTDS   ${ROOT}/dtds/${XMLVERSION})
set (SCHEMA ${ROOT}/schema/${XMLVERSION})

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
set (SAMPLES xmlversion RandomMusic xmlfactory partsummary xml2guido xml2ly xmliter countnotes readunrolled xml2midi xmlread xmltranspose xmlclone)
set (TOOLS  xml2guido xml2ly xmlread xmltranspose xmlversion)

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
message (STATUS "Install location is ${CMAKE_INSTALL_PREFIX}")		
install ( TARGETS ${target} 
	RUNTIME DESTINATION lib 
    LIBRARY DESTINATION lib 
    ARCHIVE DESTINATION lib 
    FRAMEWORK DESTINATION lib CONFIGURATIONS Release
    PUBLIC_HEADER DESTINATION include/libmusicxml
)

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

endif()
