#ifndef MACRO_H
#define MACRO_H


//#define DEBUG

#define panic(format, ...) \
  do { \
    fprintf(stdout, "\033[1;33m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout); \
  exit(0);
  
#define warning(format, ...) \
  do { \
    fprintf(stdout, "\033[1;33m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout);

#ifdef DEBUG
#define log2file(log_file, format, ...) \
  do { \
    fprintf(log_file, "[%s,%d,%s] " format "\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0);

#define log(format, ...) \
  do { \
    fprintf(stdout, "\033[1;34m[%s,%d,%s] " format "\033[0m\n", \
      __FILE__, __LINE__, __func__, ## __VA_ARGS__); \
  } while (0); \
  fflush(stdout);
#endif

#endif