#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


//execve()
//替换当前进程的代码段和数据段,但保留原进程的PID和文件描述符
int main(int argc, char const *argv[])
{
    char* name = "banzhang";
    printf("我是%s 编号%d,我现在还在1楼\n",name,getpid());
    
    char* args[] = {"erlou",name,NULL}; //参数数组,以NULL结尾
    char* envs[] ={NULL}; //环境变量数组

    //1.执行程序的路径,程序的物理位置
    //2.传入的参数,相当于main中的argv[]
    //3.传递的环境变量
    //return 成功根本没办法返回 失败返回-1
    int res = execve(args[0],args,envs);
    if(res == -1){
        perror("execve");
        return 1;
    }
    return 0;
}
