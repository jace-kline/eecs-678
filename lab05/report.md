# EECS 678 - Lab 5 Report
* Name: Jace Kline
* KUID: 2881618

## Question Answers:
1. For signals such as KILL and STOP, it is important that they are handled directly by the operating system to ensure that some process does not overwrite the signal handler functionality of these unique signals. The KILL and STOP signals must not be overwritten because they are the last line of defense to terminate a "rogue" program. If the programmer were allowed to overwrite these, there would be no guarantees regarding the termination of programs. This would imply that the only way to know a process is terminated would be to shut down the machine. Hence, it is extremely important that the KILL and STOP signals are handled directly by the operating system.
2. There are two main reasons why the pause() system call is superior to using an infinite loop. The first reason is that pause() will block sequential execution only until there is any signal received, but then program execution will continue as normal. Secondly, the return value of pause() indicates the type of signal received. Therefore, we can add logic following a pause() call to get more fine-tuned behavior out of out signal handling. 
