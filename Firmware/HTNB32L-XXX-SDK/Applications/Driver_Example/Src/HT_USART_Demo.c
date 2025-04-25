
/**
 *
 * Copyright (c) 2024 HT Micron Semicondutores S.A.
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

#include "HT_USART_Demo.h"

extern USART_HandleTypeDef huart1;
static uint8_t rx_buffer[USART_BUFFER_SIZE] = {0};

volatile uint8_t rx_callback = 0;
volatile uint8_t tx_callback = 0;

extern uint8_t *usart_tx_buffer;
extern uint8_t *usart_rx_buffer;
extern uint32_t usart_tx_buffer_size;
extern uint32_t usart_rx_buffer_size;

void HT_USART_Callback(uint32_t event) {
    if(event & ARM_USART_EVENT_RECEIVE_COMPLETE)
        rx_callback = 1;
    
    if(event & ARM_USART_EVENT_TX_COMPLETE)
        tx_callback = 1;
}

void HT_USART_App(void) {

    ht_printf("HTNB32L-XXX USART Example\n");
    
    while(1) {

        HAL_USART_IRQnEnable(&huart1, (USART_IER_RX_DATA_REQ_Msk | USART_IER_RX_TIMEOUT_Msk | USART_IER_RX_LINE_STATUS_Msk));
        HAL_USART_Receive_IT(rx_buffer, USART_BUFFER_SIZE-1);
        ht_printf("Waiting for usart rx data...\n");

        while(!rx_callback);
        rx_callback = 0;
        
        ht_printf("Received: %s\n", (char *)rx_buffer);

        memset(rx_buffer, 0, sizeof(rx_buffer));

        HAL_USART_IRQnDisable(&huart1, (USART_IER_RX_DATA_REQ_Msk | USART_IER_RX_TIMEOUT_Msk | USART_IER_RX_LINE_STATUS_Msk));

    }
    
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
