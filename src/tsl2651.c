#include "tsl2651.h"
#include "GPIO.h"
#include "main.h"
#include "i2c.h"

#include "em_i2c.h"
#include "em_gpio.h"
#include "em_int.h"

static uint8_t reg_I2C_tx_buffer_write[2] = {0,0};
static uint8_t reg_I2C_tx_buffer_read[1] = {0};
//static uint8_t init_flag = 0;
static uint8_t low_high_flag = 0;

/*
 * @brief Writing Register values to TSL2561 during enable
 * @param: void
 * @Steps:
 * 1. Power Down the Sensor
 * 2. Power Up the Sensor
 * 3. Set timer in the sensor for 101ms
 * 4. Check for low or high threshold flag
 * 5. Set the respective values in the Threshold registers
 * 6. Set the interrupt register
 */
void tsl2651_write_regs_init(void)
{
	reg_I2C_tx_buffer_write[0] = TSL2561_CTRL_REG;
	reg_I2C_tx_buffer_write[1] = TSL2561_POWER_DOWN;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = TSL2561_CTRL_REG;
	reg_I2C_tx_buffer_write[1] = TSL2561_POWER_UP;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_write[0] = TSL2561_TIMER_REG;
	reg_I2C_tx_buffer_write[1] = 0x01;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

	if(low_high_flag == 0)
	{
		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x0f;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0xff;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0xff;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);
	}
	else if(low_high_flag == 1)
	{
		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x08;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);
	}

	reg_I2C_tx_buffer_write[0] = TSL2561_INT_REG;
	reg_I2C_tx_buffer_write[1] = 0x14;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);
}

/*
 * @brief Reading Register values from TSL2561 to check proper working
 * @param: void
 * @Steps:
 * 1. Read the respective registers of the sensor
 */
void tsl2651_read_regs(void)
{
	reg_I2C_tx_buffer_read[0] = TSL2561_CTRL_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_TIMER_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_THRESH_LOWLOW_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_THRESH_LOWHIGH_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_THRESH_HIGHLOW_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_THRESH_HIGHHIGH_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_INT_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_ADC0_LOW_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);

	reg_I2C_tx_buffer_read[0] = TSL2561_ADC0_HIGH_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1, TSL2561_I2C_FLAG);
}

/*
 * @brief Enable the TSL2561 sensor
 * @param: void
 * @Steps:
 * 1. Set the GPIO power pin to the sensor high
 * 2. Set the drive mode to the lowest
 * 3. Write to the registers in the sensors
 * 4. Enable the falling edge interrupt for the GPIO interrupt pin
 * 5. Clear pending interrupts in the NVIC
 * 6. Enable the interrupts in the NVIC
 */
void tsl2651_enable(void)
{
	GPIO_PinModeSet(I2C_port, I2C_SCL_pin_number, gpioModeWiredAndPullUpFilter, 1);
	GPIO_PinModeSet(I2C_port, I2C_SDA_pin_number, gpioModeWiredAndPullUpFilter, 1);
	GPIO_PinModeSet(I2C_Power_port, I2C_Interrupt_pin, gpioModeInput, 0);
	GPIO_PinModeSet(I2C_Power_port, I2C_Power_pin, gpioModePushPull, 0);
	GPIO_PinOutSet(I2C_Power_port, I2C_Power_pin);
	GPIO_DriveModeSet(I2C_port, gpioDriveModeLowest);
	for(int i = 0; i < 200; i++);
	tsl2651_write_regs_init();
	GPIO_IntConfig(I2C_Power_port, I2C_Interrupt_pin, false, true, true);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

/*
 * @brief Disable the TSL2561 sensor
 * @param: void
 * @Steps:
 * 1. Disable the falling edge interrupt on the GPIO interrupt pin
 * 2. Power down the sensor
 * 3. Clear pending interrupts in the NVIC
 * 4. Disable the GPIO interrupt in the NVIC
 * 5. Set the drive mode for the port back to its original value
 * 6. Switch Off power to the sensor
 */
void tsl2651_disable(void)
{
	GPIO_IntConfig(I2C_Power_port, I2C_Interrupt_pin, false, false, false);
	reg_I2C_tx_buffer_write[0] = TSL2561_CTRL_REG;
	reg_I2C_tx_buffer_write[1] = TSL2561_POWER_DOWN;
	i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);
	NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	NVIC_DisableIRQ(GPIO_ODD_IRQn);
	GPIO_DriveModeSet(I2C_port, gpioDriveModeLowest);
	GPIO_DriveModeSet(I2C_Power_port, gpioDriveModeLowest);
	GPIO_PinOutClear(I2C_Power_port, I2C_Power_pin);
	GPIO_PinOutClear(I2C_port, I2C_SCL_pin_number);
	GPIO_PinOutClear(I2C_port, I2C_SDA_pin_number);
	GPIO_PinModeSet(I2C_port, I2C_SCL_pin_number, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C_port, I2C_SDA_pin_number, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C_Power_port, I2C_Interrupt_pin, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C_Power_port, I2C_Power_pin, gpioModeDisabled, 0);
}

