// Name: Tristan Lopez
// SB-Analyze Program for Superball!
// Overview: This program analyzes all the possible moves that can be done
// during the Superball game. It then proceeds by making the best possible moves
// to get the highest score until it stops.
// Note: I commented the code I added and not what was already there
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include "disjoint.h"
using namespace std;

// Superball game set up
class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
    vector<int> board;
    vector<int> goals;
    vector<int> colors;
};
// error check message requested
void usage(const char *s)
{
  fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}
// Superball game set up
Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;
  if (argc != 5) usage(NULL);
  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");
  colors.resize(256, 0);
  for (i = 0; i < (int) strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }
  board.resize(r*c);
  goals.resize(r*c, 0);
  empty = 0;
  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if ((int) s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

int main(int argc, char **argv)
{
  Superball *s;
  s = new Superball(argc, argv);

  DisjointSetByRankWPC ds(s->r * s->c);

  for (int i = 0; i < s->r; i++) {
    for (int j = 0; j < s->c; j++) {
      int index = i * s->c + j;

      if (s->board[index] == '.' || s->board[index] == '*')
        continue;

      if (j < s->c - 1 && s->board[index] == s->board[index + 1]) {
        int s1 = ds.Find(index);
        int s2 = ds.Find(index + 1);
        if (s1 != s2) ds.Union(s1, s2);
      }

      if (i < s->r - 1 && s->board[index] == s->board[index + s->c]) {
        int s1 = ds.Find(index);
        int s2 = ds.Find(index + s->c);
        if (s1 != s2) ds.Union(s1, s2);
      }
    }
  }

  vector<int> setSize(s->r * s->c, 0);
  vector<bool> setHasGoal(s->r * s->c, false);
  vector<int> goalCellInSet(s->r * s->c, -1);

  for (int i = 0; i < s->r * s->c; i++) {
    if (s->board[i] == '.' || s->board[i] == '*')
      continue;

    int leader = ds.Find(i);
    setSize[leader]++;

    if (s->goals[i] == 1) {
      setHasGoal[leader] = true;
      goalCellInSet[leader] = i;
    }
  }

  printf("Scoring sets:\n");

  bool found_any = false;

  for (int i = 0; i < s->r * s->c; i++) {
    if (setSize[i] >= s->mss && setHasGoal[i]) {
      found_any = true;
      int goalCellIndex = goalCellInSet[i];
      int row = goalCellIndex / s->c;
      int col = goalCellIndex % s->c;
      printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n",
             setSize[i], s->board[i], row, col);
    }
  }

  if (!found_any) {
  }

  delete s;
  return 0;
}
