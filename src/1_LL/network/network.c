/**
 * @file network.c
 * @brief everything related about networking sockets
 */

#include "network.h"
#include <arpa/inet.h>
#include <asm/termbits.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

static error_e network_poll(networkHandle_t* pHandle);

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
 * @brief polls the socket for reading
 * @param pHandle
 * @retval eError_ok        socket is ready to read
 * @retval eError_failed    if there's a general socket failure
 * @retval eError_timeout   if a timeout occurred
 */
static error_e network_poll(networkHandle_t* pHandle)
{
    /* Prepare poll struct */
    struct pollfd fds = (struct pollfd) {
        .fd = pHandle->sockfd,
        /* PollIn = Read event*/
        .events = POLLIN,
        .revents = 0,
    };

    const int sockfdsReady = poll(&fds, 1, pHandle->timeout);
    if (sockfdsReady == -1) {
        /* Error */
        return eError_failed;
    } else if (sockfdsReady == 0) {
        /* Timeout*/
        return eError_timeout;
    }

    if ((fds.revents & (POLLPRI | POLLERR | POLLHUP)) != 0) {
        /* Error with socket*/
        return eError_failed;
    }

    if (fds.revents != POLLIN) {
        /* Expecting POLLIN at this point */
        return eError_failed;
    }

    return eError_ok;
}

/**
 * @brief reads \p pSize bytes from socket to \p pData and updates \p pSize with the actual bytes read
 * @param[in] pHandle
 * @param[out] pData
 * @param[in,out] pSize in: size of \p pData buffer; out: total size read into buffer
 * @retval eError_ok        if the expected number of bytes were received
 * @retval eError_timeout   if a timeout occurred
 */
error_e network_readUntil(networkHandle_t* pHandle, uint8_t* pData, uint16_t* pSize)
{
    assert(*pSize != 0);

    uint16_t totalRead = 0;

    /* Keep reading until received *pSize bytes */
    while (totalRead < *pSize) {
        const error_e ret = network_poll(pHandle);
        if (eError_ok != ret) {
            *pSize = totalRead;
            return eError_timeout;
        }

        /* Actually read now from kernel */
        const int bytesRead = read(pHandle->sockfd, pData, *pSize);
        if (-1 == bytesRead) {
            /* Socket error */
            return eError_failed;
        } else if (0 == bytesRead) {
            /* Connection lost */
            break;
        }

        /* Progress */
        totalRead += bytesRead;
        pData += bytesRead;
    }

    *pSize = totalRead;

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
