#ifndef HEADER_GESTURE
#define HEADER_GESTURE

#define REG_ENABLE         0x80
#define REG_ATIME          0x81
#define REG_WTIME          0x83
#define REG_AILTL          0x84
#define REG_AILTH          0x85
#define REG_AIHTL          0x86
#define REG_AIHTH          0x87
#define REG_PILT           0x89
#define REG_PIHT           0x8B
#define REG_PERS           0x8C
#define REG_CONFIG1        0x8D
#define REG_PPULSE         0x8E
#define REG_CONTROL        0x8F
#define REG_CONFIG2        0x90
#define REG_ID             0x92
#define REG_STATUS         0x93
#define REG_CDATAL         0x94
#define REG_CDATAH         0x95
#define REG_RDATAL         0x96
#define REG_RDATAH         0x97
#define REG_GDATAL         0x98
#define REG_GDATAH         0x99
#define REG_BDATAL         0x9A
#define REG_BDATAH         0x9B
#define REG_PDATA          0x9C
#define REG_POFFSET_UR     0x9D
#define REG_POFFSET_DL     0x9E
#define REG_CONFIG3        0x9F
#define REG_GPENTH         0xA0
#define REG_GEXTH          0xA1
#define REG_GCONF1         0xA2
#define REG_GCONF2         0xA3
#define REG_GOFFSET_U      0xA4
#define REG_GOFFSET_D      0xA5
#define REG_GOFFSET_L      0xA7
#define REG_GOFFSET_R      0xA9
#define REG_GPULSE         0xA6
#define REG_GCONF3         0xAA
#define REG_GCONF4         0xAB
#define REG_GFLVL          0xAE
#define REG_GSTATUS        0xAF
#define REG_IFORCE         0xE4
#define REG_PICLEAR        0xE5
#define REG_CICLEAR        0xE6
#define REG_AICLEAR        0xE7
#define REG_GFIFO_U        0xFC
#define REG_GFIFO_D        0xFD
#define REG_GFIFO_L        0xFE
#define REG_GFIFO_R        0xFF

/*Default Values*/

#define ATIME			219
#define WTIME           0xFF     // 27ms
#define PROX_PPULSE     0x87    // 16us, 8 pulses
#define GESTURE_PPULSE  0x89    // 16us, 10 pulses
#define POFFSET_UR      0       // 0 offset
#define POFFSET_DL      0       // 0 offset
#define CONFIG1         0x60    // No 12x wait (WTIME) factor
#define PGAIN           2
#define AGAIN           1
#define PILT            0       // Low proximity threshold
#define PIHT            50      // High proximity threshold
#define AILT            0xFFFF  // Force interrupt for calibration
#define AIHT            0
#define PERS            0xA1    // 2 consecutive prox or ALS for int.
#define CONFIG2         0x01    // No saturation interrupts or LED boost
#define CONFIG3         0       // Enable all photodiodes, no SAI
#define GPENTH          40      // Threshold for entering gesture mode
#define GEXTH           30      // Threshold for exiting gesture mode
#define GCONF1          0x40    // 4 gesture events for int., 1 for exit
#define GGAIN           2
#define GLDRIVE         0
#define GWTIME          1
#define GOFFSET         0       // No offset scaling for gesture mode
#define GPULSE          0xC9    // 32us, 10 pulses
#define GCONF3          0       // All photodiodes active during gesture
#define GIEN            0        // Disable gesture interrupts

#define GVALID         0b00000001
#define FIFO_TIME		30

#define GESTURE_SENSITIVITY 50
#define GESTURE_THRESHOLD 10
void GPIO_ODD_IRQHandler(void);
void gpio_interrupt_enable();
uint8_t valueread1(uint8_t value);
void writeAPDS9960_settings();
bool processgesture();
void decodegesture();
void delay(uint8_t x);
uint8_t readgesture();

enum {
  LEFT,
  RIGHT,
  UP,
  DOWN,
  NONE,
};

int gesture_ud_d;
int gesture_lr_d;
int gesture_ud_c;
int gesture_lr_c;
int gesture_motion;
#endif
