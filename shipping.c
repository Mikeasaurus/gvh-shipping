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
*/

#include <stdio.h>

/*
  Number of characters available.
  Note: the run time grows super-exponentially with number of characters.
*/
const char n = 8;

// Shortcut for "n choose 2", needed for some looping when building matrices.
const char n2 = (n*(n-1))/2;

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
  matrix), determine if it's actually connected or not.
  (graphs that are not connected don't count as a relationship).
*/
int connected (char g[n][n]) {
  // First, figure out which of the characters are actually participating in
  // this relationship (have at least one connection marked with someone else)
  char participant[n];
  for (int i = 0; i < n; i++) {
    participant[i] = 0;
    for (int j = 0; j < n; j++) {
      if (g[i][j] == 1) participant[i] = 1;
    }
  }
  // Kind of a hack, but at this point immediately exclude this graph as
  // "disconnected" if first two characters (arbitrarily assigned as Fang and
  // Naser) are marked as bing  participants.
  if ((participant[0]==1) && (participant[1]==1)) return 0;
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
  // Check if all were reached.
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
    if (connected(g)) count ++;
  }
  // Print the total count.
  printf ("%d\n",count);
  return 0;
}
