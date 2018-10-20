#include <stdio.h>
#include <stdlib.h>

typedef struct proc{
	char start;
	int prioN;
	int procSize;
}proc;

int main(int argc, char *argv[]){

	int countH = 1; //Total number of holes.
	int procMem = 0; // How many processes there are in the memory.
	int procNum = 0; // How many processes there are.
	char longMem[128]; //Priority for swapping - Defined to be 128 in assignment

	proc runAlph[26]; //Proc for Alphabet, 26 letters in alphabet
	proc memReal[128]; //Memory Array - Defined to be 128 in assignment.
	proc qu[26]; //Queue for processing, 26 letters in alphabet.

	//Temp, Calculation Variables, and Criterias
	int bigCheck;
	int checkL, checkF;
	int temp;
	int temp4 = 0;
	int runQ = 0;
	int count = 0;
	int set = 0;
	int mode = 0;
	int temp2 = 0, temp3 =0, tempo = 0;
	int add, hold, min;
	double avgProc = 0, avgHol = 0, cumMem2 = 0, cuMem = 0;
	double ld;
	double memUsage;
	double totMem, totProc, totHol;

	FILE *input;

	//Memory of size is 128.
	memReal[0].procSize = 128;
	//Indicator with '$' since we use the alphabet.
	memReal[0].start = '$';

	input = fopen(argv[1], "r");

	if (input == 0 || argc != 2) {
		printf("Error please re-run executable checkLth format ./countH <filename>");
		exit(0);
	}

	//scanning in variables.
	while(fscanf(input, "%c %d\n", &runAlph[count].start, &runAlph[count].procSize) != EOF){

		if (runAlph[count].procSize <= 128) {
			runAlph[count].prioN = 0;
			qu[runQ] = runAlph[count];
			runQ++;
		}

		else runAlph[count].prioN = 3;

		procNum++;
		count++;

	}

	//Loop for memory Algorithms
	for(mode = 0; mode < 4; mode++){

		totMem = 0;
		totHol = 0;
		totProc = 0;

		if (mode ==0) printf("\n\n****First Fit Alogrithm Intiating****\n\n");

    else if (mode ==1) printf("\n\n****Best Fit Alogrithm Intiating****\n\n");

    else if (mode ==2) printf("\n\n****Next Fit Alogrithm Intiating****\n\n");

    else if (mode ==3) printf("\n\n****Worst Fit Alogrithm Intiating****\n\n");

		while(runQ > 0){

			bigCheck = 1;
			temp = countH + procMem;


			for (int p = 0; p < temp; p++){

				if (memReal[p].procSize >= qu[0].procSize && memReal[p].start == '$') {

					if (memReal[p].procSize < checkF) checkF = p;
					if (memReal[p].procSize > checkL) checkL = p;

					bigCheck =0;
				}

			}

			for(set = 0; set < temp; set++){


				if((memReal[set].procSize >= qu[0].procSize && memReal[set].start == '$'
					&& (mode == 0 || mode == 2)) || (mode == 3 && bigCheck == 0)
					|| (mode == 1 && bigCheck == 0)){


					if (mode == 1) set = checkF;
					if (mode == 3) set = checkL;


					if (memReal[set].procSize <= qu[0].procSize) countH--;

					//Shifting the memory.
		      else {
		        for(int k = (countH + procNum); k > set; k--){
		          memReal[k] = memReal[k - 1];
		        }
		        memReal[set+1].procSize -= qu[0].procSize;
		      }

					memReal[set] = qu[0];
					longMem[procMem] = memReal[set].start;

					procMem++;
					ld++;
					temp3 = procMem + countH;
					temp2 = 0;

					for(int r = 0; r < temp3; r++){
						if(memReal[r].start != '$') 	temp2 += memReal[r].procSize;
					}

					//Calculations to find out #of process, countH , memusage and cumualtive mem usage.
					memUsage = (temp2 * 100) / 128;

					totProc += procMem;
					totMem += memUsage;
					totHol += countH;
					cuMem = totMem / ld;

					printf("Pid Loaded, #processes = %d, #Count = %d, %%memusage = %.0f, cumulative %%mem = %.0f\n", procMem, countH, memUsage, cuMem);

					//Shift the mem
				  for (int t = 0; t < (runQ - 1); t++) qu[t] = qu[t + 1];
					bigCheck = 0;
					runQ--;

					//if Mode is not Next fit exit.
					if(mode != 2) break;

				}
			}

			if(bigCheck == 1){
				tempo = procMem + countH;
		    for (int x = 0; x < temp; x ++){
				hold = 0;
	      add = x + 1;
	      min = x - 1;
	      if(memReal[x].start == longMem[0])
	      {
						//Finding number of total countH
	          for (int p = 0; p < (procMem - 1); p++) longMem[p]= longMem[p+1];

	          memReal[x].prioN++;
	          if(memReal[x].prioN < 3) {
	            qu[runQ] = memReal[x];
	            runQ++;
	          }

	          if ( (add != tempo) && x > 0) {

	            if (memReal[add].start == '$' && memReal[min].start == '$'){
	              countH--;
	              hold = 1;
	              memReal[min].procSize += (memReal[x].procSize + memReal[add].procSize);

	            }

	            else if (memReal[add].start == '$') {

	              hold = 2;
	              memReal[x].start = '$';
	              memReal[x].procSize += memReal[add].procSize;

	            }

	            else if (memReal[min].start == '$') {

	              hold =3;
	              memReal[min].start += memReal[x].procSize;

	            }

	            else {
								countH++;
	              memReal[x].start = '$';
	            }
	          }

						else if (x > 0){

						 if ('$' == memReal[min].start){

							 hold = 3;
							 memReal[min].procSize += memReal[x].procSize;
						 }

						 else {

							 countH++;
							 memReal[x].start = '$';
						 }
					 }

						else if (add != tempo) {

						 if('$' == memReal[add].start){
							 hold = 2;
							 memReal[x].start = '$';
							 memReal[x].procSize += memReal[add].procSize;

						 }

						 else {
							 countH++;
							 memReal[x].start = '$';
						 }

					 }

	          else {

	            countH++;
	            memReal[x].start = '$';
	          }

	          procMem--;
						temp4 = (procMem + countH);

						if (hold == 3) for (int k = x; k < temp4; k++) memReal[k] = memReal[k+1];

						else if (hold == 2) for (int k = add; k < temp4; k++) memReal[k] = memReal[k+1];

		        else if (hold == 1) for (int k = x; k < temp4; k++) memReal[k] = memReal[k+2];

						break;

					}
				}
			}
			//reset variables
			checkL = 0;
			checkF = 99999;

		}

    avgProc = totProc / ld;
    avgHol = totHol / ld;
    cumMem2 = totMem / ld;

    printf("Total loads = %.0f, average #processes = %.1f, average #countH = %.1f, cumulative %%mem = %.0f\n", ld, avgProc, avgHol, cumMem2);

		//Resetting the Variables and Queue
		memReal[0].procSize = 128;
		ld = 0;
		procMem = 0;
		memReal[0].start = '$';

		countH = 1;

		for (int w = 0; w < procNum; w++) {

			if (runAlph[w].procSize <= 128){

				runAlph[w].prioN = 0;
				qu[runQ] = runAlph[w];
				runQ++;
			}
		}


	}

	return 0;
}
