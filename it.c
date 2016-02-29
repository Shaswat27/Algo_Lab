//Considering interval as both sides closed

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct li
{
	int k;
	struct li* next;
} list;

typedef struct i
{
	int a;
	int b;
	list* x;
	struct i* left;
	struct i* right;
} node;

node* CREATE_INTERVAL_TREE(int l, int u, int n)
{
	if(n == 0)
	{
		node* p = NULL;
		return p;
	}	

	int size = (u-l+1)/n;

	int median;
	if(n%2==1)	
		median = l + size*( (n-1)/2 );
	else
		median = l + size*( (n)/2 );

	node* root = (node *)malloc(sizeof(node));
	root->a = median;
	root->b = median + (size-1);
	root->x = NULL;
	root->left = NULL;
	root->right = NULL;
	
	if(n%2 == 1)
	{
		root->left = CREATE_INTERVAL_TREE(l, median-1, n/2);
		root->right = CREATE_INTERVAL_TREE((root->b+1), u, n/2);
	}
	else
	{
		root->left = CREATE_INTERVAL_TREE(l, median-1, n/2);
		root->right = CREATE_INTERVAL_TREE((root->b+1), u, n/2-1);
	}	

	
		
	return root;	
}

void INSERT(node* T, int i)
{
	if(T==NULL) return;
		
	if( i>=(T->a) && i<=(T->b) ) //if it lies in this interval
	{
		if(T->x == NULL) //first list element
		{
			T->x = (list *)malloc(sizeof(list));
			T->x->k = i;
			T->x->next = NULL;
		}
		else
		{
			list* insert = (list *)malloc(sizeof(list));		
			insert->k = i;
			insert->next = NULL;	
			
			list* p = (T->x);
			
			if((p)->k>i) ///first element is greater
			{
				insert->next = p;	
				T->x = insert;
				return;
			}
			
			while( (p)->next != NULL ) 
			{
					if((p)->next->k > i)
						break;				
					(p)=(p)->next;
			}
			//p now points to the place of insertion
			insert->next = (p)->next;
			(p)->next = insert;			
		}
		return;
	}	
	else if( i<(T->a) )
		INSERT(T->left, i);
	else if( i>(T->b) )
		INSERT(T->right, i);	
}	


void list_delete(list** head, list* n)
{
	//When node to be deleted is head node
    if(*head == n)
    {
        if((*head)->next == NULL)
        {
            *head = NULL;
            return;
        }
 
        (*head)->k = (*head)->next->k;
 
        // store address of next node
        n = (*head)->next;
 
        // Remove the link of next node
        (*head)->next = (*head)->next->next;
 
        // free memory
        free(n); 
        return;
    }
 
 
    //When not first node, follow the normal deletion process 
    //find the previous node
    list *prev = *head;
    while(prev->next != NULL && prev->next != n)
        prev = prev->next;
 
    // Check if node really exists in Linked List
    if(prev->next == NULL)
    {
        return;
    }
    // Remove node from Linked List
    prev->next = prev->next->next;
    // Free memory
    free(n);
    return; 
}

void updateLL(node** root, node** start)
{
	if(*start == NULL) return;

	//printf("Start - %d\n", (*start)->a);

	updateLL(root, &((*start)->left));

	list* p;
	p = (*start)->x;
	int i = 0;
	while(p!=NULL)
	{
		//printf("Put - %d\n", p->k);
		INSERT(*root, p->k);
		p = p->next;
		i++;
	}

	updateLL(root, &((*start)->right));
}	


