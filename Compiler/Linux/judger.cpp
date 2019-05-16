#include <stdio.h>  
#include <sys/resource.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h> 
#include <errno.h>
#include <functional>
#include <signal.h>
#include <signal.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h> 
#include <time.h> 
#include <string>
#include <sys/stat.h>  
#include <fcntl.h>  

//创建子进程，阻塞地运行指令
 void run_command(char* program_path,char* program,char* arg1=NULL, char* arg2=NULL, 
 	char* arg3=NULL,char* arg4=NULL,char* arg5=NULL,char* arg6=NULL,char* arg7=NULL,
 	char* arg8=NULL,char* arg9=NULL,char* arg10=NULL,char* arg11=NULL,char* arg12=NULL)
 {
 	pid_t sub_pid=fork();
 	if(sub_pid<0)
 	{
 		printf("fork error\n");
 	}
 	else if(sub_pid==0)
 	{// in child process
 		if(execlp(program_path,program,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11,arg12,NULL)==-1)
 		{
 			printf("execlp error\n");
 		}
 	}
 	// parent process
	waitpid(sub_pid,NULL,0);
 } 

//获取一个进程的最大虚拟内存使用率
int get_peek_memory(pid_t pid)
{
	char command[100];  
	sprintf(command,R"(cat /proc/%d/status | grep -Po '(?<=VmPeak:)\s+\S+')",pid);
	FILE* file = popen(command,"r");
	if(file==NULL)
	{
		printf("failed to popen\n"); 
		return -1;
	} 
	char buffer[100]="0";
	fread(buffer, sizeof(buffer), 1, file); 
	pclose(file); 
	printf("buff %s\n",buffer );
	return atoi(buffer); 
}

//获取进程的内存
int get_memory(pid_t pid)
{ 
	// system("ps au");
	// printf("%d\n",pid );
	char command[100];
	sprintf(command,R"(ps -au | grep -Po '^root\s*%d\s*\S*\s*\S*\s*\S\s*\S*' | grep -Po '\S+$')",pid);
	FILE* file = popen(command,"r");
	if(file==NULL)
	{
		printf("failed to popen\n"); 
		return -1;
	} 
	char buffer[100]="0";
	fread(buffer, sizeof(buffer), 1, file); 
	pclose(file); 
	// printf("buff %s\n",buffer );
	return atoi(buffer); 
}

//获取进程的内存
int get_memory(const char* program_command)
{ 
	// system("ps au"); 
	char command[100];  
	sprintf(command,R"(ps -au | grep '^.*%s$'  | grep -Po '^root\s*\S*\s*\S*\s*\S*\s*\S\s*\S*' | grep -Po '\S+$')",program_command);
	// printf("%s\n", command);
	FILE* file = popen(command,"r");
	if(file==NULL)
	{
		printf("failed to popen\n"); 
		return -1;
	} 
	char buffer[100]="0";
	fread(buffer, sizeof(buffer), 1, file); 
	pclose(file); 
	// printf("buff %s\n",buffer );
	return atoi(buffer); 
}

//获取当前进程的内存
int get_current_process_memory()
{
	get_memory(getpid());
}

//获取当前进程的PID值
pid_t get_pid_by_program(const char* program_command)
{
	char command[100];  
	sprintf(command,R"(ps -au | grep '^.*%s$'  | grep -Po '^root\s+\S+' | grep -Po '\S+$')",program_command);
	// printf("%s\n", command);
	FILE* file = popen(command,"r");
	if(file==NULL)
	{
		printf("failed to popen\n"); 
		return -1;
	} 
	char buffer[100]="0";
	fread(buffer, sizeof(buffer), 1, file); 
	pclose(file);  
	return atoi(buffer); 
}

//结束进程
void kill_program(pid_t pid)
{ 
	kill(pid,SIGKILL);
}

//结束进程
void kill_program(const char* program)
{
	pid_t pid=get_pid_by_program(program);
	if(pid==0)
	{
		printf("program not find: %s\n",program);
		return;
	}
	kill(pid,SIGKILL);
}

//创建子进程，阻塞地运行函数
 void run_function(std::function<void(void)> fun)
 {
 	pid_t sub_pid=fork();
 	if(sub_pid<0)
 	{
 		printf("fork error\n");
 	}
 	else if(sub_pid==0)
 	{// in child process
 		fun();
 	}
 	// parent process
	waitpid(sub_pid,NULL,0);
 }

