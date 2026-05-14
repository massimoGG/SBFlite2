/**
 * @file network.h
 * @brief everything related about networking sockets
 */
#ifndef NETWORK_H__
#define NETWORK_H__

#include <error_codes.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum networkState {
    eNetworkState_idle,
    eNetworkState_ok,
    eNetworkState_failed,
    eNetworkState_lostConnection,
    eNetworkState_closedConnection,
} networkState_e;

/** @brief Network Handle */
typedef struct networkHandle {
    /** @brief The socket file descriptor */
    int sockfd;

    /** @brief The desired timeout for each network system call */
    uint32_t timeout;
} networkHandle_t;

networkHandle_t* network_init(uint32_t timeoutMs);
error_e network_connect(networkHandle_t* pHandle, const char* ip, uint16_t port);
error_e network_write(networkHandle_t* pHandle, const uint8_t* pData,
    uint16_t size);
error_e network_readUntil(networkHandle_t* pHandle, uint8_t* pData, uint16_t* pSize);
error_e network_getState(networkHandle_t* pHandle);
error_e network_close(networkHandle_t* pHandle);
error_e network_deinit(networkHandle_t* pHandle);

#ifdef __cplusplus
} // extern "C"
#endif

#endif