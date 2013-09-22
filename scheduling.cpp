/*
// scheduling.cpp : Defines the entry point for the console application.
// scheduling:
//
//
*/
#include "stdafx.h"
#include "Primary_Unit.h"
#include "Secondary_Unit.h"
#define num_pu 3 //set number of pus
#define num_cu 21 //set number of cus
#define loop_count 100 //set loop:how many times to iterate
 //use vector<bit> for bitwise memory allocation. even bool takes 1 byte
    int cus_pu[num_cu]={0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2}; //edit to set cu's pu in order cu0,cu1,cu2 ...
//int cus_pu[num_cu]={0,1,2,0,0,0,1,2,0,1}; //edit to set cu's pu in order cu0,cu1,cu2 ...
	int pu_range[num_pu]={100,100,100}; //edit to set pu range
	int weight_multiplier=1; //adjust weight by setting this variable to any value,set 10,100 or any value

	int zero_counter[num_pu];
	bool blacklist[num_cu]; //one round black list to block more neghbour request assignment,stores true/false in inorder cu0,cu1,cu2
// variable for sequence 0000111000111000111000
	int rnum=0; 
    bool sw_state =0;
//deletion counter 
	int del_success=0;
	int del_unsuccess=0;
//object array
	Primary_Unit pu[num_pu];
	Secondary_Unit cu[num_cu];
	
	void set_pu();
	void set_cu();
	void loop();
	void PU_State(void);
	void CU_Request(void);
//controller array
	int controller[num_cu*4];
	void set_controller(void);
	void controller_update();
//request queue
	void cu_insert(struct node **front, struct node **rear, int value);
	void cu_assign(struct node **front, struct node **rear, int *value);
	void cu_fetch(struct node **front, struct node **rear, int *value, int front_check); //FIFO
	void request_queue();
	void make_request(int cu_id);
	void accept_request(int front_check);
//sorting cu id on the basis of weight for entry in request queue
	void weight_sort();
    void bsort(float w_list[],int id_list[], int n);
    void swap_id(int *x,int *y);
	void swap_wt(float *x,float *y);
	//euclidean
	void euclidean_dist(int i,int j);
struct node
{
   int data;
   struct node *link;
};

  struct node *front=NULL,*rear = NULL;
  int n,value;