//设置CPU运行时间限制
void set_cpu_limit(int sec)
{
	rlimit limit;
    limit.rlim_cur = sec; //SIGXCPU
    limit.rlim_max = sec+2; //SIGKILL
    if(setrlimit(RLIMIT_CPU, &limit) == -1)
    { 
    	printf("set rlimit error\n");
    }
}

//设置文件打开数目限制
void set_file_limit(int number=3)
{
	rlimit limit;
    limit.rlim_cur = number; 
    limit.rlim_max = number;  
    if(setrlimit(RLIMIT_NOFILE, &limit) == -1)
    { 
    	printf("set set file limit error\n");
    }
}

//设置用户进程数目限制
void set_fork_limit(int number=1)
{
	rlimit limit;
    limit.rlim_cur = number; 
    limit.rlim_max = number+1;  
    if(setrlimit(RLIMIT_NPROC, &limit) == -1)
    { 
    	printf("set set fork limit error\n");
    }
}

//设置进程内存限制
void set_memory_limit(double MB)
{ 
	int bytes = static_cast<int>(MB*1024*1024);
	rlimit limit;
    limit.rlim_cur = bytes;
    limit.rlim_max = 2*1024*1024*1024; //MAX 2G
    if(setrlimit(RLIMIT_AS, &limit) == -1)
    { 
    	printf("set rlimit error\n");
    }
}

//设置进程默认限制
void set_default_limit()
{
	set_file_limit();
	set_fork_limit();
}

//设置CPU超时信号处理函数
void cpu_signal_handle(int signal_no)
{
	// run_command("ps","ps","aux");
	// system("ps aux | grep a.out");
	printf("receive cpu signal\n");
	exit(1);
} 

//设置进程运行时错误处理函数
void runtime_error_handle(int signal_no)
{
	printf("runtime error\n");
	exit(2);
}

//设置一系列进程信号处理函数
void set_signal_handle()
{ 
	 signal(SIGXCPU, cpu_signal_handle); // out of time
	 signal(SIGSEGV,runtime_error_handle);//invalid memory access
	 signal(SIGFPE,runtime_error_handle);// mathmatical error
	 signal(SIGABRT,runtime_error_handle);// abort error 
}

void main_end_fun()
{
	printf("main end\n");
} 

//计时器调用函数
void timer_handler(int signal_no)
{
	static int i=0;
	printf("timer count:%d memory:%d\n",++i,get_memory(getpid()));
}

//设置计时器
void set_timer()
{
	struct itimerval tv;

    signal(SIGPROF,timer_handler);

    tv.it_value.tv_sec = 0;
    tv.it_value.tv_usec = 10 * 1000L;
 
    tv.it_interval.tv_sec = 0;
    tv.it_interval.tv_usec = 10 * 1000L;

    if(setitimer(ITIMER_PROF, &tv, NULL) != 0)
    {
        printf("settimer error\n");
    }
}

//打开、读取、关闭文件
std::string read_file(const char* path)
{ 
	FILE *file=fopen(path,"r+" );
	if(file==NULL)
	{
		printf("cannot find file: %s\n",path);
		exit(0);
	}
	size_t size = 0;
	fseek(file,0L,SEEK_END);
	size=ftell(file);
	fseek(file,0L,SEEK_SET); 
	char *buff =(char*)malloc(size+1);
	fread(buff,1,size,file);
	buff[size]='\0';
	std::string ret(buff);
	free(buff);
	return ret; 
}

//测试功能
void test()
{
	printf("in test\n");
	pid_t pid = getpid();
	printf("%d\n",get_memory(pid)); 
	malloc(200*1024*1024);
	printf("%d\n",get_memory(pid)); 
	// pr_limits("RLIMIT_CPU",RLIMIT_CPU);  
	set_signal_handle();
	set_cpu_limit(1);
	// set_memory_limit(10);
	// pr_limits("RLIMIT_CPU",RLIMIT_CPU);
	// run_command("ps","ps","aux");
	// system("ps au | grep a.out");
	// system(command);
	if(malloc(200*1024*1024)==NULL)
	{
		// printf("%d\n",get_memory(pid)); 
	}
	else
	{
		printf("malloc success\n");
	} 
	// printf("%d\n",get_memory(pid)); 
	while(true);
} 

//编译结果枚举
enum class CompileResult:int
{
	OK,
	TimeLimitExceeded,
	Failed,
};

