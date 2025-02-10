/*
 * main.c
 *
 *  Created on: Dec 19, 2024
 *      Author: alaa hassan
 */
#include "MCAL/DIO_intervace.h"
#include<avr/io.h>


#include "lib/std_typs.h"
#include "lib/BIT_MATH.h"


#include "hal/LED_intreface.h"
#include "hal/LM35_interface.h"
#include "MCAL/ADC_interface.h"
#include "MCAL/TWI_interface.h"

#include "hal/CLCD_intreface.h"  // افترض أن لديك دريفر للـ LCD
#include <util/delay.h>


// تعريف الـ LEDs
LED_config LED_Blue = {PORTC, PIN0, ACTIVE_HIGH};  // LED أزرق
LED_config LED_Yellow = {PORTC, PIN1, ACTIVE_HIGH}; // LED أصفر
LED_config LED_Red = {PORTC, PIN2, ACTIVE_HIGH};    // LED أحمر

// تهيئة الـ LM35
LM35_Config lm35_config = {
    .Copy_u8LM35Channel = ADC_CHANNEL0,  // استخدام القناة 0 لـ ADC
    .Copy_u8ADCVoltageRefrence = AVCC,   // استخدام جهد AVCC كمرجع
    .Copy_u8ADCRes = ADC_RES_10_BIT      // دقة 10 بت
};

int main(void) {
    u8 temperature = 0;
    u16 analog_signal = 0;

    // تهيئة الـ LCD
    CLCD_voidInit();
    CLCD_voidClearScreen();

    // تهيئة الـ LEDs
    LED_vInit(LED_Blue);
    LED_vInit(LED_Yellow);
    LED_vInit(LED_Red);

    // تهيئة الـ ADC
    ADC_vInit();
    ADC_vEnable();

    while (1) {
        // قراءة درجة الحرارة من الـ LM35
        LM35_u8GetTemp(&lm35_config, &temperature);
        temperature=temperature+5;

        // عرض درجة الحرارة على الـ LCD
        CLCD_voidClearScreen();
        CLCD_voidSetPosition(CLCD_Row_1, CLCD_col_1);
        CLCD_voidsendString("Temp: ");
        CLCD_vSendIntNumber(temperature);
        CLCD_voidsendString(" C");

        // التحكم في الـ LEDs بناءً على درجة الحرارة
        if (temperature < 20) {
            // درجة الحرارة أقل من 20
            CLCD_voidSetPosition(CLCD_Row_2, CLCD_col_1);
            CLCD_voidsendString("Cold");
            LED_vTurnOn(LED_Blue);
            LED_vTurnOff(LED_Yellow);
            LED_vTurnOff(LED_Red);
        } else if (temperature >= 20 && temperature < 35) {
            // درجة الحرارة بين 20 و 35
            CLCD_voidSetPosition(CLCD_Row_2, CLCD_col_1);
            CLCD_voidsendString("Normal");
            LED_vTurnOff(LED_Blue);
            LED_vTurnOn(LED_Yellow);
            LED_vTurnOff(LED_Red);
        } else if (temperature >= 35) {
            // درجة الحرارة أكبر من أو تساوي 35
            CLCD_voidSetPosition(CLCD_Row_2, CLCD_col_1);
            CLCD_voidsendString("Hot");
            LED_vTurnOff(LED_Blue);
            LED_vTurnOff(LED_Yellow);
            LED_vTurnOn(LED_Red);
        }

        // تأخير لقراءة درجة الحرارة كل ثانية
        _delay_ms(1000);
    }

    return 0;
}
