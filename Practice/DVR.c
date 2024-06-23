#include <stdio.h>

void main(){
	int n, i, j, k, cost[10][10], new;
	printf("Enter the number of nodes: ");
	scanf("%d", &n);
	printf("Enter the cost matrix: \n");
	for(i=0; i<n; i++)
		for(int j=0; j<n; j++)
			scanf("%d", &cost[i][j]);
	printf("\n");
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			if(j!=i)
				for(k=0; k<n; k++)
					if(k!=i){
						new=cost[j][i]+cost[i][k];
						if(new<cost[j][k])
							cost[j][k]=new;
					}
	for(i=0; i<n; i++){
		for(j=0; j<n; j++)
			printf("%d\t", cost[i][j]);
		printf("\n");
	}
}
