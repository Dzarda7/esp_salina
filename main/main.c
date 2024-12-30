/* ESP HTTP Client Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include <sys/param.h>
#include <stdlib.h>
#include <ctype.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "protocol_examples_utils.h"
#include "esp_tls.h"
#include "esp_crt_bundle.h"
#include "cJSON.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "esp_http_client.h"

#define MAX_HTTP_OUTPUT_BUFFER 2048
static const char *TAG = "HTTP_CLIENT";

static void https_native_request(char *output_buffer)
{
    esp_http_client_config_t config = {
        .host = "www.idsjmk.cz",
        .path = "/api/departures/busstop-by-name?busStopName=Kartouzsk%C3%A1",
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        // Certificate is not provided, so server verification will be disabled which enables MITM attack!
        // This should not be a problem, because data is not sensitive and very important.
        // Reason is to avoid need of certificate update in case of expiration.
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_method(client, HTTP_METHOD_GET);
    esp_err_t err = esp_http_client_open(client, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
    } else {
        int content_length = esp_http_client_fetch_headers(client);
        if (content_length < 0) {
            ESP_LOGE(TAG, "HTTP client fetch headers failed");
        } else {
            int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
            if (data_read >= 0) {
                output_buffer[data_read] = '\0'; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
                if (data_read < content_length) {
                    ESP_LOGW(TAG, "HTTP client read data incomplete, increase buffer size to handle all data");
                }
            } else {
                ESP_LOGE(TAG, "Failed to read response");
            }
        }
    }
    esp_http_client_close(client);
    esp_http_client_cleanup(client);
}

typedef struct departure_t {
    char line[3];
    char destination[50];
    char time[6];
    struct departure_t *next;
} departure_t;
    

static departure_t *json_parse(char *json)
{
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        ESP_LOGE(TAG, "Error before: [%s]\n", cJSON_GetErrorPtr());
        return NULL;
    }

    cJSON *stops = cJSON_GetObjectItemCaseSensitive(root, "stops");
    cJSON *data = cJSON_GetArrayItem(stops, 0);
    cJSON *signs = cJSON_GetObjectItemCaseSensitive(data, "signs");
    cJSON *sign = cJSON_GetArrayItem(signs, 0);

    cJSON *departures = cJSON_GetObjectItemCaseSensitive(sign, "departures");
    cJSON *departure = NULL;

    departure_t *head = NULL;
    departure_t *current = NULL;
    cJSON_ArrayForEach(departure, departures) {
        cJSON *link = cJSON_GetObjectItemCaseSensitive(departure, "link");
        cJSON *destination = cJSON_GetObjectItemCaseSensitive(departure, "destinationStop");
        cJSON *time = cJSON_GetObjectItemCaseSensitive(departure, "time");
        
        departure_t *new_departure = calloc(1, sizeof(departure_t));
        if (new_departure == NULL) {
            ESP_LOGE(TAG, "Memory allocation failed");
            cJSON_Delete(root);
            return NULL;
        }

        strncpy(new_departure->line, link->valuestring, sizeof(new_departure->line) - 1);
        strncpy(new_departure->destination, destination->valuestring, sizeof(new_departure->destination) - 1);

        if (time->valuestring[0] > '9') {
            strncpy(new_departure->time, &time->valuestring[3], sizeof(new_departure->time) - 1);
        } else {
            strncpy(new_departure->time, time->valuestring, sizeof(new_departure->time) - 1);
        }
        new_departure->next = NULL;

        if (head == NULL) {
            head = new_departure;
        } else {
            current->next = new_departure;
        }
        current = new_departure;
    }
    cJSON_Delete(root);
    return head;
}

static void json_free(departure_t *head)
{
    departure_t *current = head;
    departure_t *next = NULL;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

static void get_data_task(void *pvParameters)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    ESP_ERROR_CHECK(example_connect());
    ESP_LOGI(TAG, "Connected to AP, begin http example");

    char output_buffer[MAX_HTTP_OUTPUT_BUFFER + 1] = {0}; // +1 for null termination
    https_native_request(output_buffer);
    if (strlen(output_buffer) == 0) {
        ESP_LOGE(TAG, "No data received");
        vTaskDelete(NULL);
    }
    departure_t *head = json_parse(output_buffer);
    if (head == NULL) {
        ESP_LOGE(TAG, "Parsing JSON failed");
        vTaskDelete(NULL);
    }

    departure_t *current = head;
    while (current != NULL) {
        ESP_LOGI(TAG, "Line: %s, Destination: %s, Time: %s", current->line, current->destination, current->time);
        current = current->next;
    }
    json_free(head);

    ESP_LOGI(TAG, "Finish http example");
    vTaskDelete(NULL);
}

void app_main(void)
{
    xTaskCreate(&get_data_task, "get_data_task", 8192, NULL, 5, NULL);
}
