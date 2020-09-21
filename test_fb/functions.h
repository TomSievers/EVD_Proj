#define _XOPEN_SOURCE 700 
#define _XOPEN_SOURCE_EXTENDED
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h> 
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <inttypes.h>
#include <unistd.h>
#include <sys/kd.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <zlib.h>
#include <stdint.h>
#include <termios.h>
#include <signal.h>

static bool set_tty(const char* dev, const int mode)
{
    int con_fd = open(dev, O_RDWR);
	bool suc = true;

    if (!con_fd) {
        fprintf(stderr,"Could not open console.\n");
		suc = false;
    }

    if (mode)
	{
		if (ioctl( con_fd, KDSETMODE, KD_GRAPHICS))
		{
			fprintf(stderr,"Could not set console to KD_GRAPHICS mode, err: %d\n", errno);
			suc = false;
		}
	} else {
		if (ioctl( con_fd, KDSETMODE, KD_TEXT))
		{
			fprintf(stderr,"Could not set console to KD_TEXT mode, err: %d\n", errno);
			suc = false;
		}
	}

    close(con_fd);
	return suc;
}

bool set_input(struct termios* term, const int mode)
{
	switch (mode)
	{
	case 0:
		{
			tcgetattr( fileno(stdin), term );

			term->c_lflag &= ~ICANON;
			term->c_lflag &= ~ECHO;

			tcsetattr( fileno(stdin), TCSANOW, term);
		}
		break;
	case 1:
		{
			term->c_lflag |= ICANON;
			term->c_lflag |= ECHO;

			tcsetattr( fileno(stdin), TCSANOW, term);
		}
		break;
	default:
		fprintf(stderr, "Unsupported mode");
		return false;
	}
	return true;
}

bool is_pressed(char c)
{
	char buf;
	if(read(fileno(stdin), &buf, 1) == -1)
	{
		if(!raise(SIGINT))
		{
			raise(SIGTERM);
		}
	}
	if(c == buf)
	{
		return true;
	} else {
		return false;
	}
}


