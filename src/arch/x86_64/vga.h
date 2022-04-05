#ifndef VGA_H
#define VGA_H

void VGA_clear(void);
void VGA_display_char(char c);
void VGA_dispaly_str(const char *str);
void scroll_text(void);

#endif