//编译src文件，将编译结果保存在des文件中
CompileResult compile(const char* src, const char* des, const std::string& language)
{
 	volatile bool compile_time_exceed=true;// add volatile to prevent undefined behavior
 	volatile bool compiler_success=false;//  add volatile to prevent undefined behavior
 	pid_t pid = vfork();
 	if(pid<0)
 	{
 		printf("fork error\n");
 	}
 	else if(pid==0)
 	{// in child process  
 		char command[1024];
 		if(language=="CPP")
 		{
			sprintf(command,"g++ %s -pipe -static -lm -s -x c++ -std=c++11 -O2 -o %s 2>compiler_error_info",src,des);
 		}
 		else if(language=="C")
 		{
 			sprintf(command,"gcc %s -pipe -static -lm -O2 -o %s 2>compiler_error_info",src,des);
 			
 		}else if(language=="JAVA")
 		{
 			sprintf(command,"javac %s 2>compiler_error_info",src,des); 
 		}
 		else
 		{
 			printf("unkown language compile command: %s\n",language.c_str());
 			exit(1);
 		}
 		set_cpu_limit(3);// set comile time limit
 		int result=system(command);
 		compile_time_exceed=false;//in time 
 		if(result==-1)
 		{
 			printf("system error, command: %s\n",command);
 		}
 		else 
 		{
 			if(WIFEXITED(result))
 			{ 
 				if(WEXITSTATUS(result)!=0)
 				{// compile failed
 					compiler_success=false;
 				}
 				else
 				{ 
 					compiler_success=true;
 				}
 			}
 			else
 			{
 				printf("unkown compile error, exit code: %d\n",WEXITSTATUS(result)); 
 			}
 		}
 		exit(0);
 	}
 	// parent process
	waitpid(pid,NULL,0); 
	if(compile_time_exceed)
	{
		return CompileResult::TimeLimitExceeded;
	}
	else if(!compiler_success)
	{
		return CompileResult::Failed;
	}
	return CompileResult::OK;
}


//运行结果结构体
struct RunResult
{
	enum class ResultType:int
	{
		OK,
		RuntimeError,
		TimeLimitExceeded,
		MemoryLimitExceeded,
	}; 
	ResultType type= ResultType::OK;
	int time_cost=0;
	int memory_cost=0;
};

//运行program程序，设置时间与内存限制
RunResult run(const char* program,int time_limit,int memory_limit, const std::string& language)
{
	volatile bool run_time_error=false;//  add volatile to prevent undefined behavior
	volatile bool time_exceeded=false;//  add volatile to prevent undefined behavior
	volatile bool memory_exceeeded=false;//  add volatile to prevent undefined behavior
	volatile int memory_cost=0;//  add volatile to prevent undefined behavior
	volatile int time_cost=0;//  add volatile to prevent undefined behavior

	pid_t pid = vfork();
 	if(pid<0)
 	{
 		printf("fork error\n");
 	}
 	else if(pid==0)
 	{// in child process  
 		// char command[1024];
 		// if(language=="CPP"||language=="C")
 		// { 
 		// 	sprintf(command,"%s <std_input >std_output 2>&1",program);
 		// }
 		// else if(language=="JAVA")
 		// { 
 		// 	sprintf(command,"java %s <std_input >std_output 2>&1",program);
 		// }
 		// set_timer();
 		pid_t sub_pid=fork();
		struct  timeval start;
        gettimeofday(&start,NULL); 
 		if(sub_pid<0)
 		{ 
 			printf("fork error\n");
 		}
 		else if(sub_pid==0)
 		{
 			// set_cpu_limit(3);// set comile time limit
 			// execlp("/bin/sh", "sh", "-c", command,NULL);
 			// redirect input/output/error stream
 			int fin = open("/data/std_input",(O_RDONLY | O_CREAT), 0644);
 			int fout = open("/data/std_output",(O_WRONLY | O_CREAT|O_TRUNC), 0644);
 			int ferr = open("/data/std_error",(O_WRONLY | O_CREAT|O_TRUNC), 0644);
 			dup2(fin,STDIN_FILENO);
 			dup2(fout,STDOUT_FILENO);
 			dup2(ferr,STDERR_FILENO);
 			set_default_limit();
 			if(language=="CPP"||language=="C")
 			{ 
 				execlp(program,NULL);
 			}
 			else if(language=="JAVA")
 			{ 
 				execlp("java",program,NULL);
 			}
 			else
 			{
 				printf("unkown language run: %s\n",language.c_str());
 				exit(1);
 			}
 			_exit(127);// exit wehen execlp failed
 		} 
 		int result;
        struct  timeval end; 
        int memory=0;
 		while(waitpid(sub_pid, &result, WNOHANG)==0)
 		{
        	// memory=get_memory(sub_pid);
        	memory=get_memory(sub_pid);
 			// printf("peek memory: %d, memory: %d\n",get_peek_memory(sub_pid),memory);
        	if(memory>memory_cost)
        	{
        		memory_cost=memory;
        	}
        	gettimeofday(&end,NULL);
        	time_cost = 1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;

        	if(memory_cost>memory_limit)
        	{
        		memory_exceeeded=true; 
        	    kill_program(sub_pid);
 				exit(0);
        	}
        	if(time_cost>time_limit)
        	{
        		time_exceeded=true;
        	    kill_program(sub_pid);
 				exit(0);
        	}
        	usleep(15);
 		} 
 		 
 		if(result==-1)
 		{
 			printf("system error, program to run: %s\n",program);
 		}
 		else 
 		{
 			if(WIFEXITED(result))
 			{ 
 				run_time_error = (WEXITSTATUS(result)!=0); 
 			}
 			else
 			{
 				run_time_error=true;
 				//printf("run error, exit code: %d\n",WEXITSTATUS(result)); 
 			}
 		} 
 		exit(0);
 	} 
	waitpid(pid,NULL,0); 
	RunResult result;
	result.time_cost = time_cost;
	result.memory_cost = memory_cost; 
	if(time_exceeded)
	{
		result.type=RunResult::ResultType::TimeLimitExceeded; 
	} 
	else if(memory_exceeeded)
	{ 
		result.type=RunResult::ResultType::MemoryLimitExceeded; 
	}
	else if(run_time_error)
	{ 
		result.type=RunResult::ResultType::RuntimeError; 
	}
	else
	{
		result.type=RunResult::ResultType::OK;
		// printf("run status ok\n");
	}
	return result;
}