int _tmain(int argc, _TCHAR* argv[])
{
	set_pu();
	set_cu();
	set_controller();
	printf("controller names : cu.id cu.pu pu.state cu.request\n");
	loop();
	//Primary_Unit *pu1=new Primary_Unit(); pu1->id
	return 0;
}
void loop(){
//start loop 100 times. write code inside this loop. scroll the cmd to see all values
 for(int i=0;i<loop_count;i++)
 {
	 PU_State();
	 CU_Request();
	for(int j=0;j<num_pu;j++)
	 {
	// printf("random state of primary unit %d is: %d\n",j, pu[j].state);
	//printf("random request by Secondary unit %d is: %d\n",j ,cu[j].request);
		 if(pu[j].state==0)
		  {
			  //increment the corresponding pu zero counter
	       zero_counter[j]+=1;
		   //write code to assign cu requests from request_queue here
		   
	      }
		 
	  }
	//reset blacklist
	for(int j=0;j<num_cu;j++)
	{
	blacklist[j]=0;
	}
	controller_update();
 }
	for(int j=0;j<num_pu;j++)
	{
	  printf("Primary Unit %d is IDLE at number of times: %d\n",j,zero_counter[j]);
	}

	printf("Deletion Successful: %d\n Deletion Unsuccessful: %d\n",del_success,del_unsuccess);

	scanf("%d",&pu[0].id);//just to pause the console
}
void set_pu()
{
	for(int j=0;j<num_pu;j++)
	{
	pu[j].id=j;
	pu[j].state=1;
	pu[j].range=pu_range[j];
	pu[j].x=rand()%10;
	pu[j].y=rand()%10;
	}
}
void set_cu()
{	
	for(int j=0;j<num_cu;j++)
	{
	cu[j].id=j;
	cu[j].pu=cus_pu[j];
	cu[j].request=0;
	cu[j].x=rand()%10;
	cu[j].y=rand()%10;
	euclidean_dist(j,cu[j].pu);
	}
}
void PU_State(void)
{

	//code for no sequence
	// update pu state
	for(int j=0;j<num_pu;j++)
	{
	pu[j].state=rand()%2;
	}
	/*
//code to get sequeunce like this pu0's sequence: 00001110011100011000111000
//attention: this sequence will work only if there is one pu,if used in case 
//of more than one pu then all pu will be 1 for many rounds and then zero
if(rnum==0)
{
rnum = 1+rand()%4;
sw_state =!sw_state;
for(int j=0;j<num_pu;j++)
	{
     pu[j].state=sw_state;
    }
}
else
{
	for(int j=0;j<num_pu;j++)
	{
     pu[j].state=sw_state;
	}
rnum--;
}
*/

}
void CU_Request(void)
{

	//new code to consider weight(distance) of cu to decide whether to make request or not
	for(int j=0;j<num_cu;j++)
	{
			//cu[j].request=rand()%2;
		if(cu[j].weight<pu[cu[j].pu].range)
		{
			cu[j].request=rand()%2;
		}
		else
		{
		cu[j].request=0;
		}	
	}
}
//controller array initial value setting
//format: cu.id, cu.pu, pu.state, cu.request
void set_controller(void)
{
    for(int i=0;i<num_cu*4;i=i+4)
	{	
	controller[i]=cu[i/4].id;
	controller[i+1]=cu[i/4].pu;
	controller[i+2]=1;
	controller[i+3]=cu[i/4].request;
	//printf("debug1: controller values %d %d %d %d\n",controller[i],controller[i+1],controller[i+2],controller[i+3]);
	}
	
}
void controller_update()
{
		 
	for(int i=2;i<num_cu*4;i=i+4)
	{		
	 controller[i]=pu[controller[i-1]].state; 
	 controller[i+1]=cu[controller[i-2]].request;
	 
	 printf("controller values: %d \t %d \t %d\t %d\n",controller[i-2],controller[i-1],controller[i],controller[i+1]);
	/*
		//call make_request if cu request is 1	
	 if(controller[i+1]==1)
	 {
	  make_request(controller[i-2]); //this cu.id will be inserted into FIFO request queue
	 }
	 // assign request if pu is idle i.e. state=0
	 if(controller[i]==0 && blacklist[(i-2)/4]==0)
	 {
		 
	  accept_request(controller[i-2]); //pass to check whether the request is made by the appropriate cu,which is i-2th entry here
	 }
	 */
	}
	weight_sort();

	for(int i=2;i<num_cu*4;i=i+4)
	{
	 if(controller[i]==0 && blacklist[(i-2)/4]==0)
	 {		 
	  accept_request(controller[i-2]); //pass to check whether the request is made by the appropriate cu,which is i-2th entry here
	 }
	}
	
}
void weight_sort()
{
	int id_list[num_cu],n=0;
	float w_list[num_cu];
	for(int i=0;i<num_cu*4;i=i+4)
	{
	 if(controller[i+3]==1)
	 {		 
		 id_list[n]=controller[i];
		 w_list[n]=cu[i/4].weight;
		 n++;
	 }
	}
	bsort(w_list,id_list,n);
	for(int i=0;i<n;i++)
	{
		//printf("sorted id_list cu element %d is %d\n",i, id_list[i]);
       make_request(id_list[i]); //this cu.id will be inserted into FIFO request queue
	}

}
void swap_wt(float *x,float *y)
{
   float temp;
   temp = *x;
   *x = *y;
   *y = temp;
}
void swap_id(int *x,int *y)
{
   int temp;
   temp = *x;
   *x = *y;
   *y = temp;
}
void bsort(float w_list[],int id_list[], int n)
{
   int i,j;
   for(i=0;i<(n-1);i++)
      for(j=0;j<(n-(i+1));j++)
             if(w_list[j] < w_list[j+1])
			 {
                    swap_wt(&w_list[j],&w_list[j+1]);
					swap_id(&id_list[j],&id_list[j+1]);
			 }
}
//call from set_cu to set cu.pu,by calculating weights
void euclidean_dist(int i,int j)
{	
	cu[i].weight = weight_multiplier/sqrt((float)(cu[i].x-pu[i].x)*(cu[i].x-pu[i].x) + (cu[i].y-pu[i].y)*(cu[i].y-pu[i].y));
    printf("weight of cu %d is %f \n",i,cu[i].weight);
}
// request queue starts :dynamic memory allocation, linked list implementation of queue

