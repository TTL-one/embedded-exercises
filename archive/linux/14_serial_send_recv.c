#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

/* set_opt(fd,115200,8,'N',1)
 * 参数：fd设备句柄, nSpeed波特率, nBits数据位(7/8), nEvent校验(N无/O奇/E偶), nStop停止位(1/2)
 *
 * termios 是 Linux 终端（tty）的行规程结构体，串口只是 tty 的一种。
 * 四个 flag 字段控制输入/输出/硬件/行规程的不同方面，c_cc存特殊控制字符。
 */
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	
	/* tcgetattr: 获取 fd 当前行规程配置，存入 oldtio（备份）
	 * 成功返回0，失败返回-1。必须是终端类fd，文件fd会失败 */
	if ( tcgetattr( fd,&oldtio) != 0) { 
		perror("SetupSerial 1");
		return -1;
	}
	
	/* bzero: 把 newtio 全部清零 = 默认RAW模式值 */
	bzero( &newtio, sizeof( newtio ) );

	/* ===== c_cflag: 硬件控制，串口帧格式全在这 =====
	 * CLOCAL: 忽略 Modem 控制线（CD/RI/DTR等）
	 *   串口只有TX/RX/GND三根线，没有调制解调器。
	 *   不置位：内核会等DCD(载波检测)脚有效才允许读写，程序会卡在open/read！
	 *   置位后：忽略这些握手线，靠TX/RX直接通讯。
	 * CREAD: 开启UART接收器硬件
	 *   不置位：接收器被禁止，read()永远返回0（读到EOF） */
	newtio.c_cflag |= CLOCAL | CREAD; 

	/* ~CSIZE: 清除数据位长度掩码(CS5~CS8)，为后续重新设置做准备
	 * ~CSIZE = 0b11110011，与c_cflag相与后CS位全清零 */
	newtio.c_cflag &= ~CSIZE; 

	/* ===== c_lflag: 行规程本地模式，控制输入的"加工"方式 =====
	 * ICANON: 关闭标准行模式（canonical mode）
	 *   关闭后：read()不再等'\n'才返回，按字节立即可用，RAW模式核心
	 * ECHO|ECHOE|ECHOK: 关闭回显
	 *   串口双工通讯不需要回显，否则发送的字符会被自己接收到
	 * ISIG: 关闭特殊字符转信号
	 *   否则 Ctrl+C(Ctrl+Z) 不会传给程序，会被内核转成 SIGINT/SIGTSTP */
	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/

	/* ===== c_oflag: 输出后处理 =====
	 * OPOST: 关闭输出后处理
	 *   关闭后'\n'不会再自动转换为"\r\n"，
	 *   原始字节直接发送，不经任何转换，这是RAW输出的关键 */
	newtio.c_oflag  &= ~OPOST;   /*Output*/

	/* ===== 数据位长度 ===== */
	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;  /* 7位数据位 */
	break;
	case 8:
		newtio.c_cflag |= CS8;  /* 8位数据位 */
	break;
	}

	/* ===== 奇偶校验 =====
	 * PARENB: 开启奇偶校验（与PARODD组合决定奇/偶）
	 * PARODD: 置位=奇校验(Odd)，不置位=偶校验(Even)
	 * INPCK:  启用校验检查（收到校验错误帧会通知应用层）
	 * ISTRIP: 将输入字节强制裁到7位（strip high bit） */
	switch( nEvent )
	{
	case 'O':                             /* 奇校验(Odd) */
		newtio.c_cflag |= PARENB;           /* 开启校验 */
		newtio.c_cflag |= PARODD;           /* 置位=奇校验 */
		newtio.c_iflag |= (INPCK | ISTRIP); /* 启用检查+裁成7位 */
	break;
	case 'E':                             /* 偶校验(Even) */
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;           /* 开启校验 */
		newtio.c_cflag &= ~PARODD;         /* 清除=偶校验 */
	break;
	case 'N':                             /* 无校验(None) */
		newtio.c_cflag &= ~PARENB;          /* 关闭校验 */
	break;
	}

	/* ===== 波特率 =====
	 * cfsetispeed/cfsetospeed: 仅把 speed 写入 newtio 的 c_ispeed/c_ospeed 字段
	 * 实际生效在 tcsetattr 时由内核写入 UART 硬件 */
	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
	break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
	break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
	break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
	break;
	}
	
	/* ===== 停止位 =====
	 * CSTOPB: 置位=2位停止位，不置位=1位停止位 */
	if( nStop == 1 )
		newtio.c_cflag &= ~CSTOPB;  /* 1位停止位 */
	else if ( nStop == 2 )
		newtio.c_cflag |= CSTOPB;   /* 2位停止位 */
	
	/* ===== VMIN/VTIME: 控制 read() 的阻塞行为 =====
	 * c_cc[VMIN]:  read() 阻塞到至少读到 VMIN 个字节才返回
	 * c_cc[VTIME]: 单位0.1秒，超时则返回已读数据（即使未达到VMIN）
	 *
	 * VMIN=1, VTIME=0 的含义：
	 *   - VMIN > 0 时，VTIME 被忽略，read() 会一直等到有数据为止
	 *   - 只要收到至少1个字节就立即返回
	 *   - 如果一直没有数据，read() 永远不返回（所以代码里用了while(1)）
	 *
	 * 常见组合：
	 *   VMIN=0, VTIME=0  -> read立即返回（无数据返回0）
	 *   VMIN=10, VTIME=5 -> 等待第1个字节，5×0.1=0.5秒内没数据就返回0；
	 *                       一旦读到则继续等，直到满10字节或下一个0.5秒超时 */
	newtio.c_cc[VMIN]  = 1;   /* 至少读到1字节才返回 */
	newtio.c_cc[VTIME] = 0;   /* VMIN>0时此值被忽略 */

	/* tcflush: 清空串口缓冲区
	 * TCIFLUSH: 只清输入缓冲区（在配置生效前把旧数据清掉，避免残留干扰）
	 * TCIOFLUSH: 输入输出都清
	 * TCOFLUSH: 只清输出 */
	tcflush(fd,TCIFLUSH);
	
	/* tcsetattr: 把 newtio 写入串口使配置生效
	 * TCSANOW: 立即生效（不等输出完）
	 * TCSADRAIN: 等输出缓冲区清空后再生效（适合改波特率）
	 * TCSAFLUSH: 等输出完+清输入缓冲后再生效 */
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

