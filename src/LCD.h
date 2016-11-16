#ifndef HEADER_LCD
#define HEADER_LCD

void LCD_init_fn(void);
void LCD_write_string(char *string);
void LCD_write_number(int number);
void LCD_off(void);

#endif
