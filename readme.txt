
Compile the program: gcc   problem2.c
Run it as :  ./a.out   data.txt

Note:  In data.txt file , keep the last line as blank.

Question: What is the order of appearance of the start and termination messages from the 
processes and why?

Ans. The order of start and termination messages is random, because the underlying processes print the message as per cpu time slot given by the cpu scheduler.
