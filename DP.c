#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define MaxNum 20

double x[MaxNum];                 /*入力した座標を格納する配列*/
double y[MaxNum];                 /* 入力した座標を格納する配列*/
int num;                                   /*訪問する点の総数*/
int length[MaxNum][(1<<(MaxNum-1))-1];     /*動的計画法に使う最適解を格納する配列*/
int path[MaxNum]={0};           /*訪問する順番を記録する配列*/

int dist(int , int);           /*二つの点の距離を計算する関数*/
int DP(int , int);            /*動的計画法を利用する関数*/
void FindTheWay(int , int);    /*訪問する順番を格納する関数*/

int main(int argc, char **argv){
  char *fname;
  FILE *fp1, *fp2;
  double data;        /*入力内容を記録する変数*/
  int c;            
  int count=0;        /*スキャンの回数を記録する関数*/
  int now = 0;        /*今にいる点の番号*/
  int visited = 0;    /*すべての点の訪問状態を格納する集合*/
  int i = 0;

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
  while(c != EOF){                   /*テキストをスキャンし、異なる部分に値を書き込む*/
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

  DP(now, visited);        /*動的計画法を使う*/

  FindTheWay(now, visited);    /*訪問順番を格納*/
  
  fp2 = fopen("DP_TPS", "w"); /*テキストに書き込んで出力*/
  if(fp2 == NULL){
    printf("error opening file!\n");
    return 1;
  }
  for(i = 0; i<num; i++){       
    fprintf(fp2, "%d ", path[i]);
  }
  fclose(fp2);
  
  return 0; 
}

int dist(int i, int j){             /*二つの点の距離を計算する関数*/
  double xd, yd;
  xd = x[i] - x[j];
  yd = y[i] - y[j];
  return (int)(sqrt(xd*xd+yd*yd)+0.5);
}

int DP(int now, int visited){        /*動的計画法*/
  int country;                       /*点の番号*/
  int res = 99999;                   /*経路の一番短い長さを記録する変数*/
  if(visited == (1<<(num-1))-1){    /*初期状態のセット*/
    length[now][visited] = dist(0, now);
    return visited;
  }else{                              /*動的計画法の具体的操作*/
    for(country = 1; country < num;country++){  /*部分経路の最短距離の走査*/     
        if(1<<(country-1) & visited)continue;      /*もう訪問したら、飛ばす*/
        if(res > dist(now, country) + length[country][DP(country,visited|(1<<(country-1)))]) 
      	  res = dist(now, country) + length[country][DP(country,visited|(1<<(country-1)))];
    }
    length[now][visited] = res;   /*部分経路の最短距離を書き込む*/
    return visited;
  }
}
int i=0;                              /* 最短巡回回路の順序を記録する変数*/
void FindTheWay(int now, int visited){
  int country;                      /*点の番号*/
  int min = 0;                     /*最短距離を持つ点*/
  if(visited == (1<<(num-1))-1){   /*全部記録した、関数から離れる*/
    return;
  }else{
    for(country = 1; country < num; country++){      /*先動的計画法で求めた最適部分解に当てはまる点の順序を記録*/
      if(1<<(country-1) & visited)continue;
      if(length[country][visited|(1<<(country-1))] + dist(now, country) == length[now][visited]){
	min = country;                /*最短距離をもつ点の記録*/
	break;
      }
    }
    path[++i] = min;
    return FindTheWay(min, visited|(1<<(min-1)));
  }
}
