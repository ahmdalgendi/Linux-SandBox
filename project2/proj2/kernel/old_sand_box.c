#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/errno.h>
#include<linux/sched.h>
#include <linux/uaccess.h>
#include <linux/cred.h>
#include<linux/mutex.h>
#include<linux/spinlock.h>

#define __NR_syscall_max 437

/* An AVL tree node 
*/
struct SandBox_AVL
{
	
	pid_t  process_id;

	int count;
	struct SandBox_AVL *left;
	struct SandBox_AVL *right;
	int height;
};



int get_height(struct SandBox_AVL *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

/*  get maximum of two integers 
*/
int max_(int a, int b)
{
	return (a > b) ? a : b;
}



struct SandBox_AVL* init_sand_box_node(pid_t process_id)
{
	struct SandBox_AVL* node;
	/*TODO:
*/
	node = (struct SandBox_AVL*) kmalloc(sizeof(struct SandBox_AVL), GFP_KERNEL);
	node->process_id = process_id;
	node->left = NULL;
	node->right = NULL;
	node->height = 1; /* new node is initially added at leaf 
*/
	node->count = 0;
	return(node);
}


struct SandBox_AVL *rightRotate(struct SandBox_AVL *y)
{
	struct SandBox_AVL *x ;
	struct SandBox_AVL *T2;
	
	x = y->left;
	T2 = x->right;

	/* Perform rotation*/
	x->right = y;
	y->left = T2;

	/* Update heights\*/
	y->height = max_(get_height(y->left), get_height(y->right)) + 1;
	x->height = max_(get_height(x->left), get_height(x->right)) + 1;

	/* Return new root*/
	return x;
}

/* A utility function to left rotate subtree rooted with x 
*/
/* See the diagram given above. 
*/
struct SandBox_AVL *leftRotate(struct SandBox_AVL *x)
{
	struct SandBox_AVL *y ;
	struct SandBox_AVL *T2;
	y = x->right;
	T2 = y->left;

	/* Perform rotation 
*/
	y->left = x;
	x->right = T2;

	/* Update heights 
*/
	x->height = max_(get_height(x->left), get_height(x->right)) + 1;
	y->height = max_(get_height(y->left), get_height(y->right)) + 1;

	/* Return new root 
*/
	return y;
}

int getBalance(struct SandBox_AVL *N)
{
	if (N == NULL)
		return 0;
	return get_height(N->left) - get_height(N->right);
}

struct SandBox_AVL* insert(struct SandBox_AVL* node, pid_t process_id)
{
	int balance;
	if (node == NULL)
		return(init_sand_box_node(process_id));

	if (process_id < node->process_id)
		node->left = insert(node->left, process_id);
	else if (process_id > node->process_id)
		node->right = insert(node->right, process_id);
	else /* process already inserted
*/
		return node;

	node->height = 1 + max_(get_height(node->left),
		get_height(node->right));

	
	balance = getBalance(node);

	/* If this node becomes unbalanced, then there are 4 cases 
*/

	/* Left Left Case 
*/
	if (balance > 1 && process_id < node->left->process_id)
		return rightRotate(node);

	/* Right Right Case 
*/
	if (balance < -1 && process_id > node->right->process_id)
		return leftRotate(node);

	/* Left Right Case 
*/
	if (balance > 1 && process_id > node->left->process_id)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	/* Right Left Case 
*/
	if (balance < -1 && process_id < node->right->process_id)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	/* return the (unchanged) node pointer*/
	return node;
}

struct SandBox_AVL * minValueNode(struct SandBox_AVL* node)
{
	struct SandBox_AVL* curr;

	if (node == NULL)
		return  NULL;

	curr = node;

	/* loop down to find the leftmost leaf*/
	while (curr->left != NULL)
		curr = curr->left;

	return curr;
}

/*TODO:*/
struct SandBox_AVL* deleteNode(struct SandBox_AVL* root, pid_t process_id)
{

	struct SandBox_AVL *temp;
	int balance;
	if (root == NULL)
		return root;

	if (process_id < root->process_id)
		root->left = deleteNode(root->left, process_id);


	else if (process_id > root->process_id)
		root->right = deleteNode(root->right, process_id);

	else
	{

		if ((root->left == NULL) || (root->right == NULL))
		{
			temp = root->left ? root->left :
				root->right;

			if (temp == NULL)
			{
				temp = root;
				root = NULL;
			}
			else
			{
				*root = *temp; /* Copy the contents of*/
								/* the non-empty child*/
			}
			kfree(temp);
		}
		else
		{

			 temp = minValueNode(root->right);


			root->process_id = temp->process_id;
			root->count = temp->count;
			/* Delete the inorder successor */
			root->right = deleteNode(root->right, temp->process_id);
		}
	}

	
	if (root == NULL)
		return root;

	/* UPDATE HEIGHT OF THE CURRENT NODE */
	root->height = 1 + max_(get_height(root->left),
		get_height(root->right));

	/* STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to */
	/* check whether this node became unbalanced) */
	 balance = getBalance(root);

	/* If this node becomes unbalanced, then rotate*/

	/* Left Left Case */
	if (balance > 1 && getBalance(root->left) >= 0)
		return rightRotate(root);

	/* Left Right Case */
	if (balance > 1 && getBalance(root->left) < 0)
	{
		root->left = leftRotate(root->left);
		return rightRotate(root);
	}

	/* Right Right Case */
	if (balance < -1 && getBalance(root->right) <= 0)
		return leftRotate(root);

	/* Right Left Case */
	if (balance < -1 && getBalance(root->right) > 0)
	{
		root->right = rightRotate(root->right);
		return leftRotate(root);
	}

	return root;
}

/* A utility function to print preorder traversal of 
*/
/* the tree. 
*/
/* The function also prints height of every node 
*/
void preOrder(struct SandBox_AVL *root)
{
	if (root != NULL)
	{
		printk("%d ", root->process_id);
		preOrder(root->left);
		preOrder(root->right);
	}
}

struct SandBox_AVL * find_and_return(struct SandBox_AVL * root, pid_t process_id)
{
	if (root == NULL)
		return NULL;
	if (root->process_id == process_id)
		return root;
	if (root->process_id > process_id)
		return find_and_return(root->left, process_id);

	return find_and_return(root->right, process_id);
}

struct SandBox_AVL * syscalls_arr[__NR_syscall_max];

void init_sand_box(void)
{
	int i;
	for ( i = 0; i < __NR_syscall_max; ++i)
	{
		syscalls_arr[i] = 0;
	}
}

int is_blocked(int nr , int sys_call)
{
	struct SandBox_AVL* temp;
	if (sys_call >= __NR_syscall_max)
		return  -1; /* invalid parameters
*/
	temp = find_and_return(syscalls_arr[sys_call], nr);
	if (temp == NULL)
		return 0;
	temp->count++; /* increase the number of clicks in 
*/
	return 1;
}

int block_process(int nr , int sys_call)
{

	struct SandBox_AVL* temp;
	if (sys_call >= __NR_syscall_max)
		return  -1; /* invalid parameters
*/
	temp = find_and_return(syscalls_arr[sys_call], nr);
	if (temp != NULL)
		return 0; /* already blocked
*/
	syscalls_arr[sys_call] = insert(syscalls_arr[sys_call], nr);
	return 1;
}

int unblock_process(int nr , int sys_call)
{
	struct SandBox_AVL* temp;
	if (sys_call >= __NR_syscall_max)
		return  -1; /* invalid parameters
*/

	temp = find_and_return(syscalls_arr[sys_call], nr);
	if (temp == NULL)
		return 0; /* not blocked
*/
	syscalls_arr[sys_call] = deleteNode(syscalls_arr[sys_call], nr);
	return 1; /* deleted
*/
}


/*
Blocks the process identified by proc from accessing the system call identified by nr. 
If proc is non-zero, this system call requires root privileges to run. 
If proc is zero the current process id 
(of the process running the system call) is used instead and any user may use the system call. 
That is to say that a process may always block itself from making a particular system call, 
but may not block any other process unless root is running it. 
Returns 0 on success or an appropriate error code on failure.

long sbx421_block(pid_t proc, unsigned long nr)
*/
SYSCALL_DEFINE2(sbx421_block, pid_t , proc , unsigned long ,nr)
{
	printk("sbx421_block\n");
	return 0;
}

/*
Unblocks the process identified by proc from accessing the system call identified by nr. 
This system call requires root privileges to run and the proc argument must be non-zero. 
If the specified process is not blocking the system call given, then an error should be returned. 
Returns 0 on success or an appropriate error code on failure.    

long sbx421_unblock(pid_t proc, unsigned long nr) 
*/

SYSCALL_DEFINE2(sbx421_unblock, pid_t , proc , unsigned long ,nr)
{
	printk("sbx421_unblock\n");
	return 0;
}


/*Returns the number of times the process identified by the PID proc attempted to run the blocked system call nr.
 This system call requires root privileges to run and the proc argument must be non-zero. 
 If the system call specified is not blocked by the specified process, 
 returns an appropriate error code,
  otherwise returns the 0 or positive access count.

long sbx421_count(pid_t proc, unsigned long nr) 

*/
SYSCALL_DEFINE2(sbx421_count, pid_t , proc , unsigned long ,nr)
{
	printk("sbx421_count\n");
	return 0;
}
