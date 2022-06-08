//Código feito por Matheus Peixoto, Maria Júlia Zabbal e Kalebe Sousa

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct knapsackItens{
    int weight;
    int profit;
    int isInBag;
}Itens;

typedef struct currentBag{
    int weight;
    int profit;
    int *itensInside;
}CurrentBag;

int readItens(Itens **itens, int *knapsackSize, int *itensNum){
    FILE *file;
    
    file = fopen("info.txt", "r");

    if (!file) {
        printf("Erro ao ler arquivo\n");
        return 0;
    }

    fscanf(file, "%d", knapsackSize);
    fscanf(file, "%d", itensNum);
    
    *itens = (Itens*) malloc(sizeof(Itens) * (*itensNum));

    for(int i = 0; i < (*itensNum); i++){
        fscanf(file, "%d %d", &(*itens)[i].profit, &(*itens)[i].weight);
        (*itens)[i].isInBag = 0;
    }

    fclose(file);

    return 1;
}

void addItem(Itens *item, CurrentBag *currentBag, int index){
    currentBag->weight += (*item).weight;
    currentBag->profit += (*item).profit;
    item->isInBag = 1;
    currentBag->itensInside[index] = 1;
}

void removeItem(Itens *item, CurrentBag *currentBag, int index){
    currentBag->weight -= (*item).weight;
    currentBag->profit -= (*item).profit;
    item->isInBag = 0;
    currentBag->itensInside[index] = 0;
}

int main(){
    Itens *itens;
    CurrentBag currentBag, solution;
    int knapsackSize;
    int itensNum;
    float temperature = 50000, temperatureCooldownRate = 0.999;

    currentBag.profit = 0;
    currentBag.weight = 0;
    solution.profit = 0;
    solution.weight = 0;

    srand(time(NULL));
    
    if(!readItens(&itens, &knapsackSize,  &itensNum)){
         printf("Programa cancelado\n");
         return 1;
    }

    currentBag.itensInside = (int*) malloc(sizeof(int)*itensNum);
    solution.itensInside = (int*) malloc(sizeof(int)*itensNum);

    for(int i = 0; i < itensNum; i++){
        currentBag.itensInside[i] = 0;
        solution.itensInside[i] = 0;
    }

    while(temperature > 0.00005){
        int i = rand() % itensNum;
        if(!itens[i].isInBag){
            addItem(&itens[i], &currentBag, i);

            while(knapsackSize < currentBag.weight){
                i = rand() % itensNum;
                if(itens[i].isInBag){
                    removeItem(&itens[i], &currentBag, i);
                }
            }

        }
        else{
            removeItem(&itens[i], &currentBag, i);
            i = rand() % itensNum;
            
            if(!itens[i].isInBag){

                addItem(&itens[i], &currentBag, i);

                while(knapsackSize < currentBag.weight){
                    i = rand() % itensNum;
                    if(itens[i].isInBag){
                        removeItem(&itens[i], &currentBag, i);
                    }
                }
            }
        }
        double delta = currentBag.profit - solution.profit;
        if(delta > 0 || exp(delta/temperature) > ((rand() % 1001) / 1000)){
            solution.profit = currentBag.profit;
            solution.weight = currentBag.weight;
            for(int i = 0; i < itensNum; i++){
                solution.itensInside[i] = currentBag.itensInside[i];
            }
        }
        temperature = temperature * temperatureCooldownRate;
    }

    printf("Melhor solução:\n");
    printf("Peso: %d Lucro:%d\n", solution.weight, solution.profit);
    printf("Itens:\n");
    
    for(int i = 0; i < itensNum; i++){
        if(solution.itensInside[i] == 1){
            printf("%d %d\n", itens[i].weight, itens[i].profit); 
        }
    }

    return 0;
}