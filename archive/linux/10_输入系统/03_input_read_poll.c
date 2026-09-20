
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>


/* ./01_get_input_info /dev/input/event0 */
int main(int argc, char **argv)
{
	int fd;
	int err;
	int len;
	int ret;
	int i;
	unsigned char byte;
	int bit;
	struct input_id id;
	unsigned int evbit[2];
	struct input_event event;
	struct pollfd fds[2];
	nfds_t nfds = 2;
	
	char *ev_names[] = {
		"EV_SYN ",
		"EV_KEY ",
		"EV_REL ",
		"EV_ABS ",
		"EV_MSC ",
		"EV_SW	",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"NULL ",
		"EV_LED ",
		"EV_SND ",
		"NULL ",
		"EV_REP ",
		"EV_FF	",
		"EV_PWR ",
		};
	
	if (argc != 2)
	{
		printf("Usage: %s <dev>\n", argv[0]);
		return -1;
	}

	fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (fd < 0)
	{
		printf("open %s err\n", argv[1]);
		return -1;
	}

	int fd2 = open(argv[2], O_RDWR | O_NONBLOCK);
	if (fd2 < 0)
	{
		printf("open %s err\n", argv[2]);
		return -1;
	}

	while (1)
	{
		fds[0].fd = fd;
		fds[0].events  = POLLIN;
		fds[0].revents = 0;

		fds[1].fd = fd2;
		fds[1].events  = POLLIN;
		fds[1].revents = 0;

		
		ret = poll(fds, nfds, 5000);
		if (ret > 0)
		{
			if (fds[0].revents == POLLIN)
			{
				while (read(fd, &event, sizeof(event)) == sizeof(event))
				{
					printf("fd get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
				}
			}
			if(fds[1].revents == POLLIN){
				while(read(fd2,&event,sizeof(event)) == sizeof(event)){
					printf("fd2 get event: type = 0x%x, code = 0x%x, value = 0x%x\n", event.type, event.code, event.value);
				}	
			}
		}
		else if (ret == 0)
		{
			printf("time out\n");
		}
		else
		{
			printf("poll err\n");
		}
	}

	return 0;
}

