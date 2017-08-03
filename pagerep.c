/*============================================================================*
  pagerep.c
	
  This program compares the performance of different page replacement 
  algorithms.
  
  A makefile is provided, use the 'make' command to compile
  
  Author: Ben Pogrund
*============================================================================*/

/* libraries */
#include <stdio.h>
#include <colorlogs.h>    // Added for readability

/* macros */
#define PAGE_COUNT 5

/* user input handling */
void get_input(char* refstr);
int  valid_input(char input);
void print_str(char* refstr);
void print_hits(int* hits, int length);

/* page replacement algorithms */
int paging_test(const char* refstr, int frameCount, 
                int (*swap_heuristic)(const char*, int*, int, int));

int optimal_swap(const char* refstr, int* frames, int frameCount, 
                 int startPosition);
                 
int lru_swap(const char* refstr, int* frames, int frameCount, 
             int startPosition);

/* helper functions */
int getlength(const char* refstr);  
int _atoi(char input);  // copies out a single digit, and converts it to int

/*===========================================================================*/
int main () {
  char refstr[31];                   // 30 characters + null terminator
  int frameCount = 0;                // input from user
  int fault_count = 0;               // fault counts from paging tests
  
  // Input handling
  printf   ("\n");
  LOG_GOLD ("#------------------------------------------------------#\n\n");
  LOG_WHITE("Enter up to 30 page indices [0 - 5]\n");
  LOG_GREEN(" > ");
  get_input(refstr);
  printf   ("\n");
  
  LOG_CYAN ("Ref String (%d entries):\n", getlength(refstr));
  printf   ("   ");
  print_str(refstr);
  printf   ("\n");
  
  
  LOG_WHITE("Enter number of frames:\n");
  LOG_GREEN(" > ");
  scanf    ("%d", &frameCount);      // Not safe with invalid input
  printf   ("\n");
  
  // Algorithm tests
  LOG_CYAN ("Test results:\n");
  
  LOG_WHITE("   Optimal Algorithm:\n");
  fault_count = paging_test(refstr, frameCount, optimal_swap);
  printf("      [ %d faults ]\n\n", fault_count);
  
  LOG_WHITE("   LRU Algorithm:\n");
  fault_count = paging_test(refstr, frameCount, lru_swap);
  printf("      [ %d faults ]\n\n", fault_count);
  
  LOG_GOLD ("#------------------------------------------------------#\n\n");
  return 0;
}

void get_input(char* refstr) {
  int i = 0;
  char input;
  
  while ((input = getchar()) != '\n' || i == 0) {
    if (valid_input(input)) {
      refstr[i++] = input;
      if (i == 30) { 
        refstr[31] = '\0';
        while ((input = getchar()) != '\n')
          /* eat through remaining input */
          ;
        return;
      }
    }
  }
  refstr[i] = '\0';
}

int valid_input(char input) {
  return (atoi(&input) > 0 && atoi(&input) < PAGE_COUNT + 1);
}

void print_str(char* refstr) {
  int i = 0;
  while (refstr[i+1] != '\0') {
    printf("%c, ", refstr[i++]);
  }
  printf("%c\n", refstr[i]);
}

void print_hits(int* hits, int length) {
  int i = 0;
  for (i = 0; i < length-1; i++) {
    (hits[i]) ? LOG_GREEN("o  ") : LOG_CRIM("x  ");
  }
  (hits[i]) ? LOG_GREEN("o\n") : LOG_CRIM("x\n");
}

/*===========================================================================*/

int paging_test(const char* refstr, int frameCount, 
                int (*swap_heuristic)(const char*, int*, int, int)) 
{
  int fault_count = 0;
  int frames[frameCount];
  int hits[getlength(refstr)];
  
  // initialize page indices in frame set
  int i;
  for (i = 0; i < frameCount; i++)
    frames[i] = 0;
  
  // Paging process
  int j = 0; i = 0; 
  while (refstr[i] != '\0') {
    int frame_id = -1;
    int open_id = -1;
    int swap_id;
    
    // Look up page in frame table
    for (j = 0; j < frameCount; j++) {
      if (frames[j] == _atoi(refstr[i])) {
        frame_id = j;
        break;
      } else if (frames[j] == 0) {
        open_id = j;
      }
    }
    
    if (frame_id == -1) {
      // Page miss
      fault_count++;                             
      hits[i] = 0;
      if (open_id != -1) {
        // Insert to open frame
        frames[open_id] = _atoi(refstr[i]);      
      } else {
        // Swap due to no open frames
        swap_id = swap_heuristic(refstr, frames, frameCount, i);
        frames[swap_id] = _atoi(refstr[i]);      
      }
    } else {
      // Page hit
      hits[i] = 1;                               
    }
    
    /* ---- for debugging ----- */
    /**
    int k;
    printf("<");
    for (k = 0; k < frameCount; k++) printf ("%d", frames[k]);
    printf(">\n");
    **/
    
    i++;
  }
  
  printf("   "); print_str(refstr);
  printf("   "); print_hits(hits, getlength(refstr));
  printf("\n");
  
  return fault_count;
}

int optimal_swap(const char* refstr, int* frames, int frameCount, 
                 int startPosition) 
{
  int next_use[PAGE_COUNT+1];
  int pendingCount = frameCount-1;
  int i;
  
  // Initialize next_use (1 means usable, 0 means not)
  for (i = 1; i <= PAGE_COUNT; i++) {
    next_use[i] = 0;
  }
  
  // Enable entries that are in frame set (in main memory)
  for (i = 0; i < frameCount; i++) {
    next_use[frames[i]] = 1;
  }
  
  // This method looks into the future
  i = startPosition+1;
  while (i < getlength(refstr) && pendingCount) {
    if (next_use[_atoi(refstr[i])] == 1) {
      next_use[_atoi(refstr[i])] = 0;
      pendingCount--;
    }
    i++;
  }
  
  // Find a good index
  for (i = 1; i <= PAGE_COUNT; i++) {
    if (next_use[i] == 1) { 
      int k;
      for (k = 0; k < frameCount; k ++) {
        if (frames[k] == i) return k;
      }
    }
  }
  
  fprintf(stdout, "Optimal swap error");
  return -1;
}

int lru_swap(const char* refstr, int* frames, int frameCount, 
             int startPosition) 
{
  int last_use[PAGE_COUNT+1];
  int pendingCount = frameCount-1;
  int i;
  
  // Initialize last_use (1 means usable, 0 means not)
  for (i = 1; i <= PAGE_COUNT; i++) {
    last_use[i] = 0;
  }
  
  // Enable entries that are in frame set (in main memory)
  for (i = 0; i < frameCount; i++) {
    last_use[frames[i]] = 1;
  }
  
  // This method looks into the future
  i = startPosition-1;
  while (i >= 0 && pendingCount) {
    if (last_use[_atoi(refstr[i])] == 1) {
      last_use[_atoi(refstr[i])] = 0;
      pendingCount--;
    }
    i--;
  }
  
  // Find a good index
  for (i = 1; i <= PAGE_COUNT; i++) {
    if (last_use[i] == 1) { 
      int k;
      for (k = 0; k < frameCount; k ++) {
        if (frames[k] == i) return k;
      }
    }
  }
  
  fprintf(stdout, "LRU swap error");
  return -1;
}

/*===========================================================================*/

int getlength(const char* refstr) {
  int i = 0;
  while (refstr[i] != '\0') i++;
  return i;
}

int _atoi(char input) {
  // atoi for single digits, 0-9
  return input-48;
}

/*===========================================================================*/































