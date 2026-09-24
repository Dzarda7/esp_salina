/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * A DNS server that answers every question with our own address.
 *
 * This is what makes the setup page appear by itself: phones probe a known
 * hostname after joining a network, and pointing every name at the board turns
 * that probe into a hit on our web server, which answers with a redirect.
 */
#include <string.h>

#include <sys/socket.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "portal_dns.h"

static const char *TAG = "dns";

#define DNS_PORT        53
#define DNS_MAX_LEN     512
#define DNS_QR_RESPONSE 0x80
#define DNS_TTL_S       30

typedef struct __attribute__((packed)) {
    uint16_t id;
    uint16_t flags;
    uint16_t qd_count;
    uint16_t an_count;
    uint16_t ns_count;
    uint16_t ar_count;
} dns_header_t;

static TaskHandle_t s_task;
static int s_sock = -1;

/** @brief Append an answer pointing the question back at @p addr. */
static int append_answer(uint8_t *reply, int len, uint32_t addr)
{
    /* 0xC00C is a pointer to the question name at offset 12, which saves
     * repeating it: type A, class IN, then the four address bytes. */
    static const uint8_t tail[] = { 0xC0, 0x0C, 0x00, 0x01, 0x00, 0x01 };

    memcpy(reply + len, tail, sizeof(tail));
    len += sizeof(tail);

    const uint32_t ttl = htonl(DNS_TTL_S);
    memcpy(reply + len, &ttl, sizeof(ttl));
    len += sizeof(ttl);

    const uint16_t rd_length = htons(sizeof(addr));
    memcpy(reply + len, &rd_length, sizeof(rd_length));
    len += sizeof(rd_length);

    memcpy(reply + len, &addr, sizeof(addr));
    return len + sizeof(addr);
}

static void dns_task(void *arg)
{
    const uint32_t our_addr = (uint32_t)(uintptr_t)arg;
    uint8_t packet[DNS_MAX_LEN];

    while (true) {
        struct sockaddr_in from;
        socklen_t from_len = sizeof(from);
        const int len = recvfrom(s_sock, packet, sizeof(packet), 0, (struct sockaddr *)&from, &from_len);
        if (len < (int)sizeof(dns_header_t)) {
            if (len < 0) {
                break; /* the socket was closed under us; we are done */
            }
            continue;
        }

        /* The answer we append is 16 bytes; a query that nearly fills the
         * buffer would otherwise be written past the end of it. */
        if (len + 16 > (int)sizeof(packet)) {
            continue;
        }
        dns_header_t *header = (dns_header_t *)packet;
        if (ntohs(header->qd_count) != 1) {
            continue; /* one question per packet is all a probe ever asks */
        }
        header->flags = htons(DNS_QR_RESPONSE << 8);
        header->an_count = header->qd_count;

        const int reply_len = append_answer(packet, len, our_addr);
        sendto(s_sock, packet, reply_len, 0, (struct sockaddr *)&from, from_len);
    }
    vTaskDelete(NULL);
}

esp_err_t portal_dns_start(uint32_t addr)
{
    s_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (s_sock < 0) {
        ESP_LOGE(TAG, "socket: %d", errno);
        return ESP_FAIL;
    }

    const struct sockaddr_in bind_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(DNS_PORT),
        .sin_addr.s_addr = htonl(INADDR_ANY),
    };
    if (bind(s_sock, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0) {
        ESP_LOGE(TAG, "bind: %d", errno);
        close(s_sock);
        s_sock = -1;
        return ESP_FAIL;
    }

    if (xTaskCreate(dns_task, "portal_dns", 3072, (void *)(uintptr_t)addr, 5, &s_task) != pdPASS) {
        close(s_sock);
        s_sock = -1;
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

void portal_dns_stop(void)
{
    if (s_sock >= 0) {
        shutdown(s_sock, SHUT_RDWR);
        close(s_sock);
        s_sock = -1;
    }
    s_task = NULL;
}
