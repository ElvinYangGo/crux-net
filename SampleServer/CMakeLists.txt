cmake_minimum_required (VERSION 2.6)

set( SampleServerSource 
	ServerMsgHandler.cpp
	)

if( UNIX )
	set( SampleServerSource ${SampleServerSource}
		Linux/main.cpp
		)
	add_definitions( -g -Wall -ansi -DLINUX )
elseif( WIN32 )
	set( SampleServerSource ${SampleServerSource}
		Win/main.cpp
		)
	add_definitions( -DWIN32_LEAN_AND_MEAN )
endif()

include_directories( "${PROJECT_SOURCE_DIR}/include" )
link_directories( "${PROJECT_SOURCE_DIR}/lib" )

add_executable( SampleServer ${SampleServerSource} )
target_link_libraries( SampleServer net )

set_target_properties( SampleServer PROPERTIES
	DEBUG_POSTFIX "d"
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Server
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Server
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Server
	)

if( WIN32 )
	target_link_libraries( SampleServer ws2_32 mswsock )
endif()

