#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

int swap(int *firstIndex, int *secondIndex){
    int temp = *firstIndex;
    *firstIndex = *secondIndex;
    *secondIndex = temp; 
}

void MaxHeap(int * myHeap, int n){
	
	int sArray = n;
	if(n%2 != 0) n -= 1;
	else{
		if(myHeap[n] > myHeap[n/2]) swap(&myHeap[n], &myHeap[n/2]);
		n -=2;
	}

	for(int i = n; i > 1; i-=2){
		if(myHeap[i] > myHeap[i/2] && myHeap[i] > myHeap[i-1]) 
			swap(&myHeap[i], &myHeap[i/2]);
		if(myHeap[i+1] > myHeap[i/2] && myHeap[i+1] > myHeap[i]) 
			swap(&myHeap[i+1], &myHeap[i/2]);
	}

}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int *myHeap = (int *) malloc(sizeof(int)*(n+1));
	extraMemoryAllocated += sizeof(int)*n;

	for(int i = 1; i <= n; ++i) myHeap[i] = arr[i-1];
	for(int i = n; i > 0; i--){
		MaxHeap(myHeap, n);
		arr[i-1] = myHeap[1];
		swap(&myHeap[1],&myHeap[i]);
		n -= 1;
	}

	free(myHeap);
}

void Merge(int * myArray, int left, int middle, int right){
    int lArraySize = ((middle-left)+1);
    int rArraySize = (right-middle);
    int *lTempArray = (int *) malloc(sizeof(int)*lArraySize);
    int *rTempArray = (int *) malloc(sizeof(int)*rArraySize);
	extraMemoryAllocated += sizeof(int)*lArraySize + sizeof(int)*rArraySize;

    for(int i = 0; i < lArraySize; i++){
        lTempArray[i] = myArray[i+left]; 
    }

    for(int i = 0; i < rArraySize; i++){
        rTempArray[i] = myArray[i+middle+1]; 
    }

    int counterL = 0;
    int counterR = 0;

    while(counterL < lArraySize && counterR < rArraySize){
        if(lTempArray[counterL] < rTempArray[counterR]){
            myArray[left] = lTempArray[counterL];
            left ++;
            counterL ++;
        }
        else{
            myArray[left] = rTempArray[counterR];
            left ++;
            counterR ++;
        }
    }

    while(counterL < lArraySize){
        myArray[left] = lTempArray[counterL];
        left ++;
        counterL ++;
    }

    while(counterR < rArraySize){
        myArray[left] = rTempArray[counterR];
        left ++;
        counterR ++;
    }

    free(rTempArray);
    free(lTempArray);

}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if(l < r){
        int middle = (l + r)/2;
        mergeSort(pData, l, middle);
        mergeSort(pData, middle+1, r);
        Merge(pData, l, middle, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");

	for (i=0;i<dataSz;++i) /* This line allows the loop to stop when it runs out of numbers */
	{
		printf("%d ",pData[i]);
		if(i > 100) break; /* This line breaks the cycle when 100 numbers were printed */
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		if(i < 99) continue; /* This line avoids printing numbers that were alredy printed */
		else if (i < 0) break; /* This line breaks the cycle in case all numbers were already printed */
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.10lf\n",cpu_time_used); /* Modified to show more decimal numbers */
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.10lf\n",cpu_time_used); /* Modified to show more decimal numbers */
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}