int flag = 0;
node** tree;
node** sub_root;
node** lower;
node** upper;
int max_u = -32767;
int min_l = 32768;
void MERGE(node* T, int l, int u)
{
	//static int max_u = -32767;
	//static int min_l = 32768;

	if(T == NULL) return;

	//printf("T: [%d, %d]\n", T->a, T->b);

	if( (T->a)>u ) //no intersection 
	{
		MERGE(T->left, l, u);

		if(flag && T!=*tree)
		{
			if(T->b > max_u) max_u = T->b;
			if(T->a < min_l) min_l = T->a;

			if(T->a>=(*upper)->a)
			{
				//printf("T->a>=(*upper)->a\n");
				(*upper)->right = T->right;

				
				updateLL(upper, &(T->left));

				//(*upper)->x = T->x;

			}
			else
			{
				//printf("T->a<(*upper)->a\n");
				(*upper)->right = T;
			}	
		}
	}

	if( (T->b)<l ) //no intersection
	{
		MERGE(T->right, l, u);

		if(flag && T!=*tree)
		{
			
			if(T->b > max_u) max_u = T->b;
		    if(T->a < min_l) min_l = T->a;

			if(T->a>=(*lower)->a)
			{
				//printf("T->a>=(*lower)->a\n");
				(*lower)->left = T->left;
				//(*lower)->x = T->x;
				
				updateLL(lower, &(T->right));

				//
			}
			else
			{
				//printf("T->a<(*lower)->a\n");
				(*lower)->left = T;
			}
		}
	}

	//if we find an intersection
	if(!(l>(T->b) || u<(T->a)) )
	{
		//printf("Found intersection!\n");
		if(!flag) //if we've found an intersection for the first time
		{
			sub_root = &T; 
			(*sub_root)->a = l;
			(*sub_root)->b = u;
			//printf("Sub_root -> %d\n", (*sub_root)->x->k);
			flag = 1;
		}

		//case 1 - T is completely completely inside [l,u]
		if( l<(T->a) && (T->b)<u)
		{
			if(T->b > max_u) max_u = T->b;
			if(T->a < min_l) min_l = T->a;
			//printf("Case1\n");
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while(*p!=NULL)
				{
					//printf("Insert %d\n", (*p)->k);
					INSERT(*sub_root, (*p)->k);
					//printf("Inserted %d\n", (*sub_root)->x->k);
					list_delete(head, *p);
					if(*p!=NULL)
						*p = (*p)->next;
				}
			}
			//if(T->left != NULL)
				MERGE(T->left, l, u);
			//if(T->right != NULL)
				MERGE(T->right, l, u);
		}

		//case 2 - T's upper bound is inside
		else if( l<=(T->b) && l>=(T->a))
		{
			//printf("Case2\n");
			if(T->a < min_l) min_l = T->a;
			if(T->b > max_u) max_u = T->b;
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while((*p)!=NULL)
				{
					if((*p)->k>=l)
					{
						//printf("Insert %d\n", (*p)->k);
						INSERT(*sub_root, (*p)->k);
						//printf("Inserted: ");
						list* q = (*sub_root)->x;
						while(q!=NULL)
						{
							//printf(" %d", q->k);
							q = q->next;
						}
						//printf("\n");
						list_delete(head, *p);
						//printf("p = %d\n", (*p)->k);
						//if((*p)->next == NULL) (*head) = ;
					}
					if((*p)!=NULL)
						if((*p)->k<l )
						{
							//printf("Insert to lower %d\n", (*p)->k);
							INSERT(*lower, (*p)->k);
													
						}
					if((*p)!=NULL)
					*p = (*p)->next;	
				}					
			}
			//printf("Case 2 finished, move to next node\n");
			//if(T->right != NULL)
				MERGE(T->left, l, u);
				MERGE(T->right, l, u);
			//else return;
		}

		//case 3 - T's lower bound is inside
		else if( u>=(T->a) && u<=(T->b))
		{
			//printf("Case3\n");
			if(T->a < min_l) min_l = T->a;
			if(T->b > max_u) max_u = T->b;
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				while((*p)!=NULL)
				{
					if((*p)->k<=u)
					{
						//printf("Insert %d\n", (*p)->k);
						INSERT(*sub_root, (*p)->k);
						//printf("Inserted: ");
						list* q = (*sub_root)->x;
						while(q!=NULL)
						{
							printf(" %d", q->k);
							q = q->next;
						}
						printf("\n");
						list_delete(head, *p);
						//printf("p = %d\n", (*p)->k);
						//if((*p)->next == NULL) (*head) = (*p);
					}
					if((*p)!=NULL)
						if((*p)->k>u)
						{
							//printf("Insert to upper %d\n", (*p)->k);
							INSERT(*upper, (*p)->k);
							
						}
					*p = (*p)->next;		
				}					
			}
			MERGE(T->left, l, u);
			MERGE(T->right, l, u);
		}
		
		//case 4 - [l,u] is completely inside T: [T->a, T->b]
		else if( l>(T->a) && (T->b)>u)
		{
			//printf("Case4\n");
			if(T->b > max_u) max_u = T->b;
			if(T->a < min_l) min_l = T->a;
			if(*sub_root!=T) //if we are not at the subroot, insert all elemnts to 
			{
				list** head = &(T->x);
				list** p = &(T->x);
				if((*p)->k<=u && (*p)->k>=l)
					{
						//printf("Insert %d\n", (*p)->k);
						INSERT(*sub_root, (*p)->k);
						//printf("Inserted: ");
						list* q = (*sub_root)->x;
						while(q!=NULL)
						{
							//printf(" %d", q->k);
							q = q->next;
						}
						//printf("\n");
						list_delete(head, *p);
						//printf("p = %d\n", (*p)->k);
						//if((*p)->next == NULL) (*head) = (*p);
					}
					if((*p)!=NULL)
					{
						if((*p)->k>u)
						{
							//printf("Insert to upper %d\n", (*p)->k);
							INSERT(*upper, (*p)->k);
							*p = (*p)->next;
						}
						if((*p)->k<l)
						{
							//printf("Insert to lower %d\n", (*p)->k);
							INSERT(*lower, (*p)->k);
							
						}
						*p = (*p)->next;
					}
			}
			//if(T->left != NULL)
				MERGE(T->left, l, u);
			//if(T->right != NULL)
				MERGE(T->right, l, u);
		}


	}	
	//printf("Main loop ends with T = [%d, %d]\n", T->a, T->b);

	if(T == *tree)
	{
		
		//printf("Subroot - [%d, %d]\n", (*sub_root)->a, (*sub_root)->b);
		if(min_l!= 32768 && l-1<l)
		{
			(*lower)->a = min_l;
			(*lower)->b = l-1;

			if( (*lower)->a <= (*lower)->b )
				(*sub_root)->left = *lower;
			else
				(*sub_root)->left = NULL;
		}
		if(max_u != -32767 && max_u<u)
		{
			(*upper)->a = u+1;
			(*upper)->b = max_u;
			if( (*upper)->a <= (*upper)->b )
			(*sub_root)->right = *upper;
			else
				(*sub_root)->right = NULL;
		}
		//(*sub_root)->left = *lower;
		//(*sub_root)->right = *upper;
	}
		
}	