//标准化字符串内容
void standard(std::string& str)
{
	int cur=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]!='\r')
		{
			str[cur++]=str[i];
		}
	}
	cur--;
	while(cur>=0&&
		(str[cur]==' '||str[cur]=='\t'||str[cur]=='\n'))
	{
		cur--;
	}
	str.erase(cur+1);
}

//去除比较字符串中的空白符号
void soft(std::string& str)
{
	int cur=0;
	for(int i=0;i<str.size();i++)
	{
		if(str[i]!=' '&&str[i]!='\t'&&str[i]!='\n')
		{
			str[cur++]=str[i];
		}
	} 
	str.erase(cur);   
}

//比较结果枚举
enum CompareResult
{
	OK,
	WrongAnswer,
	PresentationError,
};

//比较两个字符串
CompareResult compare(const char* sample_file, const char* user_file)
{
	std::string sample=read_file(sample_file);
	std::string user=read_file(user_file);
 	standard(sample);
 	standard(user);

 	if(sample==user)
 	{
 		return CompareResult::OK;
 	}
 
 	soft(sample);   
 	soft(user);

 	if(sample==user)
 	{
 		return CompareResult::PresentationError;
 	}
 	return CompareResult::WrongAnswer;
}

//裁判结果结构体
struct JudgeResult
{
	enum class ResultType:int
	{
		Accept,
	    WrongAnswer,
	    PresentationError,
	    TimeLimitExceeded,
	    OutputLimitExceeded,
	    MemoryLimitExceeded,
	    RuntimeError,
	    CompileError,
	    CompileTimeLimitExceeded,
	    JudgerInnerError,
	};
	ResultType type=ResultType::JudgerInnerError;
	int time_cost=0;
	int memory_cost=0;
};

