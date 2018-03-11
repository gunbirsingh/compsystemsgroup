#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <csignal>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

typedef struct node tree;

// struct to hold node data
struct node {
	string name;
	vector<tree> children;
	int wait;
	pid_t pid;
};

// the root of the tree
tree root;

// This function explains the status of a pid
void wait_status(pid_t pid, int status){
	if (WIFEXITED(status))
	{
		fprintf(stderr, "Child with PID: %ld was temintaed correctly, exit status = %d\n", (long)pid, WEXITSTATUS(status));

	}
	else if (WIFSIGNALED(status))
	{
		fprintf(stderr, "Child with PID: %d was teminated by a ignal, signo = %d\n", pid, WTERMSIG(status));	
	}
	else if (WIFSTOPPED(status))
	{
		fprintf(stderr, "Child with PID: %d has been stopped by a signal, signo %d\n", pid, WSTOPSIG(status));
	}
	else
	{
		fprintf(stderr, "%s: Internal error: Unhandled case, PID: %d. Status: %d\n", __func__, pid, status );
		exit(1);
	}
	fflush(stderr);
}
// this inserts new nodes into the tree
bool insert_proc(tree& g, tree f) {
	if (g.name == f.name) {
		g=  f;
		return true;
	}
	for (tree &g : f.children) {
		if (insert_proc(g, f)) return true;
	}
	return false;
}

// This function waits for all of the execution of a child to run
void child_wait_finish(tree f){
	if (f.children.size() == 0)
	{
		wait_status(f.pid, f.wait);
	}
	else{
		exit(0);
	}
}


// this build the process tree recursively in a depth first fashion
void build_tree(tree f) {
	cout << "node " << f.name << "\tpid=" << getpid() << "\t";
	// store the PIDs of child processes
	vector<pid_t> pids;
	// Set that the signal is waiting
	f.wait = 1;
	// if this is a leaf node, set the node as done
	if (f.children.size() == 0) {
		f.wait = 0; // The deepest child process has been reached
		if (f.wait == false)
		{
		cout << "leaf" << endl;
		sleep(1);
		cout << "node " << f.name << " done" << endl;
		exit(0);		
		}	
	} 
		else {
		cout << f.children.size() << " children" << endl;
		}

	// fork and recurse for each child node
	for (tree g : f.children) {
		cout << f.name << " forking for " << g.name << endl;
		pids.push_back(fork());
		if (pids.back()) {
			build_tree(g);	
		} else {
			continue;
			exit(0);
		}
	}

	// print the PIDs of the child processes
	cout << "node " << f.name << " waiting for: ";
	for (pid_t pid : pids) {
		cout << pid << ", ";
	}
	cout << endl;
	// wait for all the child processes to exit
	while (wait(0) > 0);
	cout << "node " << f.name << " done" << endl;
}

// this prints the node tree recursively
void view(int x, tree f) {
	for (int y=0; y < x; y++) {
		cout << "\t";
	}
	cout << f.name << endl;
	for (tree g : f.children) {
		view(x+1, g);
	}
}


int main(int a, char** b) {
	// check input arguments
	if (a != 2) {
		cout << "Usage: ./proctree FILENAME\n";
		exit(-1);
	}

	ifstream file(b[1]);
	string line;
	vector<string> lines;
	while (getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	istringstream stream;
	string tok;
	int num_children;

	for (string line : lines) {
		tree t1;

		stream = istringstream(line);

		getline(stream, tok, ' ');
		t1.name = tok;

		getline(stream, tok, ' ');
		num_children = atoi(tok.c_str());

		while (getline(stream, tok, ' ')) {
			tree t2;
			t2.name = tok;
			t1.children.push_back(t2);
		}
		if (root.name == "") {
			root = t1;
		} else if (!insert_proc(root, t1)) {
			cout << "Insert failed" << endl;
			exit(-1);
		}
	}
	view(0, root);
	build_tree(root);
}