node** first;
void PRETTY_PRINT(node* Tree)
{
	static int h = -1;
	static int rt = 0;

	if(Tree == NULL) {return;}

	if(!rt)
	{
		first = &Tree;
		node* p = Tree;
		while(p!=NULL)
		{
			h++;
			p=p->left;
    	}
		rt = 1;
	}

	if(Tree->left!=NULL)
	{
		h=h-1;
		PRETTY_PRINT(Tree->left);
	}
	int i=0;
	for(;i<h;i++)
		printf("\t");
	printf("[%d,%d] ", Tree->a, Tree->b);
	list* p = Tree->x;
	while(p != NULL)
	{
		if(p->next == NULL)
			printf("%d", p->k);
		else
			printf("%d, ", p->k);
		p = p->next;
	}
	printf("\n");

	if(Tree->right!=NULL)
	{
		h=h-1;
		PRETTY_PRINT(Tree->right);	
	}
	h=h+1;
}	

int main()
{
	node* root;

	int l, u, n, i, m;

	printf("Enter l: ");
	scanf("%d", &l);
	printf("Enter u: ");
	scanf("%d", &u);
	printf("Enter n: ");
	scanf("%d", &n);

	root = CREATE_INTERVAL_TREE(l,u,n);

	node* root1 = root;

	printf("Enter i: ");
	scanf("%d", &i);

	int j=0;
	for(; j<i; j++)
		INSERT(root, l + rand()%(u-l+1) );

	PRETTY_PRINT(root);

	printf("Enter m: ");
	scanf("%d", &m);

	j = 0;
	int size = (u-l+1)/m;
	for(; j<=m/2; j++)
	{
		flag = 0;
		node* y = (node *)malloc(sizeof(node));
		node* z = (node *)malloc(sizeof(node));

		node* sr = (node*)malloc(sizeof(node));

		sub_root = &sr;
		lower = &y;
		upper = &z;

		(sr)->a = -32767;
		(sr)->b = 32768;
		(sr)->x = NULL;

		(*lower)->a = -32767; (*lower)->b = 32768; (*lower)->x = NULL; (*lower)->left = NULL; (*lower)->right = NULL;
		*upper = (node *)malloc(sizeof(node));
		(*upper)->a = -32767; (*upper)->b = 32768; (*upper)->x = NULL; (*upper)->left = NULL; (*upper)->right = NULL;

		tree = &root;

		printf("\nMerge [%d, %d]\n", l+(m/2-j)*size, l+(m/2-j)*size+(size-1) );
		int a = l+(m/2-j)*size;
		int b = l+(m/2-j)*size+(size-1);
		MERGE(root, a, b);
		PRETTY_PRINT(root);
		printf("\n");

		if(j)
		{printf("Merge [%d, %d]\n", l+(m/2+j)*size, l+(m/2+j)*size+(size-1) );
		a = l+(m/2+j)*size;
		b = l+(m/2+j)*size+(size-1);

		flag = 0;
		y = (node *)malloc(sizeof(node));
		z = (node *)malloc(sizeof(node));

		sr = (node*)malloc(sizeof(node));

		sub_root = &sr;
		lower = &y;
		upper = &z;

		(sr)->a = -32767;
		(sr)->b = 32768;
		(sr)->x = NULL;

		(*lower)->a = -32767; (*lower)->b = 32768; (*lower)->x = NULL; (*lower)->left = NULL; (*lower)->right = NULL;
		*upper = (node *)malloc(sizeof(node));
		(*upper)->a = -32767; (*upper)->b = 32768; (*upper)->x = NULL; (*upper)->left = NULL; (*upper)->right = NULL;

		tree = &root;
		MERGE(root, a, b);
		PRETTY_PRINT(root);}
		printf("\n");
	}

	//PRETTY_PRINT(root);

	printf("\n");
	return 0;
}