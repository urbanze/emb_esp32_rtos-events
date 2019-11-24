#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_log.h>

EventGroupHandle_t evt;//Cria o objeto do grupo de eventos

#define EV_1SEG (1<<0)//Define o BIT do evento


void t1(void*z)
{
    EventBits_t x;//Cria a variavel que recebe o valor dos eventos
    while (1)
    {
        //Vamos esperar pelo evento (EV_1SEG) por no maximo 1000ms
        x = xEventGroupWaitBits(evt, EV_1SEG, true, true, pdMS_TO_TICKS(1000));

        if (x & EV_1SEG)//Se X & EV_1SEG (mascara binaria), significa que o evento ocorreu
        {
            ESP_LOGI("T1", "OK");
        }
        else
        {
            ESP_LOGE("T1", "Event group TIMEOUT");
        }
    }
}

extern "C" void app_main()
{
    evt = xEventGroupCreate();//Cria o grupo de eventos

    xTaskCreatePinnedToCore(t1, "t1", 2048, NULL, 1, NULL, 0);//Cria a tarefa que espera pelos eventos

    while (1)
    {
        for (uint8_t i = 0; i < 3; i++)//Envia 3 eventos antes do timeout
        {
            vTaskDelay(pdMS_TO_TICKS(333));
            xEventGroupSetBits(evt, EV_1SEG);//Configura o BIT (EV_1SEG) em 1
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));//Espera o timeout para mostrar o erro
    }
}
