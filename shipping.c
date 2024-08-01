/*
  Code for counting number of possible relationships, including generalized
  polycules where not every participant is directly connected to every other
  participant (but all are at least indirectly connected).
  For more specific case for counting only fully-conncect polycules, see the
  Python version.

  Assumes two of the characters are siblings, and are excluded from existing
  in the same polycule construct.

  The basic approach for this method is to define each potential relationship
  combination as an NxN symmetric boolean matrix, where each matrix element
  represents a yes/no (1/0) of whether those participants are directly
  connected in the relationship.
  All possible matrices are iterated over, and checked if they are connected
  together (connected graph).  If so, then that relationship gets added to the
  total count.  The resulting count is printed at the end, but individual
  relationships are not printed since that would be hella complicated to do
  with ascii art.

  If the "lists" flag is set to 1, then instead of counting individual
  relationships, it will count possible lists of parallel relationships that
  could exist at the same time
  (E.g. Rosa <--> Fang <--> Naomi  and  Reed <--> Naser)
*/

#include <stdio.h>

/*
  Number of characters available.
  Note: the run time grows super-exponentially with number of characters.
*/
static const char n = 8;

/*
  Counting individual relationships or lists of mutually-exclusive
  relationships that can exist at the same time?
*/
static const char lists = 0;

// Shortcut for "n choose 2", needed for some looping when building matrices.
static const char n2 = (n*(n-1))/2;

/*
  Internal helper method: given a potential relationship (as NxN boolean
  matrix), traverse through it and mark which characters are reachable from
  some given reference participant.
  This is used further down to determine if the graph is actually connected
  or not.
*/
void traverse (char g[n][n], char visited[n], char i) {
  visited[i] = 1;
  for (int j = i+1; j != i; j = (j+1)%n) {
    if ((g[i][j] == 1) && (visited[j] == 0)) traverse (g, visited, j);
  }
  return;
}

/*
  Internal helper method: given a potential relationship (as NxN boolean
  matrix), determine if it's valid.
  If counting individual relationships, check if it's actually connected or not.
  If counting lists of parallel relationships, only constraint is that Fang and 
  Naser can't be within the same subgroup.
*/
int isvalid (char g[n][n]) {
  // First, figure out which of the characters are actually participating in
  // this relationship (have at least one connection marked with someone else)
  char participant[n];
  for (int i = 0; i < n; i++) {
    participant[i] = 0;
    for (int j = 0; j < n; j++) {
      if (g[i][j] == 1) participant[i] = 1;
    }
  }
  // If doing lists of parallel relationships, only constraint is that Fang
  // and Naser can be reachable from the same graph.
  if ((lists==1) && ((participant[0]==0)||(participant[1]==0))) return 1;
  // If doing individual relationships, can't have Fang and Naser participating
  // at the same time at all.
  if ((lists==0) && ((participant[0]==1)&&(participant[1]==1))) return 0;

  // The rest of the code below will check if all participants can be reached
  // by starting from the first participant (implying the graph is connected).
  char visited[n];
  for (int i = 0; i < n; i++) visited[i] = 0;
  // Find first participant.
  {
    int p;
    for (p = 0; participant[p] == 0; p++);
    traverse (g, visited, p);
  }
  // If counting lists, and we're still in this routine, that means both Fang
  // and Naser were in this list of relationships.
  // Make sure they weren't in the same relationship (could be in different
  // relationships in the list).
  if (lists==1) {
    return (visited[1] == 0);  // Was Naser reachable from Fang?
  }
  // If counting individual relationships, check if all participants were
  // reachable
  for (int i = 0; i < n; i++) {
    if ((participant[i]==1) && (visited[i]==0)) {
      return 0;
    }
  }
  return 1;
}

/*
  Main part of code.
  Counts all viable relationships (couples and generalized polycules).
*/
int main (int argc, char *argv[]) {
  int count = 0;
  // Define the NxN matrix to represent a relationship.
  char g[n][n];
  /*
    Main loop for iterating over every relationship.
    Essentially flips a 1/0 swith for each (n choose 2) possible inter-character
    connection.  It does this by looping over the first 2**(Nc2) numbers and
    using the binary representation for filling in the upper triangle of the
    matrix.  The lower triangle is then defined as the transpose of that since
    the matrix is necessarily symmetric.
    The main diagnonal is set to zeros - can't be in a "relationship" with
    yourself, e.g. no self-connected nodes in the graph.
    Start at 1 to skip null case, which would complicate other parts of code
    to handle properly.
  */
  for (int ind = 1; ind < (1<<n2); ind++) {
    int tmp = ind;
    char i = 0;
    char j = 1;
    // Build the relationship matrix based on binary representation of "ind".
    for (char k = 0; k < n2; k++) {
      g[i][j] = (tmp%2);
      tmp >>= 1;
      j += 1;
      // Skipping over lower triangle for now, only upper filled.
      if (j == n) { j = i+2; i += 1; }
    }
    // Now fill in lower triangle and zero out diagonal.
    for (i = 0; i < n; i++) {
      g[i][i] = 0;
      for (j = 0; j < n; j++) {
        if (i > j) g[i][j] = g[j][i];
      }
    }
    // If we managed to make a coherent polycule (or couple), then add it to
    // the count.
    if (isvalid(g)) count ++;
  }
  // Print the total count.
  printf ("%d\n",count);
  return 0;
}
