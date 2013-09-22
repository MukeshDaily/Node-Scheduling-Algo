Node-Scheduling-Algo
============

Node Scheduling Algorithm and implementaion in c++
IDE : Visual Stuio 2012

=============settings=============
#define num_pu 3 //set number of pus
#define num_cu 3 //set number of cus
    int cus_pu[num_cu]={0,1,2}; //edit to set cu's pu in order cu0,cu1,cu2 ...
	int pu_range[num_pu]={100,100,100}; //edit to set pu range

//use vector<bit> use it for bitwise memory allocation. even bool takes 1 byte
================implemented==============
cu wight w=1/d              d is cu's distance from pu
cu will generate request if cu.wt< range of corresponding pu
cu requests are inserted into queue after sorting weights

==============drawback=============
if the desired cu is not in front of queue,request will not be assigned

===============definitions====================
request assignment : front element deletion from queue(FIFO)
insert request     : insert into request queue in order of weight of requesting cu
pu.state=0 means IDLE
cu.request=1 means request is made
black_list: once a pu's neighbour is assigned request(deleted from request queue),other neighbours can't be assigned
to the zero state of that pu,so other neighbours are added into blacklist array for one round.
(non-neighbours are not added into blacklist)

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
sorting other than bubble sort