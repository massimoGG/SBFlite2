/**
 * @file network.c
 * @brief everything related about networking sockets
 */

#include "network.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief creates and returns a prepared network handle
 * @param[in] timeoutMs
 * @retval NULL if we couldn't allocate memory for the handle or socket() failed
 * @retval networkHandle_t
 */
networkHandle_t* network_init(uint32_t timeoutMs)
{
    networkHandle_t* handle = (networkHandle_t*)malloc(sizeof(networkHandle_t));
    if (NULL == handle) {
        return NULL;
    }

    handle->timeout = timeoutMs;

    /* Create socket for IPv4 TCP */
    handle->sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == handle->sockfd) {
        return NULL;
    }

    /* Set socket option TCP_NODELAY so there's no caching */
    int flag = 1;
    setsockopt(handle->sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));

    return handle;
}

/**
 * @brief attemps to connect to the remote
 * @param[in] pHandle
 * @param[in] ip
 * @param[in] port
 * @retval eError_ok        successfully connected
 * @retval eError_failed    couldn't connect
 */
error_e network_connect(networkHandle_t* pHandle, const char* ip, uint16_t port)
{
    /* Convert IP string to correct format */
    struct sockaddr_in sockaddr;
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);

    if (connect(pHandle->sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr))) {
        return eError_failed;
    }
    return eError_ok;
}

/**
 * @brief closes the socket connection
 * @param[in] pHandle
 * @return error_e
 */
error_e network_close(networkHandle_t* pHandle)
{
    if (pHandle > 0) {
        close(pHandle->sockfd);
    }
    return eError_ok;
}

/**
 * @brief writes \p pData of length \p size to the socket
 * @param[in] pHandle
 * @param[in] pData
 * @param[in] size
 * @return error_e
 */
error_e network_write(networkHandle_t* pHandle,
    const uint8_t* pData,
    uint16_t size)
{
    if (send(pHandle->sockfd, pData, size, 0) != size) {
        return eError_failed;
    }

    return eError_ok;
}

/**
 * @brief reads from socket to \p pData and updates \p pSize with the actual bytes read
 * @param[in] pHandle
 * @param[out] pData
 * @param[in,out] pSize
 * @return error_e
 */
error_e network_read(networkHandle_t* pHandle, uint8_t* pData, uint16_t* pSize)
{
    assert(*pSize != 0);

    /* Prepare poll struct */
    struct pollfd fds = (struct pollfd) {
        .fd = pHandle->sockfd,
        /* PollIn = Read event*/
        .events = POLLIN,
    };

    const int sockfdsReady = poll(&fds, 1, pHandle->timeout);
    if (sockfdsReady == -1) {
        /* Error */
        return eError_failed;
    } else if (sockfdsReady == 0) {
        /* Timeout*/
        return eError_timeout;
    }

    if ((fds.revents | (POLLPRI | POLLERR | POLLHUP)) != 0) {
        /* Error with socket*/
        return eError_failed;
    }

    if (fds.revents != POLLIN) {
        /* Expecting POLLIN at this point */
        return eError_failed;
    }

    /* Actually read now from kernel */
    *pSize = recv(pHandle->sockfd, pData, *pSize, 0);
    if (*pSize < 1) { /* -1 = error, 0 = connection closed */
        return eError_failed;
    }

    return eError_ok;
}

error_e network_getState(networkHandle_t* pHandle)
{
    return eError_ok;
}

error_e network_deinit(networkHandle_t* pHandle)
{
    if (pHandle != NULL) {
        free(pHandle);
    }
    return eError_ok;
}
