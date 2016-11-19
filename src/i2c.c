#include "i2c.h"
#include "SiLabs_errcodes.h"
#include "sleep.h"
#include "clock.h"
#include "main.h"

#include "em_i2c.h"
#include "em_cmu.h"
#include "em_int.h"
#include "em_emu.h"

#define rx_Buffer_size 10
#define tx_Buffer_size 2

volatile bool I2C_rx_active;
volatile bool I2C_tx_start;

uint8_t I2C_rx_buffer_index;
uint8_t I2C_tx_buffer_index;

uint8_t I2C_tx_buffer[] = { 0x00, 0x00 };
uint8_t I2C_tx_buffer_size = tx_Buffer_size;

uint8_t I2C_tx_buffer1[] = { 0x00 };
uint8_t I2C_tx_buffer_size1 = 1;

static uint8_t I2C_rx_buffer_size = 10;
static uint8_t I2C_rx_buffer[rx_Buffer_size];

I2C_TransferSeq_TypeDef i2c_transfer;

/*
 * @brief Initialize the I2C on Leopard Gecko
 * @param: void
 * @Steps:
 * 1. Enable the clocks for HFPER and I2C1
 * 2. Write to the route registers for respective location of the GPIO pins for I2C communication
 * 3. Initialize the I2C peripheral on the Leopard Gecko
 * 4. Check the state of the I2C peripheral
 * 5. Write the slave address to the register
 * 6. Write to the control register of the I2C peripheral for Auto ack, Auto stop, Cntrl Slave and Cntrl Enable
 * 7. Return error code SUCCESS
 */
unsigned int i2c_init_fn(void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_I2C1, true);

	I2C_Init_TypeDef i2c_init =
			{                                                                         				\
			  true,                    /* Enable when init done */                    				\
			  true,                    /* Set to master mode */                       				\
			  0,                       /* Use currently configured reference clock */ 				\
			  I2C_FREQ_STANDARD_MAX,   /* Set to standard rate assuring being within I2C spec */    \
			  i2cClockHLRStandard      /* Set to use 4:4 low/high duty cycle */       				\
			};

	I2C_IntClear(I2C1, I2C_IEN_ADDR | I2C_IEN_RXDATAV | I2C_IEN_SSTOP);

	I2C1->ROUTE = I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN | (0 << _I2C_ROUTE_LOCATION_SHIFT);
	I2C_Init(I2C1, &i2c_init);

	if(I2C1->STATE && I2C_STATE_BUSY)
	{
		I2C1->CMD = I2C_CMD_ABORT;
	}

	I2C_rx_active = false;
	I2C_tx_start = false;
	I2C_rx_buffer_index = 0;

	I2C1->SADDR = I2C_Slave_Address;
	I2C1->CTRL |= I2C_CTRL_SLAVE | I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN | I2C_CTRL_EN;
	return SILABS_SUCCESS;
}

/*
 * @brief Write function for I2C communication protocol
 * @param: void
 * @Steps:
 * 1. Write the slave address
 * 2. Write the flags to perform a write operation
 * 3. Update the on board I2C transmit buffer
 * 4. Update the on board I2C transmit buffer size
 * 5. Update the on board I2C receive buffer
 * 6. Update the on board I2C receive buffer size
 * 7. Transfer the Initialize values
 * 8. Wait till the I2C finishes transmitting
 */
void i2c_write(void)
{
	i2c_transfer.flags = I2C_FLAG_WRITE;
	i2c_transfer.buf[0].data = I2C_tx_buffer;
	i2c_transfer.buf[0].len = I2C_tx_buffer_size;
	i2c_transfer.buf[1].data = I2C_rx_buffer;
	i2c_transfer.buf[1].len = I2C_rx_buffer_size;

	if(0)
	{
		i2c_transfer.addr = I2C_Slave_Address;
		I2C_TransferInit(I2C1, &i2c_transfer);
	}

	while (I2C_Transfer(I2C1) == i2cTransferInProgress);
}

/*
 * @brief Read function for I2C communication protocol
 * @param: void
 * @Steps:
 * 1. Write the slave address
 * 2. Write the flags to perform a write read operation
 * 3. Update the on board I2C transmit buffer
 * 4. Update the on board I2C transmit buffer size
 * 5. Update the on board I2C receive buffer
 * 6. Update the on board I2C receive buffer size
 * 7. Transfer the Initialize values
 * 8. Wait till the I2C finishes transmitting
 */
void i2c_read(void)
{
	I2C_TransferReturn_TypeDef i2c_status;

	i2c_transfer.flags = I2C_FLAG_WRITE_READ;

	i2c_transfer.buf[0].data = I2C_tx_buffer1;
	i2c_transfer.buf[0].len = I2C_tx_buffer_size1;

	i2c_transfer.buf[1].data = I2C_rx_buffer;
	i2c_transfer.buf[1].len = I2C_rx_buffer_size;

	i2c_status = I2C_TransferInit(I2C1, &i2c_transfer);

	while(I2C_Transfer(I2C1) == i2cTransferInProgress);
}

/*
 * @brief Filling respective buffers for I2C read/write
 * @param:
 * 1. Buffer array
 * 2. Read or Write bit
 * 3. Size of the buffer
 * @Steps:
 * 1. Check whether it is for read or for write
 * 2. If read, transfer buffer to the transmit buffer for read operation, update the size
 * 3. Perform I2C read
 * 4. If write,  transfer buffer to the transmit buffer for write operation, update the size
 * 5. Perform I2C write
 */
void i2c_buffer_fill(uint8_t buffer[], uint8_t rd_wr, uint8_t size, uint8_t I2C_periph_flag)
{
	if(I2C_periph_flag == 1)
	{
		i2c_transfer.addr = LSM303C_ACC_I2C_ADDR;
	}
	else if(I2C_periph_flag == 2)
	{
		i2c_transfer.addr = LSM303C_MAG_I2C_ADDR;
	}
	else if(I2C_periph_flag == 3)
	{
		i2c_transfer.addr = BME280_I2C_ADDR;
	}
	else if(I2C_periph_flag == 4)
	{
		i2c_transfer.addr = APDS9960_I2C_ADDR;
	}
	if(rd_wr == 0)
	{
		I2C_tx_buffer1[0] = buffer[0];
		I2C_tx_buffer_size1 = size;
		i2c_read();
	}
	else if(rd_wr == 1)
	{
		for (int i = 0; i < size; i++)
		{
			I2C_tx_buffer[i] = buffer[i];
		}
		I2C_tx_buffer_size = size;
		i2c_write();
	}
}

/*
 * @brief Read the buffer for I2C communication protocol
 * @param: void
 * @Steps:
 * 1. Return the I2C receive buffer value
 */
uint8_t i2c_buffer_read(void)
{
	return I2C_rx_buffer[0];
}
