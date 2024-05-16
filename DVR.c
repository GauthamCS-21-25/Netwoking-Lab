#include <stdio.h>
#include <stdlib.h>

#define N 6

void main() {
	int cost[N][N] = {
		{0,99,2,10,99,99},
		{99,0,99,2,1,99},
		{2,99,0,1,99,18},
		{10,2,1,0,11,6},
		{99,1,99,11,0,2},
		{99,99,18,6,2,0},
	};
	int min;
	
	printf("Initial Cost :\n");
	for (int i = 0; i <= N; i++) {
		if(i == 0) {
			for(int j = 0; j <= N; j++) {
				printf("%c\t",j + 64);
			}
			printf("\n");
		} else {
			for(int j = 0; j <= N; j++) {
				if(j == 0 && i != 0) printf("%c\t", i + 64);
				else printf("%d\t",cost[i - 1][j - 1]);
			}
			printf("\n");
		}
	}
	
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(j!=i) {
				for(int k = 0; k < N; k++) {
					if(k!=i) {
						int new = cost[j][i] + cost[i][k];
						if(new < cost[j][k]) cost[j][k] = new;
					}
				}
			}
		}
	}
	
	
	printf("Routing Table :\n");
	for (int i = 0; i <= N; i++) {
		if(i == 0) {
			for(int j = 0; j <= N; j++) {
				printf("%c\t",j + 64);
			}
			printf("\n");
		} else {
			for(int j = 0; j <= N; j++) {
				if(j == 0 && i != 0) printf("%c\t", i + 64);
				else printf("%d\t",cost[i - 1][j - 1]);
			}
			printf("\n");
		}
	}
}

