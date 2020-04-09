#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef VERBOSE_ENABLED
#define oas(...) fprintf(stderr, __VA_ARGS__)
#else
#define oas(...) \
  do {           \
  } while (false);
#endif
struct request 
{
  int pid;
  int* rqs;
} * ri;
struct Statesys {
  int rc;
  int pct;
  int* ar;
  int** at;
  int** mt;
} * gls, *gts;
void freeres() 
{
  for (int a = 0; a < gls->pct; a++) 
  {
    free(gls->at[a]);
    free(gls->mt[a]);
    free(gts->mt[a]);
    free(gts->at[a]);
  }
  free(gls->at);
  free(gts->at);
  free(gls->mt);
  free(gts->mt);
  free(gls->ar);
  free(gts->ar);
  free(gls);
  free(gts);
}
void input() 
{
  oas();
  gls =(struct Statesys*)malloc(sizeof(struct Statesys));
  gts =(struct Statesys*)malloc(sizeof(struct Statesys));
  oas();
  printf("Enter Total Process: ");
  scanf("%d", &(gls->pct));
  gts->pct = gls->pct;
  printf("\nEnter Total Resource: ");
  scanf("%d", &(gls->rc));
  gts->rc = gls->rc;
  oas();
  printf("\nEnter Allocated Resource\n");
  oas();
  gls->at =(int**)malloc(gls->pct * sizeof(int*));
  gts->at =(int**)malloc(gts->pct * sizeof(int*));
  for (int a = 0; a < gls->pct; a++) 
  {
    gls->at[a] =(int*)malloc(gls->rc * sizeof(int));
    gts->at[a] =(int*)malloc(gts->rc * sizeof(int));
  }
  oas();
  for (int a = 0; a < gls->pct; a++)
    for (int b = 0; b < gls->rc; b++) 
	{
      scanf("%d", &(gls->at[a][b]));
      gts->at[a][b] =gls->at[a][b];
    }
  oas();
  gls->mt =(int**)malloc(gls->pct * sizeof(int*));
  gts->mt =(int**)malloc(gts->pct * sizeof(int*));
  for (int a = 0; a < gls->pct; a++) 
  {
    gls->mt[a] =(int*)malloc(gls->rc * sizeof(int));
    gts->mt[a] =(int*)malloc(gts->rc * sizeof(int));
  }
  oas();
  printf("\nEnter Maximum Resource\n");
  for (int a = 0; a < gls->pct; a++)
    for (int b = 0; b < gls->rc; b++) 
	{
      scanf("%d", &(gls->mt[a][b]));
      gts->mt[a][b] =gls->mt[a][b];
    }
  oas();
  gls->ar =(int*)malloc(sizeof(int) * gls->rc);
  gts->ar =(int*)malloc(sizeof(int) * gts->rc);
  oas();
  printf("\nEnter available resource\n");
  for (int a = 0; a < gls->rc; a++) 
  {
    scanf("%d", &(gls->ar[a]));
	gts->ar[a] =gls->ar[a];
  }
};
bool allocatableve(int* a, int* b, int* c, int len)
{
  for (int iter = 0; iter < len; iter++)
    if (a[iter] + b[iter] < c[iter]) return false;
  return true;
}
void freeall(int* a, int* b, int* c, int len) 
{
  assert(allocatableve(a, b, c, len));
  for (int iter = 0; iter < len; iter++) 
  {
    a[iter] += b[iter];
    b[iter] = 0;
  }
}
bool grantt(int* a, int* b, int* c, int len) 
{
  for (int t = 0; t < len; t++)
    if (a[t] + b[t] > c[t]) return false;
  return true;
}
void restore() 
{
  oas();
  for (int a = 0; a < gls->rc; a++)
    gts->ar[a] =gls->ar[a];
  oas();
 for (int a = 0; a < gls->pct; a++)
    for (int b = 0; b < gls->rc; b++)
      gts->at[a][b] =gls->at[a][b];
}
bool solve() {
  oas();
  int nonex = gls->pct;
  bool dl = false;
  bool has_completed[gls->pct];
  for (int a = 0; a < gls->pct; a++)
    has_completed[a] = false;
  while (nonex) {
    dl = true;
    for (int a = 0; a < gls->pct; a++) {
      if (has_completed[a]) continue;
      oas("\nChecking if P%d can be allocated for execution", a);
      bool res =allocatableve(gts->ar,gts->at[a],gts->mt[a],gts->rc);
      if (res) {
        has_completed[a] = true;
        nonex--;
        dl = false;
        oas("\nAllocating and releasing resources for P%d", a);
        freeall(gts->ar,gts->at[a],gts->mt[a],gts->rc);
      } else {
        oas("\nCannot satisfy needs for P%d. Skipping", a);
      }
    }
    if (dl) {
      oas("\ndeadlock");
      return false;
    }
  }
  return true;
};
void reqcountt(int* target) {
  printf("\nenter no of req arrive");
  scanf("%d", target);
}
void ask_requests(int n) {
  for (int t = 0; t < n; t++) {
    printf("\nRequest %d : ", t + 1);
    int p_c;
    printf("\nenter the process id for which the req have arrived");
    scanf("%d", &p_c);
    p_c--;
    if (p_c > gls->pct) {
      printf("\nYou enterred wrong proccess id enter correct");
      t--;
      continue;
    }
    printf("\nEnter %d integers each for each resource type ",gls->rc);
    ri = (struct request*)malloc(sizeof(struct request));
    ri->pid = p_c;
    ri->rqs =
        (int*)malloc(sizeof(int) * gls->rc);
    for (int a = 0; a < gls->rc; a++)
      scanf("%d", &(ri->rqs[a]));
    if (grantt(gls->at[p_c],ri->rqs,gls->mt[p_c],gls->rc)) {
      bool flag = true;
      for (int a = 0; a < gls->rc; a++)
        if (gts->ar[a] <
            ri->rqs[a]) {
          flag = false;
          break;
        }
      if (flag)
        printf("\nResource granted");
      else
        printf("\nPartially Request grantedallocation limit increased");
      for (int a = 0; a < gls->rc; a++) 
	  {
        if (flag)
          gts->ar[a] -=ri->rqs[a];
        gts->at[p_c][a] +=ri->rqs[a];
      }
      if (!solve())
        printf("\nDEADLOCK occuredd");
      else
        printf("\nSafe state");
    }
    else 
	{
      printf("\nRequest for the resources denied");
    }
    free(ri->rqs);
    free(ri);
    restore();
    printf("\n");
  }
}
int main() 
{
  input();
  if (solve())
    printf("\nInitially system is in Safe State");
  else 
  {
    printf("\nDeadlock.");
    return 0;
  }
  restore();
  int n;
  reqcountt(&n);
  ask_requests(n);
  freeres();
  return 0;
}
