#include<stdio.h> 
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<math.h>


int nReads = 0;
int nWrites = 0;
int nHits = 0;
int nMisses = 0;
int pReads = 0;
int pWrites = 0;
int pHits = 0;
int pMisses = 0;


struct cacheBlock { 
	char* tag;
	int valid;
	int rank;
};

struct cacheBlock** initializeCache(int assocSize, int setSize, int tagSize) {
	struct cacheBlock** result = (struct cacheBlock**) malloc(setSize * sizeof(struct cacheBlock*));
	for(int i = 0; i < setSize; i++) {
		result[i] = (struct cacheBlock*) malloc(assocSize * sizeof(struct cacheBlock));
	}

	for(int i = 0; i < setSize; i++) {
		for(int j = 0; j < assocSize; j++) {
			result[i][j].tag = malloc(tagSize+1 * sizeof(char));
			result[i][j].valid = 0;
			result[i][j].rank = 0;
		}
	}

	return result;

}

int convertDecimal(int input) {
	int base = 1;
	int value = 0;
	while(input > 0) {
		int remainder = input % 10;
		value += (remainder*base);
		input /= 10;
		base *= 2;
	}

	return value;
}

void updateN(struct cacheBlock** cache, char function, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
		}
	}

	if(found) {
		if(function == 'R') {
			nHits++;
			return;
		} else if(function == 'W') {
			nHits++;
			nWrites++;
			return;
		}
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
				cache[index][i].rank = 1;
	
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
					}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
			
					break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
					cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		if(function == 'R') {
			nMisses++;
			nReads++;
		} else if(function == 'W'){
			nMisses++;
			nReads++;
			nWrites++;
		}
	}
}

bool updateP(struct cacheBlock** cache, char function, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
		}
	}

	if(found) {
		if(function == 'R') {
			pHits++;
		} else if(function == 'W') {
			pHits++;
			pWrites++;
		}
		return false;
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
				cache[index][i].rank = 1;
	
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
					}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
			
					break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
					cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		if(function == 'R') {
			pMisses++;
			pReads++;
		} else if(function == 'W'){
			pMisses++;
			pReads++;
			pWrites++;
		}

		return true;	
	}

}

void prefetch(struct cacheBlock** cache, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
		}
	}

	if(found) {
		return;
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
				cache[index][i].rank = 1;
	
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
					}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
			
					break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
					cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		pReads++;

		return;	
	}

}

void LRUupdateN(struct cacheBlock** cache, char function, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
			for(int j = 0; j < i; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < cache[index][i].rank) {
					cache[index][j].rank++;
				}
			}
	
			for(int j = i+1; j < assocSize; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < cache[index][i].rank) {
					cache[index][j].rank++;
				}
			}
			cache[index][i].rank = 1;
			break;
		}
	}

	if(found) {
		if(function == 'R') {
			nHits++;
			return;
		} else if(function == 'W') {
			nHits++;
			nWrites++;
			return;
		}
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
	
				for(int j = 0; j < i; j++) {
					if(cache[index][j].valid == 1 && cache[index][j].rank < cache[index][i].rank) {
						cache[index][j].rank++;
					}
				}
	
				for(int j = i+1; j < assocSize; j++) {
					if(cache[index][j].valid == 1 && cache[index][j].rank < cache[index][i].rank) {
						cache[index][j].rank++;
					}
				}
				cache[index][i].rank = 1;
			
				break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
				cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
		
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		if(function == 'R') {
			nMisses++;
			nReads++;
		} else if(function == 'W'){
			nMisses++;
			nReads++;
			nWrites++;
		}
	}
}

bool LRUupdateP(struct cacheBlock** cache, char function, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
			int temp = cache[index][i].rank;
			cache[index][i].rank = 1;
			for(int j = 0; j < i; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < temp) {
					cache[index][j].rank++;
				}
			}
	
			for(int j = i+1; j < assocSize; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < temp) {
					cache[index][j].rank++;
				}
			}
			break;
		}
	}

	if(found) {
		if(function == 'R') {
			pHits++;
		} else if(function == 'W') {
			pHits++;
			pWrites++;
		}
		return false;
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
				cache[index][i].rank = 1;
	
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
					}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
			
					break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
					cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		if(function == 'R') {
			pMisses++;
			pReads++;
		} else if(function == 'W'){
			pMisses++;
			pReads++;
			pWrites++;
		}

		return true;	
	}

}

