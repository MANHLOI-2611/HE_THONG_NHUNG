#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "tim.h"
#include <stdint.h>

#define NUM_LED        3
#define PWM_MAX        1000
#define PWM_STEP       100
#define PWM_INIT       500
#define BTN_POLL_MS    20
#define QUEUE_LEN      8

typedef enum {
    CMD_INCREASE = 0,
    CMD_DECREASE
} LedCmd_t;

typedef struct {
    LedCmd_t cmd;
} LedMsg_t;

static const uint32_t led_channel[NUM_LED] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3 };

static osMessageQueueId_t ledQueue[NUM_LED];
static osThreadId_t ledTaskHandle[NUM_LED];
static osThreadId_t buttonTaskHandle;

static const osThreadAttr_t ledTaskAttr[NUM_LED] = {
    { .name = "LED1", .stack_size = 128 * 4, .priority = osPriorityNormal },
    { .name = "LED2", .stack_size = 128 * 4, .priority = osPriorityNormal },
    { .name = "LED3", .stack_size = 128 * 4, .priority = osPriorityNormal },
};
static const osThreadAttr_t buttonTaskAttr = {
    .name = "BTN", .stack_size = 128 * 4, .priority = osPriorityAboveNormal
};

static void LedTask(void *argument)
{
    uint32_t idx = (uint32_t)argument;
    uint32_t duty = PWM_INIT;
    LedMsg_t msg;

    __HAL_TIM_SET_COMPARE(&htim2, led_channel[idx], duty);

    for (;;) {
        if (osMessageQueueGet(ledQueue[idx], &msg, NULL, osWaitForever) == osOK) {
            if (msg.cmd == CMD_INCREASE) {
                duty = (duty + PWM_STEP > PWM_MAX) ? PWM_MAX : duty + PWM_STEP;
            } else {
                duty = (duty < PWM_STEP) ? 0 : duty - PWM_STEP;
            }
            __HAL_TIM_SET_COMPARE(&htim2, led_channel[idx], duty);
        }
    }
}

static void ButtonTask(void *argument)
{
    (void)argument;
    uint8_t selected = 0;
    uint8_t last[3] = {1, 1, 1};
    uint8_t now[3];
    LedMsg_t msg;

    for (;;) {
        now[0] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12);
        now[1] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13);
        now[2] = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);

        if (last[0] == 1 && now[0] == 0) {
            selected = (selected + 1) % NUM_LED;
        }
        if (last[1] == 1 && now[1] == 0) {
            msg.cmd = CMD_INCREASE;
            osMessageQueuePut(ledQueue[selected], &msg, 0, 0);
        }
        if (last[2] == 1 && now[2] == 0) {
            msg.cmd = CMD_DECREASE;
            osMessageQueuePut(ledQueue[selected], &msg, 0, 0);
        }

        last[0] = now[0];
        last[1] = now[1];
        last[2] = now[2];

        osDelay(BTN_POLL_MS);
    }
}

void MX_FREERTOS_Init(void)
{
    for (uint32_t i = 0; i < NUM_LED; i++) {
        ledQueue[i] = osMessageQueueNew(QUEUE_LEN, sizeof(LedMsg_t), NULL);
    }
    for (uint32_t i = 0; i < NUM_LED; i++) {
        ledTaskHandle[i] = osThreadNew(LedTask, (void *)i, &ledTaskAttr[i]);
    }
    buttonTaskHandle = osThreadNew(ButtonTask, NULL, &buttonTaskAttr);
}
