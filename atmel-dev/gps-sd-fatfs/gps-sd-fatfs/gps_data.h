#ifndef GPS_DATA_H_
#define GPS_DATA_H_

#include <stdbool.h>

void get_position();
void fat_init();
void sd_write(unsigned char *, unsigned char *);
int check_if_fix();

#endif /* GPS_DATA_H_ */