void LRUprefetch(struct cacheBlock** cache, char* tag, int index, int assocSize) {
	bool found = false;
	for(int i = 0; i < assocSize; i++) {
		if(strcmp(cache[index][i].tag, tag) == 0 && cache[index][i].valid == 1) {
			found = true;
			int temp = cache[index][i].rank;
			cache[index][i].rank = 1;
			for(int j = 0; j < i; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < temp) {
					cache[index][j].rank++;
				}
			}
	
			for(int j = i+1; j < assocSize; j++) {
				if(cache[index][j].valid == 1 && cache[index][j].rank < temp) {
					cache[index][j].rank++;
				}
			}
			break;
		}
	}

	if(found) {
		return;
	} else {
		for(int i = 0; i < assocSize; i++) {
	
			if(cache[index][i].rank == assocSize) {			
				strcpy(cache[index][i].tag, tag);
				cache[index][i].rank = 1;
	
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
			
					break;
	
			} else if(cache[index][i].valid == 0) {
				strcpy(cache[index][i].tag, tag);
				cache[index][i].valid = 1;
					cache[index][i].rank = 1;
								
				for(int j = 0; j < i; j++) {
					cache[index][j].rank++;
				}
	
				for(int j = i+1; j < assocSize; j++) {
					cache[index][j].rank++;
				}
				
				break;
			}
		}
	
		pReads++;

		return;	
	}

}

void printCache() {
	printf("no-prefetch\n");
	printf("Memory reads: %d\n", nReads);
	printf("Memory writes: %d\n", nWrites);
	printf("Cache hits: %d\n", nHits);
	printf("Cache misses: %d\n", nMisses);
	printf("with-prefetch\n");
	printf("Memory reads: %d\n", pReads);
	printf("Memory writes: %d\n", pWrites);
	printf("Cache hits: %d\n", pHits);
	printf("Cache misses: %d\n", pMisses);
}

void freeCache(struct cacheBlock** cache, int assocSize, int setSize, int tagBit) {
	for(int i = 0; i < setSize; i++) {
		for(int j = 0; j < assocSize; j++) {
			free(cache[i][j].tag);
		}
	}

	for(int i = 0; i < setSize; i++) {
		free(cache[i]);
	}

	free(cache);
}

