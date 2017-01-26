#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define MaxNum 20

double x[MaxNum];
double y[MaxNum];
int num;
int path[MaxNum]={0};
int dist(int , int);

int min(int , int);

int DP(int , int, int **);

void FindTheWay(int , int, int **);

int main(int argc, char **argv){
  char *fname;
  FILE *fp1, *fp2;
  double data;
  int c;
  int count=0;
  int now = 0;
  int visited = 0;
  int i = 0;
  int **length;

  if(argc!=2){
    fprintf(stderr, "Usage:%s prob_file\n", argv[0]);
    exit(1);
  }
  fname = argv[1];

  fp1 = fopen(fname,"r");
  if(fp1 == NULL){
    printf("error opening file!\n");
    return 1;
  }

  c = fscanf(fp1, "%lf", &data);
  while(c != EOF){
    count += 1;
    if(count == 1){
      num = data;
    }else if(count % 2 == 0){
      x[count/2 - 1] = data;
    }else if(count % 2 == 1){
      y[count/2 - 1] = data;
    }
    c = fscanf(fp1, "%lf", &data);
  }
  fclose(fp1);

  length = malloc(sizeof(int)*MaxNum*((1<<(MaxNum-1))-1));

  DP(now, visited, length);
  
  FindTheWay(now, visited, length);
  
  fp2 = fopen("DP_TPS", "w");
  if(fp2 == NULL){
    printf("error opening file!\n");
    return 1;
  }
  for(i = 0; i<num; i++){
    fprintf(fp2, "%d ", path[i]);
  }
  fclose(fp2);
  free(length);
  return 0; 
}

int dist(int i, int j){
  double xd, yd;
  xd = x[i] - x[j];
  yd = y[i] - y[j];
  return (int)(sqrt(xd*xd+yd*yd)+0.5);
}

int DP(int now, int visited, int **length){
  int country;
  int res = 99999;
  if(visited == (1<<(num-1))-1){
    length[now][visited] = dist(0, now);
    return visited;
  }else{
    for(country = 1; country < num;country++){
      if(1<<(country-1) & visited)continue;
      if(res > dist(now, country) + length[country][DP(country,visited|(1<<(country-1)),length)])
      res = dist(now, country) + length[country][DP(country,visited|(1<<(country-1)),length)];
    }
    length[now][visited] = res;
    return visited;
  }
}
int i = 0;
void FindTheWay(int now, int visited, int **length){
  int country;
  int min = 0;
  int res = 99999;
  if(visited == (1<<(num-1))-1){
    return;
  }else{
    for(country = 1; country < num; country++){
      if(1<<(country-1) & visited)continue;
      if(length[country][visited|(1<<(country-1))] + dist(now, country) == length[now][visited]){
	res = length[country][visited|(1<<(country-1))];
	min = country;
	break;
      }
    }
    path[++i] = min;
    return FindTheWay(min, visited|(1<<(min-1)),length);
  }
}
