#include <stdio.h>
const char exclude = 0;
const char exclude_direct = 1;
const char n = 8;
const char n2 = (n*(n-1))/2;

void traverse (char g[n][n], char visited[n], char i) {
  visited[i] = 1;
  for (int j = i+1; j != i; j = (j+1)%n) {
    if ((g[i][j] == 1) && (visited[j] == 0)) traverse (g, visited, j);
  }
  return;
}

int connected (char g[n][n]) {
  if ((exclude_direct==1) && (g[0][1]==1)) return 0;
  char participant[n];
  for (int i = 0; i < n; i++) {
    participant[i] = 0;
    for (int j = 0; j < n; j++) {
      if (g[i][j] == 1) participant[i] = 1;
    }
  }
  if ((exclude==1) && (participant[0]==1) && (participant[1]==1)) return 0;
  char visited[n];
  for (int i = 0; i < n; i++) visited[i] = 0;
  // Find first participant.
  {
    int p;
    for (p = 0; participant[p] == 0; p++);
    traverse (g, visited, p);
  }
  // Check if all were reached.
  for (int i = 0; i < n; i++) {
    if ((participant[i]==1) && (visited[i]==0)) {
      return 0;
    }
  }
  return 1;
}

int main (int argc, char *argv[]) {
  int count = 0;
  char g[n][n];
  // Start at 1 to skip null case.
  for (int ind = 1; ind < (1<<n2); ind++) {
    int tmp = ind;
    char i = 0;
    char j = 1;
    for (char k = 0; k < n2; k++) {
      g[i][j] = (tmp%2);
      tmp >>= 1;
      j += 1;
      if (j == n) { j = i+2; i += 1; }
    }
    for (i = 0; i < n; i++) {
      g[i][i] = 0;
      for (j = 0; j < n; j++) {
        if (i > j) g[i][j] = g[j][i];
      }
    }
/*
    printf ("%d\n",ind);
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) printf ("%d ",g[i][j]);
      printf ("\n");
    }
    printf ("\n");
*/
    if (connected(g)) count ++;
  }
  printf ("%d\n",count);
  return 0;
}
