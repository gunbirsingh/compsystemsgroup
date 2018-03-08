#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <algorithm>
#include <csignal>

using namespace std;

typedef struct node node;

// struct to hold node data
struct node {
	string name;
	vector<node> children;
	int waiting;
	pid_t pid;
};

// the root of the tree
node root;

// This function explains the status of a pid
void explain_wait_status(pid_t pid, int status){
	if (WIFEXITED(status))
	{
		fprintf(stderr, "Child with PID = %ld was temintaed correctly, exit status = %d\n", (long)pid, WEXITSTATUS(status));

	}
	else if (WIFSIGNALED(status))
	{
		fprintf(stderr, "Child with PID = %d was teminated by a ignal, signo = %d\n", pid, WTERMSIG(status));	
	}
	else if (WIFSTOPPED(status))
	{
		fprintf(stderr, "Child with PID = %d has been stopped by a signal, signo %d\n", pid, WSTOPSIG(status));
	}
	else
	{
		fprintf(stderr, "%s: Internal error: Unhandled case, PID = %d. status = %d\n", __func__, pid, status );
		exit(1);
	}
	fflush(stderr);
}
// This function waits for all of the execution of a child to run
void wait_for_children(node z){
	if (z.children.size() == 0)
	{
		explain_wait_status(z.pid, z.waiting);
	}
	else{
		exit(0);
	}
}

// this build the process tree recursively in a depth first fashion
void exec_child(node z) {
	cout << "node " << z.name << "\tpid=" << getpid() << "\t";
	// store the PIDs of child processes
	vector<pid_t> pids;
	// Set that the signal is waiting
	z.waiting = 1;
	// if this is a leaf node, set the node as done
	if (z.children.size() == 0) {
		z.waiting = 0; // The deepest child process has been reached
		if (z.waiting == false)
		{
		cout << "leaf" << endl;
		sleep(1);
		cout << "node " << z.name << " done" << endl;
		exit(0);		
		}	
	} 
		else {
		cout << z.children.size() << " children" << endl;
		}
	// fork and recurse for each child node
	for (node c : z.children) {
		cout << z.name << " forking for " << c.name << endl;
		pids.push_back(fork());
		if (pids.back()) {
			exec_child(c);	
		} else {
			continue;
			exit(0);
		}
	}
	// print the PIDs of the child processes
	cout << "node " << z.name << " waiting for: ";
	for (pid_t pid : pids) {
		cout << pid << ", ";
	}
	cout << endl;
	// wait for all the child processes to exit
	while (wait(0) > 0);
	cout << "node " << z.name << " done" << endl;
}

// this prints the node tree recursively
void print(int i, node z) {
	for (int j=0; j < i; ++j) {
		cout << "\t";
	}
	cout << n.name << endl;
	for (node c : z.children) {
		print(i+1, c);
	}
}

// this inserts new nodes into the tree
bool insert(node& a, node z) {
	if (a.name == z.name) {
		a = z;
		return true;
	}
	for (node &c : r.children) {
		if (insert(c, n)) return true;
	}
	return false;
}

int main(int argc, char** argv) {
	// check input arguments
	if (argc != 2) {
		cout << "Usage: ./proctree FILENAME\n";
		exit(-1);
	}

	ifstream tree_file(argv[1]);
	string line;
	vector<string> lines;
	while (getline(tree_file, line)) {
		lines.push_back(line);
	}
	tree_file.close();

	istringstream iss;
	string tok;
	int num_children;

	for (string line : lines) {
		node t1;

		iss = istringstream(line);

		getline(iss, tok, ' ');
		t1.name = tok;

		getline(iss, tok, ' ');
		num_children = atoi(tok.c_str());

		while (getline(iss, tok, ' ')) {
			node t2;
			t2.name = tok;
			t1.children.push_back(t2);
		}
		if (root.name == "") {
			root = t1;
		} else if (!insert(root, t1)) {
			cout << "Insert failed" << endl;
			exit(-1);
		}
	}
	print(0, root);
	exec_child(root);
}