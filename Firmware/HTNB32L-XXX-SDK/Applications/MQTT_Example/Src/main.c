/**
 *
 * Copyright (c) 2023 HT Micron Semicondutores S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
// #include "queue.h"
#include "semphr.h"

// QueueHandle_t xFila;
SemaphoreHandle_t xSemaforo;

static uint32_t uart_cntrl = (ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 | ARM_USART_PARITY_NONE | 
                                ARM_USART_STOP_BITS_1 | ARM_USART_FLOW_CONTROL_NONE);

extern USART_HandleTypeDef huart1;

void Task1(void *pvParameters) {
    bool last_button_state = true;
    while (1) {
        bool button_state = (bool) GPIO_PinRead(BLUE_BUTTON_INSTANCE, BLUE_BUTTON_PIN);
        if (button_state != last_button_state)
        {
            if (!button_state)
            {
                xSemaphoreGive(xSemaforo);
            }
            last_button_state = button_state;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }  
}

void Task2(void *pvParameters) {
    while (1) {
        if (xSemaphoreTake(xSemaforo, portMAX_DELAY))
        {
            HT_GPIO_WritePin(BLUE_LED_PIN, BLUE_LED_INSTANCE, LED_OFF);
            vTaskDelay(pdMS_TO_TICKS(500));
            HT_GPIO_WritePin(BLUE_LED_PIN, BLUE_LED_INSTANCE, LED_ON);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

/**
  \fn          int main_entry(void)
  \brief       main entry function.
  \return
*/
void main_entry(void) {
    HAL_USART_InitPrint(&huart1, GPR_UART1ClkSel_26M, uart_cntrl, 115200);
    
    // xFila = xQueueCreate(10, sizeof(bool));

    // if (xFila == NULL)
    // {
    //     printf("Erro ao criar fila\n");
    //     while(1);
    // }


    xSemaforo = xSemaphoreCreateBinary();

    if (xSemaforo == NULL)
    {
        printf("Erro ao criar semaforo\n");
        while(1);
    }

    printf("Exemplo FreeRTOS\n");

    HT_GPIO_ButtonInit();
    HT_GPIO_LedInit();

    xTaskCreate(Task1, "Button", 128, NULL, 1, NULL);
    xTaskCreate(Task2, "LED", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    
    printf("Nao deve chegar aqui.\n");

    while(1);

}

/******** HT Micron Semicondutores S.A **END OF FILE*/