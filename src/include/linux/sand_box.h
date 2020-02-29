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
#include "linux/audit.h"
#ifndef _SBX_421_
#define _SBX_421_



// #define NR_syscalls 437
// #define NR_syscalls NR_syscalls

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



int get_height(struct SandBox_AVL *N);

/*  get maximum of two integers
*/


struct SandBox_AVL* init_sand_box_node(pid_t process_id);

struct SandBox_AVL *rightRotate(struct SandBox_AVL *y);

/* A utility function to left rotate subtree rooted with x
*/
/* See the diagram given above.
*/
struct SandBox_AVL *leftRotate(struct SandBox_AVL *x);

int getBalance(struct SandBox_AVL *N);

struct SandBox_AVL* insert(struct SandBox_AVL* node, pid_t process_id);


struct SandBox_AVL * minValueNode(struct SandBox_AVL* node);



struct SandBox_AVL* deleteNode(struct SandBox_AVL* root, pid_t process_id);


/* A utility function to print preorder traversal of
*/
/* the tree.
*/
/* The function also prints height of every node
*/
void preOrder(struct SandBox_AVL *root);

struct SandBox_AVL * find_and_return(struct SandBox_AVL * root, pid_t process_id);

void init_sand_box(void);

int is_blocked(unsigned long nr, pid_t roc_id);

int block_process(unsigned long nr, pid_t roc_id);

int unblock_process(unsigned long nr, pid_t roc_id);

// extern struct SandBox_AVL * syscalls_arr[NR_syscalls];


#endif