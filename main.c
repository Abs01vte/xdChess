#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool alternating = true;
bool middle = false;
bool infinite = true;
int maxMoves = -1;
int maxBytes = -1;
FILE* file1=NULL;
FILE* file2=NULL;

void openFiles(int argc, char **argv){
  //TODO: files with dashes
  for(int i = 1; i<argc; i++){
    const char* curArg = argv[i];
    if(curArg[0] == '-'){
      if(curArg[1] == 0){
        if(file1 == NULL){
          file1 = stdin;
        }
        else{
          file2 = stdin;
        }
      }
    }
    else{
      if(file1 == NULL){
        file1 = fopen(curArg, "r");
        if(file1 == NULL){
          fprintf(stderr, "file cannot be found:%s\n", curArg);
        }
      }
      else{
        file2 = fopen(curArg, "r");
        if(file2 == NULL){
          fprintf(stderr, "file cannot be found:%s\n", curArg);
        }
      }
    }
  }
  if(file1 == NULL){
    file1=stdin;
  }
}

int main(int argc, char **argv){
  openFiles(argc, argv);
  char buff[512];
  int bytesRead = 0;
  for(int i=0; i<512; i++){
    char* retVal = fgets(buff+bytesRead, 512-bytesRead, file1);
    if(retVal == NULL){
      break;
    }
    bytesRead += strlen(buff+bytesRead)-1;

  }
  printf(buff);
  return 0;
}
