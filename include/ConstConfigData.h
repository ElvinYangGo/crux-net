#ifndef CONST_CONFIG_DATA_H
#define CONST_CONFIG_DATA_H

#include <cstring> //for size_t

namespace Crux
{

#define SERVER

//--------------------------------------------------------
//the followings are sample config data for server

#ifdef SERVER
/**
 * config data for BUFFER
 * change these value to configure Session
 */
const size_t config_BUFFER_SIZE = 1024;


/**
 * config data for Allocator
 * change these value to configure Allocator
 */
const size_t config_ALIGN = 8;
const size_t config_BYTES_MAX = 128;
const int config_FREE_LIST_ELEMENT_COUNT = 1024;


/**
 * config data for MsgPool
 * change these value to configure MsgPool
 * must larger than client session amount
 */
const int config_MSG_COUNT_MAX = 4096;


/**
 * config data for MsgQueue
 * change these value to configure MsgQueue
 * must larger than client session amount
 */
const size_t cofig_MSG_QUEUE_SIZE = 4096;

/**
 * config data for AsyncOperationQueue
 * must larger than client session amount
 */
const size_t cofig_ASYNC_OPERATION_QUEUE_SIZE = 4096;

#elif defined(CLIENT)

//--------------------------------------------------------
//the followings are sample config data for client

/**
 * config data for BUFFER
 * change these value to configure Session
 */
const size_t config_BUFFER_SIZE = 1024;


/**
 * config data for Allocator
 * change these value to configure Allocator
 */
const size_t config_ALIGN = 8;
const size_t config_BYTES_MAX = 128;
const int config_FREE_LIST_ELEMENT_COUNT = 8;


/**
 * config data for MsgPool
 * change these value to configure MsgPool
 * must larger than client session amount
 */
const int config_MSG_COUNT_MAX = 32;


/**
 * config data for MsgQueue
 * change these value to configure MsgQueue
 * must larger than client session amount
 */
const size_t cofig_MSG_QUEUE_SIZE = 32;

/**
 * config data for AsyncOperationQueue
 * must larger than client session amount
 */
const size_t cofig_ASYNC_OPERATION_QUEUE_SIZE = 32;

#endif

} //namespace Crux

#endif //CONST_CONFIG_DATA_H