/*
 * @brief GPIO Port D, Pin 1 Interrupt Handler
 * @param: void
 * @Steps:
 * 1. Disable all interrupts
 * 2. Clear the respective GPIO interrupt
 * 3. Clear the bit on the command register in the sensor
 * 4. Check whether we want to read the ADC buffer registers or not
 * 5. If yes,
 * 		a. Read ADC Low Register
 * 		b. Read ADC High Register
 * 		c. Club them to make a 16 bit value
 * 		d. Check whether it is lower than the threshold low value, set the LED and change the threshold values if true
 * 		e. Check whether it is higher than the threshold high value, clear the LED and change the threshold values if true
 * 5. If no,
 * 		a. Check for count, so that it enters the IRQ handler only once.
 * 		b. Set the led and change threshold values because first interrupt will always be that for the low threshold
 * 		c. Clear the led and change threshold values because the next interrupt will be because of a high threshold
 * 6. Enable all interrupts
 */
void GPIO_ODD_IRQHandler(void)
{
	static uint32_t count;
	static uint8_t threshold_flag = 0;
	INT_Disable();
	uint32_t GPIO_int_flags = GPIO_IntGet();
	GPIO_IntClear(GPIO_int_flags);

	reg_I2C_tx_buffer_read[0] = 0xC0;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 1, 1, TSL2561_I2C_FLAG);

#ifdef READ_SENSOR_ADC_BUFFERS
	uint8_t buffer_low, buffer_high = 0;
	uint16_t buffer_adc_val = 0;

	reg_I2C_tx_buffer_read[0] = TSL2561_ADC0_LOW_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1);
	buffer_low = i2c_buffer_read();

	reg_I2C_tx_buffer_read[0] = TSL2561_ADC0_HIGH_REG;
	i2c_buffer_fill(reg_I2C_tx_buffer_read, 0, 1);
	buffer_high = i2c_buffer_read();

	buffer_adc_val = ((uint16_t)buffer_high << 8 | 0x0000) | buffer_low;

	if(buffer_adc_val < TSL2561_ADC_LOW_VAL && threshold_flag == 0)
	{
		threshold_flag = 1;
		reg_I2C_tx_buffer_write[0] = 0x82;
		reg_I2C_tx_buffer_write[1] = 0x00;

		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);
		reg_I2C_tx_buffer_write[0] = 0x83;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		reg_I2C_tx_buffer_write[0] = 0x84;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		reg_I2C_tx_buffer_write[0] = 0x85;
		reg_I2C_tx_buffer_write[1] = 0x08;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		low_high_flag = 1;

		GPIO_on_off(0, LED_pin_port, LED0_pin_number);
	}
	else if(buffer_adc_val > 1000 && threshold_flag == 1)
	{
		GPIO_on_off(1, LED_pin_port, LED0_pin_number);
		threshold_flag = 0;

		reg_I2C_tx_buffer_write[0] = 0x84;
		reg_I2C_tx_buffer_write[1] = 0xFF;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		reg_I2C_tx_buffer_write[0] = 0x85;
		reg_I2C_tx_buffer_write[1] = 0xFF;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		reg_I2C_tx_buffer_write[0] = 0x82;
		reg_I2C_tx_buffer_write[1] = 0x0f;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		reg_I2C_tx_buffer_write[0] = 0x83;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2);

		low_high_flag = 0;
	}
#else

	if(count == 0)
	{
		count = 1;
	}
	else if(count == 1)
	{
		count = 0;
		INT_Enable();
		return;
	}

	if(threshold_flag == 0)
	{
		threshold_flag = 1;
		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;

		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);
		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x08;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		low_high_flag = 1;

		GPIO_on_off(0, LED_pin_port, LED0_pin_number);
		//led_flag = 1;
	}
	else if(threshold_flag == 1)
	{
		GPIO_on_off(1, LED_pin_port, LED0_pin_number);
		//led_flag = 0;
		threshold_flag = 0;

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0xFF;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_HIGHHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0xFF;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWLOW_REG;
		reg_I2C_tx_buffer_write[1] = 0x0f;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		reg_I2C_tx_buffer_write[0] = TSL2561_THRESH_LOWHIGH_REG;
		reg_I2C_tx_buffer_write[1] = 0x00;
		i2c_buffer_fill(reg_I2C_tx_buffer_write, 1, 2, TSL2561_I2C_FLAG);

		low_high_flag = 0;
	}
#endif
	INT_Enable();
}
