/*
* sd.c
*
* Created: 11/29/20 10:56:43 AM
*  Author: johnn
*/

#include "ff.h"
#include "diskio.h"

FIL file;                                               /* Opened file object */
FATFS fatfs;                                            /* File system object */
DIR dir;                                                /* Directory object   */
UINT bytesRead;                                         /* Bytes read object  */

void sd_init( void )
{
	f_mount(0, &fatfs);
}

void sd_write( unsigned char *str, int size )
{
	f_opendir(&dir, "/");
	f_open(&file, "/data.txt", FA_OPEN_ALWAYS | FA_WRITE);
	f_lseek(&file, f_size(&file)); // f_size(&file)
	f_write(&file, str, size, &bytesRead);
	f_close(&file);
}