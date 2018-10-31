#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define A 81

int main(int argc, char* argv[])
{
	int inArr[]={5, 4, 36, 29, 74, 78, 61, 23, 54, 1, 63, 75, 20, 13, 14, 11, 28, 29, 7, 55, 12, 19, 10, 16, 22, 8, 33, 38, 18, 56, 31, 20, 0, 47, 42, 27, 53, 38, 47, 12, 71, 24, 18, 80, 37, 22, 64, 45,18, 30, 5, 78, 37, 26, 44,  52, 21, 17, 40, 79, 12, 20, 48,  55,69, 25,  7,73, 68, 33,  32, 59, 39, 70, 16, 47,19,  63,4, 46, 74};
	int outArr[A],*buckS,rank,size,*ccc,*disp;
  	MPI_Status status;
  	int i,j,n,inc,min,max,tmp;

  	MPI_Init(&argc,&argv);

  	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  	MPI_Comm_size(MPI_COMM_WORLD,&size);

 	if (rank == 0)
	{
    		for (i = 1; i < size; i++)
		{
      			MPI_Send(inArr,A,MPI_INT,i,1,MPI_COMM_WORLD);
    		}
  	}
  
	else 
	{
    		MPI_Recv(inArr,A,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
  	}

  	inc = 0;
  	min = rank * (A/size);
  	max = (rank + 1) * (A/size);

  	for (i = 0; i < A; i++)
	{
    		if ((inArr[i] >= min) && (inArr[i] < max))
		{
      			inc += 1;
    		}
  	}

  	buckS = malloc(inc * sizeof(int));
  	inc = 0;
  	for (i = 0; i < A; i++)
	{
    		if ((inArr[i] >= min) && (inArr[i] < max))
		{
      			buckS[inc] = inArr[i];
      			inc += 1;
    		}
  	}
  	for (i = 0; i < inc; i++)
	{
    		for (j = i+1; j < inc; j++)
		{
      			if (buckS[i] > buckS[j])
			{
        			tmp = buckS[i];
        			buckS[i] = buckS[j];
        			buckS[j] = tmp;
      			}
    		}
  	}
  	if (rank == 0)
	{
      		ccc = malloc(size * sizeof(int));
    		disp = malloc(size * sizeof(int));
  	}
  	MPI_Gather(&inc,1,MPI_INT,ccc,1,MPI_INT,0,MPI_COMM_WORLD);

  	if (rank == 0)
	{
    		disp[0] = 0;
    		for (i = 0; i < size-1; i++)
		{
      			disp[i+1] = disp[i] + ccc[i];
    		}
  	}

  	MPI_Gatherv(buckS,inc,MPI_INT,outArr,ccc,disp,MPI_INT,0,MPI_COMM_WORLD);

  	if (rank == 0)
	{
    		printf("Input Array: \n");
    		for (i = 0; i < A; i++) 
			printf("%d ",inArr[i]);
    		printf("\nSorted Output: \n");
    		for (i = 0; i < A; i++) 
			printf("%d ",outArr[i]);
  	}
	printf("\n");

  	MPI_Finalize();
  	return 0;
}

