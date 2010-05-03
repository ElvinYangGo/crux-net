#ifndef ACCEPT_TYPE_H
#define ACCEPT_TYPE_H

/** @file */

/**
 * Server's acceptor type.
 */

enum AcceptType
{
	ACCEPT_EXTERNAL = 1, /*!< accept an external connection, ie a client side connection. */
	ACCEPT_INTERNAL, /*!< accept an internal connection, ie other server's connection. */
};

#endif	//ACCEPT_TYPE_H

