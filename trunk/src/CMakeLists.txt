cmake_minimum_required (VERSION 2.6)

set( HeaderPath "${PROJECT_SOURCE_DIR}/include" )
message( STATUS "Header path: ${HeaderPath}" )

set( NetHeaders
	${HeaderPath}/AcceptType.h
	${HeaderPath}/Allocator.h
	${HeaderPath}/Buffer.h
	${HeaderPath}/Config.h
	${HeaderPath}/ConfigReader.h
	${HeaderPath}/ConstConfigData.h
	${HeaderPath}/Dispatcher.h
	${HeaderPath}/HelperFunction.h
	${HeaderPath}/Logger.h
	${HeaderPath}/Msg.h
	${HeaderPath}/MsgHandler.h
	${HeaderPath}/MsgPool.h
	${HeaderPath}/MsgQueue.h
	${HeaderPath}/Mutex.h
	${HeaderPath}/MutexGuard.h
	${HeaderPath}/NetClient.h
	${HeaderPath}/NetServer.h
	${HeaderPath}/Queue.h
	${HeaderPath}/Semaph.h
	${HeaderPath}/Session.h
	${HeaderPath}/SessionMgr.h
	${HeaderPath}/SessionPool.h
	${HeaderPath}/Singleton.h
	${HeaderPath}/Stack.h
	${HeaderPath}/SystemAllocator.h
	${HeaderPath}/TCPClient.h
	${HeaderPath}/TCPServer.h
	${HeaderPath}/TCPStream.h
	${HeaderPath}/Thread.h
	${HeaderPath}/UniSizeAllocator.h
	${HeaderPath}/WorkerThread.h
	)

set( NetSources
	Allocator.cpp
   	Buffer.cpp 
	Config.cpp 
	ConfigReader.cpp 
	Logger.cpp 
	MsgQueue.cpp 
	NetClient.cpp 
	NetServer.cpp 
	Session.cpp 
	SessionMgr.cpp 
	SystemAllocator.cpp 
	TCPClient.cpp 
	TCPServer.cpp 
	TCPStream.cpp 
	UniSizeAllocator.cpp 
	)

if( UNIX )
	set( NetHeaders ${NetHeaders}
		${HeaderPath}/Linux/AcceptCompleteOperation.h
		${HeaderPath}/Linux/AsyncCompleteOperation.h
		${HeaderPath}/Linux/AsyncOperationQueue.h
		${HeaderPath}/Linux/AsyncOperationType.h
		${HeaderPath}/Linux/Dispatcher.h
		${HeaderPath}/Linux/Mutex.h
		${HeaderPath}/Linux/Semaph.h
		${HeaderPath}/Linux/SessionCompleteOperation.h
		${HeaderPath}/Linux/Thread.h
		${HeaderPath}/Linux/WorkerThread.h
		)
		
	set( NetSources ${NetSources}
		Linux/AcceptCompleteOperation.cpp
		Linux/AsyncOperationQueue.cpp
		Linux/Dispatcher.cpp
		Linux/Mutex.cpp
		Linux/Semaph.cpp
		Linux/SessionCompleteOperation.cpp
		Linux/Thread.cpp
		Linux/WorkerThread.cpp
		)
		
	add_definitions( -Wall -ansi -DLINUX )
elseif( WIN32 )
	set( NetHeaders ${NetHeaders}
		${HeaderPath}/Win/AcceptHandler.h
		${HeaderPath}/Win/AsyncCompleteOperation.h
		${HeaderPath}/Win/CloseHandler.h
		${HeaderPath}/Win/CompleteHandler.h
		${HeaderPath}/Win/Dispatcher.h
		${HeaderPath}/Win/Mutex.h
		${HeaderPath}/Win/RecvHandler.h
		${HeaderPath}/Win/Semaph.h
		${HeaderPath}/Win/SendHandler.h
		${HeaderPath}/Win/Thread.h
		${HeaderPath}/Win/WorkerThread.h
		)

	set( NetSources ${NetSources}
		Win/AcceptHandler.cpp
		Win/AsyncCompleteOperation.cpp
		Win/CloseHandler.cpp
		Win/Dispatcher.cpp
		Win/Mutex.cpp
		Win/RecvHandler.cpp
		Win/Semaph.cpp
		Win/SendHandler.cpp
		Win/Thread.cpp
		Win/WorkerThread.cpp
		)
		
	add_definitions( -DWIN32_LEAN_AND_MEAN -DUNICODE -D_UNICODE)
endif()

include_directories( "${PROJECT_SOURCE_DIR}/include" )
set( NetAllSources ${NetHeaders} ${NetSources} )

find_package( Threads )
message(STATUS "Threads: ${CMAKE_THREAD_LIBS_INIT}")

add_library( net STATIC ${NetAllSources} )
target_link_libraries( net ${CMAKE_THREAD_LIBS_INIT} )

set_target_properties( net PROPERTIES
	DEBUG_POSTFIX "d"
	ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/lib
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib
	)

message( STATUS "build type: ${CMAKE_BUILD_TYPE}" )
