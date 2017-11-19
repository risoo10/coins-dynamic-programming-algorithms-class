#include <stdio.h>
#include <stdlib.h>
#include <limits.h>



typedef struct coin{

    int value;
    int weight;
    int count;

} Coin;


typedef struct knapsackNode {
    int weight;
    int *coinCount;
} KnapsackNode;


// Select minimum
void minimumNode(KnapsackNode *actual, KnapsackNode *previous, Coin *coin, int coinIndex, int coinsCount){

    int newWeight = previous->weight == INT_MAX ? INT_MAX : previous->weight + coin->weight;

    // Check if you can add the coin.
    // If is limit full, return actual value
    if(previous->coinCount[coinIndex] == coin->count){
        return;
    }

    // Keep the actual if the result of weight is smaller or equal
    if(actual->weight <= newWeight ){
        return;
    } else {

        if(previous->weight == INT_MAX){
            return;
        }

        // Save new weight
        actual->weight = newWeight;

        int j;
        for(j=0; j<coinsCount; j++){
            actual->coinCount[j] = previous->coinCount[j];
        }

        // Increment newly added coin
        actual->coinCount[coinIndex]++;
    }
}




KnapsackNode **knapsack1D(Coin **coins, int sum, int coinsCount){

    int i,j;

    // Alloc memory
    KnapsackNode **metaData = calloc(sum+1, sizeof(KnapsackNode *));
    for(i=0; i<=sum; i++){
        metaData[i] = calloc(1, sizeof(KnapsackNode));
        metaData[i]->coinCount = calloc(coinsCount, sizeof(int));
    }


    metaData[0]->weight = 0;

    for(i=1; i<=sum; i++){

        metaData[i]->weight = INT_MAX;
        for(j=0; j<coinsCount; j++){
            Coin *coin = coins[j];
            if(coin->value <= i){
                minimumNode(metaData[i], metaData[i-coin->value], coin, j, coinsCount);
            }
        }
    }




    return metaData;

}


int main() {


    FILE *file = fopen("C:\\Users\\Riso\\CLionProjects\\Mimnce\\input.txt", "r");
    FILE *fileO = fopen("C:\\Users\\Riso\\CLionProjects\\Mimnce\\output.txt", "w");
    int caseCount;

    fscanf(file, "%d\n", &caseCount);

    int i, j, k;

    // Prcess every case
    for(i=0; i<caseCount; i++){

        printf("Start a case number : %d\n", i);

        int coinsCount;
        Coin **coins;
        int *sums;

        fscanf(file, "\n%d\n", &coinsCount);


        // Alloc memory
        coins = malloc(coinsCount * sizeof(Coin*));

        // Insert coins to the variable
        for(j=0; j<coinsCount; j++){
            coins[j] = malloc(sizeof(Coin));

            int value;
            int weight;
            int count;

            fscanf(file, "%d %d %d\n", &value, &weight, &count);

            coins[j]->value = value;
            coins[j]->count = count;
            coins[j]->weight = weight;
        }

        int sumCount;
        fscanf(file, "%d\n", &sumCount);

        // Load Sums to memory
        sums = malloc(sumCount*sizeof(int));
        for(j=0; j<sumCount; j++){
            int sum;
            fscanf(file, "%d ", &sum);
            sums[j] = sum;
        }


        // For every sum traverse table and printout selected coins
        for(j=0; j<sumCount; j++){
            int sum = sums[j];

            KnapsackNode **table = knapsack1D(coins, sum, coinsCount);
            KnapsackNode *result = table[sum];

            if(result->weight == INT_MAX){
                printf("%d\n", -1);

            } else {

                // Print results
                for (k = 0; k < coinsCount; k++) {
                    coins[k]->count -= result->coinCount[k];
                    printf("%d ", result->coinCount[k]);
                }
                printf("\n");

            }

            // Free memory
            for(k=0; k<=sum; k++){
                free(table[k]->coinCount);
                free(table[k]);
            }

            free(table);

        }

        printf("\n");

        // FREE memory

        // free sum array
        free(sums);

        // Free coins array
        for(j=0; j<coinsCount; j++){
            free(coins[j]);
        }
        free(coins);
    }


    fclose(file);
    fclose(fileO);



}
