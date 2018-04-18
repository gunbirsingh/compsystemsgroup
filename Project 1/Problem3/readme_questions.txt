Problem 3:


For this problem, the first major decision I made was to write the code in c++ as supposed to simply c. This decision was made because c++ allows for the use of vectors which simplified a ton when working with the tree structure and storing the pids of the processes. This program was built off the work done by Rajan on problem 2 so the node and tree structures I used were the same as the ones he defined with some minor additions. For example, I added a status to the node class so I could properly signal between the parents and children and notify one or the other when a process had fully completed. 


Questions:
1: When using the sleep() function, the programmer is restricted to time and he has no idea how long the process might take, it is basically a blind guess as to how much time the program should wait for. When using signals, the program execution will only happen when the signals are sent and received. The main difference between using sleep() and signals is level of control the programmer has. 
2: The wait_for_children() function has the responsibility of waiting for all of the children processes of a parent to finish before the parent dies. One of the main benefits of this function is that it prevents any zombie children from spawning, and the program losing resources.