int main(int argc, char** argv) {
	
	if(argc != 7) {
		printf("insufficient arguments\n");
		exit(0);
	}

	int cacheSize = atoi(argv[1]);
	int blockSize = atoi(argv[2]);
	int setSize;
	int assocSize;
	int prefetchCount = atoi(argv[5]);
	bool lru = false;	

	if(strcmp(argv[3], "lru") == 0) {
		lru = true;
	}
/*
	printf("number of arguments: %d\n", argc);
	printf("cache policy: %s\n", argv[3]);
	printf("associativity: %s\n", argv[4]);
	printf("prefetchCount: %d\n", prefetchCount);
*/
	bool fully = false;
	if(argv[4][0] == 'd') {
		assocSize = 1;
		setSize = cacheSize/blockSize;

	} else if(argv[4][5] == ':') {
		assocSize = argv[4][6] - '0';
		setSize = cacheSize/(assocSize*blockSize);
	} else {
		assocSize = cacheSize/blockSize;
		setSize = 1;		
		fully = true;
	}

	printf("cacheSize: %d\n", cacheSize);
	printf("blockSize: %d\n", blockSize);
	printf("assocSize: %d\n", assocSize);
	printf("setSize: %d\n", setSize);
	
	int indexSize = log2(setSize);
	int dirtySize = log2(blockSize);
	int tagSize = 48 - (indexSize + dirtySize);
/*
	printf("indexSize: %d\n", indexSize);
	printf("dirtySize: %d\n", dirtySize);
	printf("tagSize: %d\n", tagSize);
*/
	struct cacheBlock** cacheN = initializeCache(assocSize, setSize, tagSize);
	struct cacheBlock** cacheP = initializeCache(assocSize, setSize, tagSize);

	FILE* fp = fopen(argv[6], "r");
	if(fp == NULL) {
		printf("error\n");
		exit(0);
	}

	
	char function;
	unsigned long int address;


	while(fscanf(fp, "%c %lx\n", &function, &address) != EOF) {
		
		if(function == '#') {
			break;
		}
		
		unsigned long int copy = address;

		char reverse[100] = "";
		for(int i = 0; copy > 0; i++) {
			int temp = copy%2;
			if(temp == 0) {
				strcat(reverse, "0\0");
			} else {
				strcat(reverse, "1\0");
			}
			copy = copy/2;	
		}
		
		char binary[100] = "";
		int k = 0; 
		for(int i = strlen(reverse)-1; i >= 0; i--)  {
			binary[k] = reverse[i];
			k++;
		}

		char fixedBinaryArr[49] = "";
		int numZeroes = 48 - strlen(binary);
		for(int i = 0; i < numZeroes; i++) {
			strcat(fixedBinaryArr, "0\0");
		}

		strcat(fixedBinaryArr, binary);
		char* fixedBinary = fixedBinaryArr;
		fixedBinary[strlen(fixedBinaryArr)] = '\0';
	//	printf("binary is: %s\n", fixedBinary);
	
		int cur = 0;
		char tagArr[tagSize];
		char indexArr[indexSize];
		
		for(int i = 0; i < tagSize; i++) {
			tagArr[i] = fixedBinary[cur];
			cur++;
		}

		for(int i = 0; i < indexSize; i++) {
			indexArr[i] = fixedBinary[cur];
			cur++;
		}

		char* tag = tagArr;
		tag[tagSize] = '\0';
	//	printf("tag is: %s\n", tag);
		int index = atoi(indexArr);
		index = convertDecimal(index);

		if(fully) {
			index = 0;
		}
		
	//	printf("index is: %d\n", index);

		if(!lru) {
			updateN(cacheN, function, tag, index, assocSize);
			if(updateP(cacheP, function, tag, index, assocSize)) {
				unsigned long int prefetch_address = address;
				for(int i = 0; i < prefetchCount; i++) {
					prefetch_address += blockSize;
			
					unsigned long int copyP = prefetch_address;
			
					char reverseP[100] = "";
					for(int i = 0; copyP > 0; i++) {
						int tempP = copyP%2;
						if(tempP == 0) {
							strcat(reverseP, "0\0");
						} else {
							strcat(reverseP, "1\0");
						}
							copyP = copyP/2;	
					}
					
					char binaryP[100] = "";
					int l = 0; 
					for(int i = strlen(reverseP)-1; i >= 0; i--)  {
						binaryP[l] = reverseP[i];
						l++;
					}
				
					char fixedBinaryArrP[49] = "";
					int numZeroesP = 48 - strlen(binaryP);
					for(int i = 0; i < numZeroesP; i++) {
						strcat(fixedBinaryArrP, "0\0");
					}
			
					strcat(fixedBinaryArrP, binaryP);
					char* fixedBinaryP = fixedBinaryArrP;
					fixedBinaryP[strlen(fixedBinaryArrP)] = '\0';
				//	printf("binary is: %s\n", fixedBinaryP);
					int curP = 0;
					char tagArrP[tagSize];
					char indexArrP[indexSize];
				
					for(int i = 0; i < tagSize; i++) {
						tagArrP[i] = fixedBinaryP[curP];
						curP++;
					}
			
					for(int i = 0; i < indexSize; i++) {
						indexArrP[i] = fixedBinaryP[curP];
						curP++;
					}
				
					char* tagP = tagArrP;
					tagP[tagSize] = '\0';
				//	printf("   tag is: %s\n", tag);
					int indexP = atoi(indexArrP);
				//	printf("index is: %s\n", indexArrP);
					indexP = convertDecimal(indexP);		
				
					if(fully) {
						indexP = 0;
					}

					
					prefetch(cacheP, tagP, indexP, assocSize);
					


				}
			}
		} else {
			LRUupdateN(cacheN, function, tag, index, assocSize);
			if(LRUupdateP(cacheP, function, tag, index, assocSize)) {
				unsigned long int prefetch_address = address;
				for(int i = 0; i < prefetchCount; i++) {
					prefetch_address += blockSize;
			
					unsigned long int copyP = prefetch_address;
			
					char reverseP[100] = "";
					for(int i = 0; copyP > 0; i++) {
						int tempP = copyP%2;
						if(tempP == 0) {
							strcat(reverseP, "0\0");
						} else {
							strcat(reverseP, "1\0");
						}
							copyP = copyP/2;	
					}
					
					char binaryP[100] = "";
					int l = 0; 
					for(int i = strlen(reverseP)-1; i >= 0; i--)  {
						binaryP[l] = reverseP[i];
						l++;
					}
				
					char fixedBinaryArrP[49] = "";
					int numZeroesP = 48 - strlen(binaryP);
					for(int i = 0; i < numZeroesP; i++) {
						strcat(fixedBinaryArrP, "0\0");
					}
			
					strcat(fixedBinaryArrP, binaryP);
					char* fixedBinaryP = fixedBinaryArrP;
					fixedBinaryP[strlen(fixedBinaryArrP)] = '\0';
				//	printf("binary is: %s\n", fixedBinaryP);
					int curP = 0;
					char tagArrP[tagSize];
					char indexArrP[indexSize];
				
					for(int i = 0; i < tagSize; i++) {
						tagArrP[i] = fixedBinaryP[curP];
						curP++;
					}
			
					for(int i = 0; i < indexSize; i++) {
						indexArrP[i] = fixedBinaryP[curP];
						curP++;
					}
				
					char* tagP = tagArrP;
					tagP[tagSize] = '\0';
				//	printf("   tag is: %s\n", tag);
					int indexP = atoi(indexArrP);
				//	printf("index is: %s\n", indexArrP);
					indexP = convertDecimal(indexP);		
				
					if(fully) {
						indexP = 0;
					}

					
					LRUprefetch(cacheP, tagP, indexP, assocSize);
					


				}
			}
		}
	//	printf("Reads:%d Writes:%d Hits:%d Miss:%d\n", pReads, pWrites, pHits, pMisses);
	}

	fclose(fp);
//	printf("%d\n", prefetchCount);
	printCache();
	freeCache(cacheN, assocSize, setSize, tagSize);
	freeCache(cacheP, assocSize, setSize, tagSize);
	return 0;

}
