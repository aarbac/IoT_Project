#ifndef HEADER_TSL2651
#define HEADER_TSL2651

#define TSL2561_CTRL_REG				0x80
#define TSL2561_TIMER_REG				0x81
#define TSL2561_THRESH_LOWLOW_REG		0x82
#define TSL2561_THRESH_LOWHIGH_REG		0x83
#define TSL2561_THRESH_HIGHLOW_REG		0x84
#define TSL2561_THRESH_HIGHHIGH_REG		0x85
#define TSL2561_INT_REG					0x86
#define TSL2561_ADC0_LOW_REG			0x8C
#define	TSL2561_ADC0_HIGH_REG			0x8D

#define TSL2561_POWER_UP				0x03
#define TSL2561_POWER_DOWN				0x00

#define TSL2561_ADC_HIGH_VAL	0x0800
#define	TSL2561_ADC_LOW_VAL		0x000F

void tsl2651_disable(void);
void tsl2651_enable(void);

void tsl2651_read_regs(void);
void tsl2651_write_regs_init(void);

#endif