// time_limit: msec, memory_limimt: kb
JudgeResult judge(int time_limit,int memory_limit,const std::string& language,bool need_compile)
{
	JudgeResult result;
	if(need_compile)
	{
		CompileResult compiler_result;
		if(language=="CPP")
		{
			compiler_result = compile("/data/program.cpp","/data/program.out",language);
		}
		else if(language=="C")
		{
			compiler_result = compile("/data/program.c","/data/program.out",language);
		}
		else if(language=="JAVA")
		{
			compiler_result = compile("/data/Program.java","/data/Program.class",language);
		}
		else
		{
			printf("unkown language to judge: %s\n",language.c_str());
			exit(1);
		}
		switch(compiler_result)
		{
			case CompileResult::TimeLimitExceeded:
			result.type=JudgeResult::ResultType::CompileTimeLimitExceeded;
			return result;
			break;
			case CompileResult::Failed: 
			result.type=JudgeResult::ResultType::CompileError;
			return result;
			break;
			case CompileResult::OK:
			break;
			default:
			break;
		}	 
	}

	RunResult run_result;;// asumme path
	if(language=="CPP")
	{
		run_result = run("/data/program.out",time_limit,memory_limit,language);
	}
	else if(language=="C")
	{
		run_result = run("/data/program.out",time_limit,memory_limit,language);
	}
	else if(language=="JAVA")
	{
		run_result = run("/data/program.class",time_limit,memory_limit,language);
	}
	else
	{
		printf("unkown language to judge: %s\n",language.c_str());
		exit(1);
	}
	result.time_cost=run_result.time_cost;
	result.memory_cost=run_result.memory_cost;

	switch(run_result.type)
	{
		case RunResult::ResultType::RuntimeError:
		result.type=JudgeResult::ResultType::RuntimeError;
		return result;
		break;
		case RunResult::ResultType::TimeLimitExceeded:
		result.type=JudgeResult::ResultType::TimeLimitExceeded;
		return result;
		break;
		case RunResult::ResultType::MemoryLimitExceeded:
		result.type=JudgeResult::ResultType::MemoryLimitExceeded;
		return result;
		break; 
		case RunResult::ResultType::OK:
		break;
		default:
		break;
	}

	CompareResult compare_result=compare("/data/sample","/data/std_output");//assume path
	switch(compare_result)
	{ 
		case CompareResult::WrongAnswer:
		result.type=JudgeResult::ResultType::WrongAnswer;
		return result;
		break; 
		case CompareResult::PresentationError:
		result.type=JudgeResult::ResultType::PresentationError;
		return result;
		break;
		case CompareResult::OK:
		break;
		default:
		break;
	}
	result.type=JudgeResult::ResultType::Accept;
	return result;

}

//输出裁判结果
void output_judge_result(JudgeResult result)
{
	printf("time: %d\nmemory: %d\n", result.time_cost,result.memory_cost);
	switch(result.type)
	{
		case JudgeResult::ResultType::Accept:
		printf("result: %s\n", "Accept");
		break; 
		case JudgeResult::ResultType::WrongAnswer:
		printf("result: %s\n", "WrongAnswer");
		break;
		case JudgeResult::ResultType::PresentationError:
		printf("result: %s\n", "PresentationError");
		break;
		case JudgeResult::ResultType::TimeLimitExceeded:
		printf("result: %s\n", "TimeLimitExceeded");
		break;
		case JudgeResult::ResultType::OutputLimitExceeded:
		printf("result: %s\n", "OutputLimitExceeded");
		break;
		case JudgeResult::ResultType::MemoryLimitExceeded:
		printf("result: %s\n", "MemoryLimitExceeded");
		break;
		case JudgeResult::ResultType::RuntimeError:
		printf("result: %s\n", "RuntimeError");
		break;
		case JudgeResult::ResultType::CompileError:
		printf("result: %s\n", "CompileError");
		break;
		case JudgeResult::ResultType::CompileTimeLimitExceeded:
		printf("result: %s\n", "CompileTimeLimitExceeded");
		break;
		case JudgeResult::ResultType::JudgerInnerError:
		printf("result: %s\n", "JudgerInnerError");
		break; 
		default:
		printf("result: %s\n", "JudgerInnerError(default)"); 
		break;
	} 
}

//初始化参数
void init_arg(int argc,char* argv[],int& time_limit,int& memory_limit, std::string& language,bool& need_compile)
{
	if(argc!=5)
	{
		printf("argc is not coorrent: %d\n",argc);
	}

	time_limit=atoi(argv[1]);
	memory_limit=atoi(argv[2]);
	language=std::string(argv[3]);
	need_compile=atoi(argv[4]);

	if(time_limit<=0)
	{
		printf("time_limit argument error: %d\n",time_limit);
		exit(1);
	}
	if(time_limit<=0)
	{
		printf("time_limit argument error: %d\n",time_limit);
		exit(1);
	}
	if(language.size()==0)
	{
		printf("language argument is empty");
		if(language!="CPP"&&language!="C"&&language!="JAVA")
		{
			printf("unkown language to init: %s\n",language.c_str());
		}
		exit(1);
	}
}



// 运行参数：time_limit(ms),memory_limit(kb),language(CPP|C|JAVA),need_compile(0|1)
int main(int argc, char* argv[])
{
	int time_limit=0;
	int memory_limit=0;
	bool need_compile=false;
	std::string language;
	init_arg(argc,argv,time_limit,memory_limit,language,need_compile);
	// printf("time_limit: %d,memory_limit: %d,language: %s,need_compile: %d\n",time_limit,memory_limit,language.c_str(),need_compile );
	JudgeResult	result = judge(time_limit,memory_limit,language,need_compile);
	output_judge_result(result);
} 