void cu_insert(struct node **front, struct node **rear, int value)
{
   struct node *temp;
   temp=(struct node *)malloc(sizeof(struct node));
       /* creates new node
       using data value
       passed as parameter */
   if(temp==NULL)
   {
      printf("No Memory available Error\n");
      exit(0);
   }
   temp->data = value;
   temp->link=NULL;
   if(*rear == NULL)
   {
      *rear = temp;
      *front = *rear;
   }
   else
   {
      (*rear)->link = temp;
      *rear = temp;
   }
   printf("\t cu %d request inserted in the queue \n",value);
}
// call this function when cu wants to make a request, pass cu id as parameter to store into request queue.
void make_request(int cu_id)
{
	cu_insert(&front,&rear,cu_id);
}
//call this function to accept cu request,controller will call this function when cui.pui=0 
void accept_request(int front_check)
{
            cu_fetch(&front,&rear,&value, front_check);
           
}

void cu_fetch(struct node **front, struct node **rear, int *value, int front_check)
{
	
   struct node *temp;
   if((*front == *rear) && (*rear == NULL))
   {
      printf(" The request queue is empty cannot assign \n");
     // exit(0);
   }
   else
   {
   *value = (*front)->data;
    if(front_check==*value)
    {
      temp = *front;
      *front = (*front)->link;
      if(*rear == temp)
      {
	   *rear = (*rear)->link;
      }
      free(temp);
	   printf("The cu assigned is %d\n",*value);
	   del_success++;
	   //create a black list to avoid other neighbour (cus ) request assignment
	   for(int i=1;i<num_cu*4;i=i+4)
	   {
		   if(controller[front_check*4+1]==controller[i])
		   {
		   blacklist[controller[i-1]]=1;
		   }
	   }
    }
    else
    {
		printf("assigning cu ...%d ....failed...not in front of queue. \n",front_check);
		del_unsuccess++;
   // printf(" %d cu is in front of the queue. %d cu is either not in queue or is not in front of queue\n",*value, front_check);	
    }
   }
}

/*
=============settings=============
#define num_pu 3 //set number of pus
#define num_cu 3 //set number of cus
    int cus_pu[num_cu]={0,1,2}; //edit to set cu's pu in order cu0,cu1,cu2 ...
	int pu_range[num_pu]={100,100,100}; //edit to set pu range

//use vector<bit> use it for bitwise memory allocation. even bool takes 1 byte
================implemented==============
cu wight w=1/d              d is cu's distance from pu
cu will generate request if cu.wt<range of corresponding pu
cu requests are inserted into queue after sorting by weight

==============drawback=============
if the desired cu is not in front of queue,request will not be assigned

===============definitions====================
request assignment : front element deletion from queue(FIFO)
insert request     : insert into request queue in order of wieght of cu
pu.state=0 means IDLE
cu.request=1 means request is made
black_list: once a pus neighbour is assigned request(deleted from request queue),other neighbours can't be assigned
to the zero state of that pu,so other neighbours are added into blacklist array.(non-neighbours are not added into blacklist)

black_list resets in every round.

=================generating sequence of randm numbers============
000011100011100001110001
[public variables]
int rnum=0; 
bool sw_state =0;

code within state update function:
//code to get sequeunce like this pu0's sequence: 00001110011100011000111000
//attention: this sequence will work only if there is one pu,if used in case 
//of more than one pu then all pu will be 1 for many rounds and then zero
if(rnum==0)
{
rnum = 1+rand()%4;
sw_state =!sw_state;
for(int j=0;j<num_pu;j++)
	{
     pu[j].state=sw_state;
    }
}
else
{
	for(int j=0;j<num_pu;j++)
	{
     pu[j].state=sw_state;
	}
rnum--;
}

=======================optimize =====================
use Primary_Unit structure in Secondary_Unit as a member for cu.pu (currently int pu)
then we can access the cu.pu.all_properties and methods directly.
[in this program cu.pu.propery won't work as pu is not Primary unit object here]


*/