#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct tree_node
{
    char node_name; 
    int numChild;
    char *childrenName;
};


int total_lines;

char myId;
int Value;

struct tree_node* read_tree_file(const char *filename)
{
    int i;
    FILE *fd;
	struct tree_node *tree_ptr = (struct tree_node*)malloc(sizeof(struct tree_node) * 50);
    fd=fopen(filename,"r");
    if (fd==NULL)
	{
        printf("Error opening file \n");
        return 0;
    }
    else
	{
		int j = 0;
		char line[1024];
		char *node_name;
		char *children = NULL;
		int num_child = -1;
		while (fgets(line, sizeof(line), fd) != NULL)
		{
			char *token;
			char *temp;
			const char del[2] = " ";
			struct tree_node tree = {};
			line[strlen(line) - 1] = '\0';
			/* get the first token */
			token = strtok(line, del);
			node_name = token;
			
			if(node_name == NULL)
			{
				printf("Invalid file\n");
				exit(0);
			}
			token = strtok(NULL, del);
			if(token == NULL)
			{
				printf("Invalid file\n");
				exit(0);
			}
			num_child = atoi(token);
			tree_ptr[j].node_name = *node_name;
			tree_ptr[j].numChild = num_child;
			if (num_child == 0)
				tree_ptr[j].childrenName = NULL;
			else
			{
				children = (char*)malloc(num_child);
				if(children == NULL)
					exit(0);
				token = strtok(NULL, del);
				temp = children;
				
				/* walk through other tokens */
				while( token != NULL ) 
				{
					*temp = *token;
					token = strtok(NULL, del);
					temp++;
				}
				tree_ptr[j].childrenName = children;
			}
			
			//printf("Root Node %c : Num Of Children %d : Child Leafs : %s\n",*node_name,num_child,children);
			j++;
			total_lines = j;
		}
		fclose(fd);
    }
	return tree_ptr;
}


void createNodes(char node_name, struct tree_node *root)
{
	int curIndex,i = 0;
	//if (curIndex >= total_lines)
      //  return;

	while(i < total_lines)
	{
		if(root[i].node_name == node_name)
		{
			curIndex = i;
			break;
		}
		i++;
	}
	
    printf("(%d, %c) Pid: %d with Parent: %d and children : %s , child num : %d\n", curIndex, root[curIndex].node_name,
            getpid(), getppid(), root[curIndex].childrenName, root[curIndex].numChild);

	if(root[curIndex].numChild == 0)
		sleep(1);
    for (int i = 0; i < root[curIndex].numChild;i++)
    {
        pid_t childPid = fork();
		int fd1[2];  // Used to store two ends of first pipe
		int fd2[2];  // Used to store two ends of second pipe
        if (childPid == -1)
        {
            perror("Couldn't create process");
            exit(1);
        }
        if (childPid == 0)
        {
			printf("Spawning child %c\n",root[curIndex].childrenName[i]);
            createNodes(root[curIndex].childrenName[i], root);
        }
        else
        {
            wait(NULL);
        }
	}
	exit(0);
}


void print_tree(struct tree_node* root)
{
	int i = 0;
	while(i < total_lines)
	{
		printf("Root Node %c : Num Of Children %d : Child Leafs : %s\n",root[i].node_name,root[i].numChild,root[i].childrenName);
		i++;
	}
}


int main(int arc, char *argv[])
{
	struct tree_node *root = NULL;
	if (arc != 2)
	{
		printf("Please use this program as :\n");
		printf("./arb_tree  [input_file_path]\n");
		exit(0);
	}
	//Call function to read from file passed in command line argument
    root = read_tree_file(argv[1]);
	
	// call function to print the tree
	print_tree(root);
	
	// call function to create arbitrary process tree
    createNodes(root[0].node_name,root);
	
	
}
