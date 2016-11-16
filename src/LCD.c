#include "LCD.h"

#include "em_cmu.h"
#include "em_emu.h"
#include "em_int.h"
#include "em_gpio.h"
#include "em_lcd.h"
#include "segmentlcd.h"

void LCD_init_fn(void)
{
	/* Configuration structure for LCD animation. */
	static const LCD_AnimInit_TypeDef animInit =
	{
		.enable      = true,                  /* Enable animation. */
		.AReg        = 0x00,                  /* Set up animation start data. */
		.AShift      = lcdAnimShiftLeft,      /* Register A Shift direction. */
		.BReg        = 0x01,                  /* Set up animation start data. */
		.BShift      = lcdAnimShiftRight,     /* Register B Shift direction. */
		.animLogic   = lcdAnimLogicOr,        /* Logic function. */
	};

	/* Configuration structure for frame counter. */
	static const LCD_FrameCountInit_TypeDef fcInit =
	{
		 .enable      = true,                  /* Enable frame counter. */
		 .top         = 0x3,                   /* Frame counter period. */
		 .prescale    = lcdFCPrescDiv1         /* Set frame counter prescaler. */
	};

	/* Initialize Animation */
	LCD_AnimInit(&animInit);

	/* Configure and start the frame counter which clocks the animation state machine. */
	LCD_FrameCountInit(&fcInit);
}

void LCD_write_string(char *string)
{
	/* Initialize LCD controller without boost. */
	SegmentLCD_Init(false);

	/* Write something to text field on LCD display. */
	SegmentLCD_Write(string);

	/* Configure Animation. */
	LCD_init_fn();

	/* Enable segment blinking. */
	LCD_BlinkEnable(false);        /* Set to true to see blink feature. */

	/* Set contrast to suitable level for 3.3V. */
	LCD_ContrastSet(0x1A);
}

void LCD_write_number(int number_val)
{
	/* Initialize LCD controller without boost. */
	SegmentLCD_Init(false);

	/* Write something to text field on LCD display. */
	SegmentLCD_Number(number_val);

	/* Configure Animation. */
	LCD_init_fn();

	/* Enable segment blinking. */
	LCD_BlinkEnable(false);        /* Set to true to see blink feature. */

	/* Set contrast to suitable level for 3.3V. */
	LCD_ContrastSet(0x1A);
}

void LCD_off(void)
{
	void SegmentLCD_AllOff(void);
}