/* open_port: 打开串口设备文件，返回文件描述符
 * com: 设备路径，如 "/dev/ttySAC0"
 *
 * open() 的标志位解释：
 * O_RDWR   : 读写模式打开（必须，否则无法读写）
 * O_NOCTTY : 重要！不让这个设备成为"控制终端"
 *             - Linux 每个进程可以有一个控制终端（CTTY）
 *             - 如果不置此位：打开串口后，按 Ctrl+C 会发送 SIGINT 给前台进程组，
 *               read() 可能被信号打断，行为变得复杂
 *             - 置此位：把这个串口当作普通数据通道用，不当作控制终端
 *             - 串口编程必须加这个标志
 *
 * O_NDELAY : （你代码注释掉的那个）
 *             - 使 read/write 不阻塞，即使没有数据也立即返回
 *             - 0=读不到数据返回0，>0=没空间写返回-1
 *             - 等价于把 fd 设为 non-blocking
 *             - 不推荐用这个：用 VMIN/VTIME 控制阻塞行为更规范
 *
 * O_NONBLOCK : 与 O_NDELAY 功能完全相同，POSIX标准，名字更直观
 *               fcntl(fd, F_SETFL, 0) 等价于去掉 O_NONBLOCK/O_NDELAY
 */
int open_port(char *com)
{
	int fd;
	/* O_RDWR|O_NOCTTY  : 打开为读写+不做控制终端 */
	/* O_RDWR|O_NOCTTY|O_NDELAY : 注释掉的版本 = 加上 non-blocking 模式 */
	fd = open(com, O_RDWR|O_NOCTTY);
    if (-1 == fd){
		return(-1);
    }

	/* fcntl: File Control，操作 fd 的文件状态标志
	 * F_SETFL: 设置文件状态标志
	 * 参数0  : 实际是 O_flags & ~O_NONBLOCK & ~O_NDELAY
	 *          即：清除 O_NONBLOCK 和 O_NDELAY，把 fd 设为"阻塞模式"
	 *
	 * 为什么先 open 再 fcntl？
	 *   因为 open() 的 non-blocking 标志（O_NONBLOCK）是直接写在 open 参数里的
	 *   open 本身没有"只开阻塞"的快捷方式
	 *   所以用 open 打开后，再用 fcntl 显式把阻塞标志写回去
	 *   这跟你注释掉的 O_NDELAY 效果相反——这里是"显式设为阻塞"
	 *
	 * 注意：fcntl 能修改的标志有限，只能改 O_APPEND/O_NONBLOCK/O_DSYNC/
	 *       O_RSYNC/O_SYNC/O_NOATIME 这几个，其他标志（如 O_RDWR）改不了
	 */
	if(fcntl(fd, F_SETFL, 0)<0)  /* 设置文件描述符为阻塞状态（清除O_NONBLOCK） */
	{
		printf("fcntl failed!\n");
		return -1;
	}

	return fd;
}


/*
 * ./serial_send_recv <dev>
 */
int main(int argc, char **argv)
{
	int fd;
	int iRet;
	char c;

	/* 1. open */

	/* 2. setup 
	 * 115200,8N1
	 * RAW mode
	 * return data immediately
	 */

	/* 3. write and read */
	
	if (argc != 2)
	{
		printf("Usage: \n");
		printf("%s </dev/ttySAC1 or other>\n", argv[0]);
		return -1;
	}

	fd = open_port(argv[1]);
	if (fd < 0)
	{
		printf("open %s err!\n", argv[1]);
		return -1;
	}

	iRet = set_opt(fd, 115200, 8, 'N', 1);
	if (iRet)
	{
		printf("set port err!\n");
		return -1;
	}

	printf("Enter a char: ");
	while (1)
	{
		scanf("%c", &c);
		iRet = write(fd, &c, 1);
		iRet = read(fd, &c, 1);
		if (iRet == 1)
			printf("get: %02x %c\n", c, c);
		else
			printf("can not get data\n");
	}

	return 0;
}

