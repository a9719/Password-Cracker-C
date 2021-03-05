# include <stdio.h>
# include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <memory.h>
#include "sha256.h"
#include "sha256.c"
#include <stdbool.h>
  int exist[30];
BYTE old[6];
int count = 0;
int exist3[10000];


//function to generate lower character strings of length 6 for random brute force: Got 8 passwords
void printRandomString(int hashes[30][32]) {
  bool cracked = true;
  SHA256_CTX ctx;

  BYTE buffer[SHA256_BLOCK_SIZE];

  for (int i = 0; i < 100000000000000000; ++i) {
    if (count == 30) {
      break;
    }
    BYTE res[6];
    for (int j = 0; j < 6; j++)
      res[j] = (rand() % 26) + 97;;
    res[6] = '\0';
    sha256_init( & ctx);
    sha256_update( & ctx, res, sizeof(res));
    sha256_final( & ctx, buffer);

    for (int k = 10; k < 30; k++) {

      cracked = true;
      for (int a = 0; a < 32; a++) {
        if (buffer[a] != hashes[k][a]) {

          cracked = false;
          break;
        }
      }
      if (cracked == true && exist[k + 1] != 1) {
        ctx.data[6] = '\0';
        exist[k + 1] = 1;
        ++count;
        printf("%s %d\n", ctx.data, k + 1);
      }
    }

  }
}

//Variations of password including alternate capitalisation
void variations(char * word_list_file, int hashes[30][32]) {
  SHA256_CTX ctx;
  BYTE buffer[SHA256_BLOCK_SIZE];
  BYTE password[6];
  bool cracked = true;

  FILE * word_list;
  word_list = fopen(word_list_file, "rb");

  while (fgets(password, 7, word_list) != NULL) {
    if (count == 30) {
      break;
    }

    for (int c = 0; c < 6; ++c) {
      BYTE password1[6];
      for (int i = 0; i < 6; ++i) {
        if (i == c) {

          if (password[c] >= 65 && password[c] < 90) {
            password1[c] = password[c] + 32;
          } else if (password[c] >= 97 && password[c] < 123) {
            password1[c] = password[c] - 32;
          } else {
            password1[c] = password[c];
          }
        } else {
          password1[i] = password[i];
        }
      }
      password1[6] = '\0';

      sha256_init( & ctx);
      sha256_update( & ctx, password1, sizeof(password1));
      sha256_final( & ctx, buffer);

      for (int j = 10; j < 30; j++) {

        cracked = true;
        for (int i = 0; i < 32; i++) {
          if (buffer[i] != hashes[j][i]) {

            cracked = false;
            break;
          }
        }
        if (cracked == true && exist[j + 1] != 1) {
          ctx.data[6] = '\0';
          exist[j + 1] = 1;
          count++;
          printf("%s %d\n", ctx.data, j + 1);
        }
      }

    }

  }

  fclose(word_list);

}

//for 1 argument generate passwords
void printRandomString1(int k) {

  for (int i = 0; i < k ; ++i) {
    BYTE res[6];
    for (int j = 0; j < 6; j++)
      res[j] = (rand() % 59) + 65;;
    res[6] = '\0';
    printf("%s\n", res);
  }
}
// brute force for 4 character sets
void bruteforce(int hashes[30][32]) {
  SHA256_CTX ctx;
  BYTE buffer[SHA256_BLOCK_SIZE];
  BYTE password[4];
  bool cracked = true;

  for (int c1 = 32; c1 < 127; c1++) {
    password[0] = c1;
    if (count == 10) break;
    for (int c2 = 32; c2 < 127; c2++) {
      password[1] = c2;
      if (count == 10) break;
      for (int c3 = 32; c3 < 127; c3++) {
        password[2] = c3;
        if (count == 10) break;
        for (int c4 = 32; c4 < 127; c4++) {
          if (count == 10) break;
          password[3] = c4;
          password[4] = '\0';

          sha256_init( & ctx);
          sha256_update( & ctx, password, sizeof(password));
          sha256_final( & ctx, buffer);

          for (int i = 1; i < 11; i++) {
            cracked = true;
            for (int j = 0; j < 32; j++) {
              if (buffer[j] != hashes[i][j]) {
                cracked = false;
                break;
              }
            }
            if (cracked == true) {
              count++;
              ctx.data[4] = '\0';
              printf("%s %d\n", ctx.data, i);
            }
          }
        }
      }
    }
  }
}

