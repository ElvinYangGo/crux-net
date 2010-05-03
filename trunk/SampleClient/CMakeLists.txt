cmake_minimum_required (VERSION 2.6)

set( SampleClientSource
	ClientMsgHandler.cpp
	)

if( UNIX )
	set( SampleClientSource ${SampleClientSource}
		Linux/main.cpp
		)
	add_definitions( -Wall -ansi -DLINUX )
elseif( WIN32 )
	set( SampleClientSource ${SampleClientSource}
		Win/main.cpp
		)
	add_definitions( -DWIN32_LEAN_AND_MEAN )
endif()

include_directories( "${PROJECT_SOURCE_DIR}/include" )
link_directories( "${PROJECT_SOURCE_DIR}/lib" )

add_executable( SampleClient ${SampleClientSource} )
target_link_libraries( SampleClient net )

set_target_properties( SampleClient PROPERTIES
	DEBUG_POSTFIX "d"
	RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Client
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin/Client
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/Client
	)

if( WIN32 )
	target_link_libraries( SampleClient ws2_32 mswsock )
endif()

