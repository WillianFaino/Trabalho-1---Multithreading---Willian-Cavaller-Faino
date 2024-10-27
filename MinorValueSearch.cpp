#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;


clock_t T;

int *v_index;
double **m;
double minorValue;
sem_t S;
//#define X 15000
//#define Y 10000
int X, Y;

double **matrixAlloc(double X, double Y){
	double **m = (double**)malloc(sizeof(double*) * X);
	
	for (int i=0;i<X;i++){
		m[i] = (double*)malloc(sizeof(double)*Y);
	}
	srand(time(NULL));
	
	for(int i=0;i<X;i++){
		for(int j=0;j<Y;j++){
			
			m[i][j] = rand()-(rand()%10000);
		}
	}
	
	
	return m;
}

void* minorSearch(void* id){
	

	int iI=v_index[(int)id];
	int iF=v_index[(int)id+1];
	
	//cout<<"iI = "<<iI<<endl;
	//cout<<"iF = "<<iF<<endl;
	for (int i=iI; i<iF; i++){
		for (int j=0;j<Y;j++){
			if(m[i][j]<minorValue){
				sem_wait(&S);
				minorValue=m[i][j];
				sem_post(&S);
			}
		}
	}
	//cout<<"minorSearch "<<(int)id<<endl;
	
	
	return NULL;
}

void* minorSearchNoSemaphore(void* id){
	

	int iI=v_index[(int)id];
	int iF=v_index[(int)id+1];
	
	//cout<<"iI = "<<iI<<endl;
	//cout<<"iF = "<<iF<<endl;
	for (int i=iI; i<iF; i++){
		for (int j=0;j<Y;j++){
			if(m[i][j]<minorValue){
				minorValue=m[i][j];
			}
		}
	}
	//cout<<"minorSearch "<<(int)id<<endl;
	
	
	return NULL;
}

void matrixDiv(int tam, int n, int *vindex){
	int a = tam/n;
	for(int i=0;i<n;i++){
		vindex[i]=a*i;
	}
	vindex[n]=tam;
}

void checkMinorSearch(int x, int y){
	double mv = minorValue;
	
	for (int i=0; i<x; i++){
		for (int j=0;j<y;j++){
			if(m[i][j]<minorValue){
				mv=m[i][j];
			}
		}
	}
	
	if(mv==minorValue)cout<<"Valido"<<endl;
	else cout<<"Invalido"<<endl;
}


int main(int argc, char const *argv[]){
	int N_Threads;
	
	bool sem;
	
	cout<<"Dimensoes da Matriz (maximo 15000x10000 ou 10000x15000)\n";
	cout<<"Numero de linhas: ";
	cin>>X;
	cout<<"Numero de colunas: ";
	cin>>Y;
	cout<<"\n\nQuantidade de threads: ";
	cin>>N_Threads;
	cout<<"\n\nUtilizar semaforo para bloquear a secao critica (0 para nao usar, 1 para usar): ";
	cin>>sem;

	if(sem){
		cout<<"\n\n\n\n--------------------------------------------------------------------------------------\n\n\n\n";
		cout<<"Busca por menor valor em matriz "<<X<<"x"<<Y<<" com "<<N_Threads<<" threads, com o uso de semaforos\n\n\n\n";
		cout<<"--------------------------------------------------------------------------------------\n\n\n\n";
		sem_init(&S, 0, 1);
		pthread_t *nt = (pthread_t *) malloc(sizeof(pthread_t)*N_Threads);
		
		v_index = (int*)malloc(4*(N_Threads+1));
		m = matrixAlloc(X,Y);
		cout<<"Alocacao e preenchimento da matriz finalizados\n";
		minorValue=m[0][0];
		
		matrixDiv(X, N_Threads, v_index);
		
		for(long int i=0;i<N_Threads;i++){
			pthread_create(&nt[i],NULL,minorSearch, (void*)i);
		}
		
		T = clock();
		for(int i=0;i<N_Threads;i++){
			pthread_join(nt[i],NULL);
		}
		T = clock() - T;
		
		float ms = ((float)T/CLOCKS_PER_SEC)*1000;
		
		printf("Menor valor na matriz = %lf\n", minorValue);
		printf("Tempo da operacao de busca com %d thread(s) = %lf ms\n", N_Threads, ms);
	}else{	
		cout<<"\n\n\n\n--------------------------------------------------------------------------------------\n\n\n\n";
		cout<<"Busca por menor valor em matriz "<<X<<"x"<<Y<<" com "<<N_Threads<<" threads, sem o uso de semaforos\n\n\n\n";
		cout<<"--------------------------------------------------------------------------------------\n\n\n\n";
		pthread_t *nt = (pthread_t *) malloc(sizeof(pthread_t)*N_Threads);
		
		v_index = (int*)malloc(4*(N_Threads+1));
		m = matrixAlloc(X,Y);
		cout<<"Alocacao e preenchimento da matriz finalizados\n";
		minorValue=m[0][0];
		
		matrixDiv(X, N_Threads, v_index);
		
		for(long int i=0;i<N_Threads;i++){
			pthread_create(&nt[i],NULL,minorSearchNoSemaphore, (void*)i);
		}
		
		T = clock();
		for(int i=0;i<N_Threads;i++){
			pthread_join(nt[i],NULL);
		}
		T = clock() - T;
		
		float ms = ((float)T/CLOCKS_PER_SEC)*1000;
		printf("Menor valor na matriz = %lf\n", minorValue);
		printf("Tempo da operacao de busca com %d thread(s) = %lf ms\n", N_Threads, ms);
		
		cout<<"Validacao da busca sem semaforo atraves de busca por forca bruta: ";
		checkMinorSearch(X,Y);
	}
	
	
	return 0;
}

