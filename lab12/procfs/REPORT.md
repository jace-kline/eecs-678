# EECS 678 - Lab 12 Report
### Author: Jace Kline 2881618

## Questions:

### Question 1:
Is lack of change in system and user time in between sampling periods a guarantee that deadlock has occurred? Explain briefly.

A lack of change in system and user time between sampling periods indicates that deadlock has most likely occurred, but not necessarily. A lack of progress (time change) in either system or user time means for a philosopher thread that a particular thread was not scheduled at all, most likely because it was waiting on a chopstick mutex. We consider the program "deadlocked" if all child threads have made no time progress (haven't been scheduled) between times when the main thread checks again. If the main thread goes through an entire sleep iteration to wait for the philosopher threads to think and eat, and none of the philosopher (child) threads have been scheduled, then we make the assumption that the program is in a deadlocked state. However, if there are lots of different processes running currently on the CPU and the sleep period is short enough in the main thread's loop, then it is technically possible for the main thread to check for deadlock too early and possibly detecting a false deadlock.

### Question 2:
What aspects of the system conditions would affect how long the sampling period should be to ensure a reliable assessment of whether deadlock has occurred or not.

If the CPU of our process is very busy with lots of other CPU-intensive processes running, then we must increase the sampling period to ensure that the philosopher (child) threads of our process have all had sufficient time to run. On the other hand, if the CPU is not busy, then we can decrease the sampling period because the CPU has more time to devote to our process. Another factor to consider is the clock speed of the CPU. A faster clock speed would also mean we could decrease the sampling period of the main thread.

### Question 3:
Informal experimentation tends to show that larger values of ACTIVE_DURATION make deadlock less likely, as indicated by how many sampling periods it takes to occur, and that smaller values make it more likely. Try a few different values yourself and then discuss whether you think this is true, and why you think it might have the influence you observe.

After my own observations, the claim appears to be true. By increasing the value of ACTIVE_DURATION, the program will complete many more iterations before deadlock occurs (on average). My reasoning on why this occurs it that when you increase the ACTIVE_DURATION time, it increases the amount of time that each philosopher spends thinking, and therefore not wanting to pick up the chopsticks. With longer thinking periods for each philosopher, the overlapped time of them trying to pick up each of the chopsticks is reduced, and therefore the probability of them getting into deadlock goes down. On the contrary, if the ACTIVE_DURATION is decreased, then each philosopher attempts to access the shared resources (chopstick mutexes) at a faster rate and with more percentage of their scheduled time. This implies that there is more time overlap of philosophers attempting to pick up chopsticks, leading to a quicker deadlock.