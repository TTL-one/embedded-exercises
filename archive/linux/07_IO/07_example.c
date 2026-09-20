/* IO操作练习
 * 要求： 处理 score.c 中的文件 补充总分和评分的内容
 */

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int read_line(int fd, char *buf, int buflen){
    char c;
    int len = 0, error = 0;
    while(1){
        error = read(fd, &c, 1);
        if(error <= 0){
            break;
        }
        if(c == '\n' || c == '\r'){
            break;
        }
        else{
            *buf++ = c;
            len++;
            buflen--;
            if(buflen <= 0){
                break;
            }
        }
    }
    *buf = '\0';

    //确实发生错误或者读到文件末尾了，并且没有读到任何数据
    if(len == 0 && error <= 0){
        error = -1;
        return error;
    }else{
        //单纯为0可能是读到\r了
        //其余情况正常
        return len;
    }
}

void process_data(char * data, char * result){
    //处理一行数据，计算总分和评分
    //数据格式：姓名,语文成绩,数学成绩,英语成绩
    //示例一:,语文,数学,英语,总分,评价 不做任何处理
    //示例二:张三,90,91,92,,处理成 张三,90,91,92,273,A
    char name[20];
    int score[3];
    char grade;
    //通过hexdump -C score.csv 可以看到文件中的内容第一行开头并不是逗号，而是不可见的字符
    //用来表示文件格式
    /*hexdump -C score.csv 
        00000000  ef bb bf 2c e8 af ad e6  96 87 2c e6 95 b0 e5 ad  |...,......,.....|
        00000010  a6 2c e8 8b b1 e8 af ad  2c e6 80 bb e5 88 86 2c  |.,......,......,|
        00000020  e8 af 84 e4 bb b7 0a e5  bc a0 e4 b8 89 2c 39 30  |.............,90|
        00000030  2c 39 31 2c 39 32 2c 2c  0a e6 9d 8e e5 9b 9b 2c  |,91,92,,.......,|
        00000040  38 30 2c 38 31 2c 38 32  2c 2c 0a e7 8e 8b e4 ba  |80,81,82,,......|
        00000050  94 2c 37 30 2c 37 31 2c  37 32 2c 2c 0a           |.,70,71,72,,.|
        0000005d
    */
    if(data[0]==0xef){
        sprintf(result,"%s",data);
    }else{
        sscanf(data,"%[^,],%d,%d,%d,",name,&score[0],&score[1],&score[2]);
        printf("name:%s, score1:%d, score2:%d, score3:%d\n", name, score[0], score[1], score[2]);
        int total = score[0] + score[1] + score[2];
        grade = (total >= 270) ? 'A' : ((total >= 240) ? 'B' : 'C');
        sprintf(result,"%s,%d,%d,%d,%d,%c", name, score[0], score[1], score[2], total, grade);
    }
}

int main(int argc,char *argv[]){
    // 创建两个文件句柄
    int targetFd = 0, resultFd = 0;

    // 判断代码操作数是否正确
    if(argc != 3){
        printf("Usage:%s <target_file> <result_file>\n", argv[0]);
        return -1;
    }

    // 参数正确，打开文件
    targetFd = open(argv[1], O_RDONLY);
    if(targetFd == -1){
        perror("open target file");
        return -1;
    }

    resultFd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0777);
    if(resultFd == -1){
        perror("open result file");
        return -1;
    }

    char buffer[1000];
    char result[1000];

    while(1){
        // 一行一行读取文件内容
        int len = read_line(targetFd, buffer, sizeof(buffer));
        if(len <= 0){
            break;
        }
        //printf("read line: %s\n", buffer);
        
        // 一行一行处理读取到的内容
        process_data(buffer,result);
        printf("result : %s\n", result);
        // 将处理的结果写入到结果文件中
        write(resultFd,result,strlen(result));
        write(resultFd,"\r\n",2);
    }
    close(targetFd);
    close(resultFd);
    return 0;
}