//dictionary attack for 6 character words
void crack6(char * word_list_file, int hashes[30][32]) {
  SHA256_CTX ctx;
  BYTE buffer[SHA256_BLOCK_SIZE];
  BYTE password[6];
  bool cracked = true;

  char word[30];

  FILE * word_list;
  word_list = fopen(word_list_file, "rb");

  while (fgets(password, 7, word_list) != NULL) {

    password[6] = '\0';
    sha256_init( & ctx);
    sha256_update( & ctx, password, sizeof(password));
    sha256_final( & ctx, buffer);
    for (int j = 10; j < 30; j++) {

      cracked = true;
      for (int i = 0; i < 32; i++) {
        if (buffer[i] != hashes[j][i]) {

          cracked = false;
          break;
        }
      }
      if (cracked == true && exist[j + 1] != 1) {
        ctx.data[6] = '\0';
        exist[j + 1] = 1;
        ++count;
        printf("%s %d\n", ctx.data, j + 1);
      }
    }
  }
  fclose(word_list);

}

int main(int argc, char * argv[]) {
  if (argc == 1) {
    
    unsigned char buff[320];
    int hashes[30][32];
    FILE * file_4, * file_6;
    int i, a = 0, b = 0;
    file_4 = fopen("pwd4sha256", "rb");

    fread(buff, sizeof(buff), 1, file_4);

    for (i = 0; i < 320; i++) {
      if (i % 32 == 0) {
        a++;
        b = 0;
      }
      hashes[a][b] = buff[i];
      b++;
    }

    bruteforce(hashes);
    unsigned char buff6[640];
    file_6 = fopen("pwd6sha256", "rb");
    fread(buff6, sizeof(buff6), 1, file_6);
    for (i = 0, a = 9; i < 640; ++i) {
      if (i % 32 == 0) {
        a++;
        b = 0;
        
      }
      hashes[a][b] = buff6[i];

      b++;
    }
    crack6("common_passwords.txt", hashes);
    variations("common_passwords.txt", hashes);
    printRandomString(hashes);

    fclose(file_4);
    fclose(file_6);
  }
  if (argc == 2) {
    int k = atoi(argv[1]);
    printRandomString1(k);

  }
  if (argc == 3) {
    FILE * password_file;
    password_file = fopen(argv[1], "rb");
    char word[10000];

    int i, a = 0, b = 0;
    fseek(password_file, 0L, SEEK_END);
    int sz = ftell(password_file);
    rewind(password_file);
    int hashes[sz / 32][32];
    unsigned char buff[sz];
    
    fread(buff, sizeof(buff), 1, password_file);
    for (i = 0; i < sz; i++) {
      if (i % 32 == 0 && i>0) {
        a++;
        b = 0;

      }

      hashes[a][b] = buff[i];

      b++;
    }
    bool cracked=true;
    FILE *word_list;
    SHA256_CTX ctx;
  	BYTE buffer[SHA256_BLOCK_SIZE];
    word_list=fopen(argv[2],"rb");
    while( fscanf(word_list, "%s", word) != EOF ){
    	sha256_init(&ctx);
        sha256_update(&ctx, word, strlen(word));
        sha256_final(&ctx, buffer);
        for (int i = 0; i < sz/32; i++) {
            cracked = true;
            for (int j = 0; j < 32; j++) {
              if (buffer[j] != hashes[i][j]) {
                cracked = false;
                break;
              }
            }
            if (cracked == true && exist3[i+1]!=1) {
              count++;
              ctx.data[strlen(word)] = '\0';
              exist3[i+1]=1;
              printf("%s %d\n", ctx.data, i+1);
            }
          }

    }



  }
  return 0;

}

  
