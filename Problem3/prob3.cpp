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

struct node {
	string name;
	vector<tree> children;
	int wait;
	pid_t pid;
};

tree root;

void wait_status(pid_t pid, int status){
	if (WIFEXITED(status))
	{
		fprintf(stderr, "Child with PID ID: %ld was ended correctly, exit status = %d\n", (long)pid, WEXITSTATUS(status));

	}
	else if (WIFSIGNALED(status))
	{
		fprintf(stderr, "Child with PID ID: %d was ended by a signal, signal = %d\n", pid, WTERMSIG(status));	
	}
	else if (WIFSTOPPED(status))
	{
		fprintf(stderr, "Child with PID ID: %d was ended by a signal, signal =  %d\n", pid, WSTOPSIG(status));
	}
	else
	{
		fprintf(stderr, "%s: Internal error: Unhandled case, PID: %d. Status: %d\n", __func__, pid, status );
		exit(1);
	}
	fflush(stderr);
}
void build_tree(tree f) {
	cout << "node " << f.name << "\tpid=" << getpid() << "\t";
	vector<pid_t> pids;
	f.wait = 1;
	if (f.children.size() == 0) {
		f.wait = 0; 
		if (f.wait == false)
		{
		cout << "This is a leaf" << endl;
		sleep(1);
		cout << "Node ID:" << f.name << " is done" << endl;
		exit(0);		
		}	
	} 
		else {
		cout << f.children.size() << " children" << endl;
		}

	for (tree g : f.children) {
		cout << "Node ID:" << f.name << " is forking for Node ID:" << g.name << endl;
		pids.push_back(fork());
		if (pids.back()) {
			build_tree(g);	
		} else {
			continue;
			exit(0);
		}
	}

	cout << "Node ID" << f.name << " is waiting for: ";
	for (pid_t pid : pids) {
		cout << pid << ", ";
	}
	cout << endl;
	while (wait(0) > 0);
	cout << "Node ID" << f.name << " is done" << endl;
}

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

void wait_for_children(tree f){
	if (f.children.size() == 0)
	{
		wait_status(f.pid, f.wait);
	}
	else{
		exit(0);
	}
}


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
	if (a != 2) {
		cout << "\n";
		exit(-1);
	}

	string line;
	vector<string> lines;
	ifstream file(b[1]);
	
	while (getline(file, line)) {
		lines.push_back(line);
	}
	file.close();

	istringstream stream;
	string manz;
	int num_children;

	for (string line : lines) {
		tree node1;

		stream = istringstream(line);

		getline(stream, manz, ' ');
		node1.name = manz;

		getline(stream, manz, ' ');
		num_children = atoi(manz.c_str());

		while (getline(stream, manz, ' ')) {
			tree node2;
			node2.name = manz;
			node1.children.push_back(node2);
		}
		if (root.name == "") {
			root = node1;
		} else if (!insert_proc(root, node1)) {
			cout << "Inserting node failed" << endl;
			exit(-1);
		}
	}
	view(0, root);
	build_tree(root);
}