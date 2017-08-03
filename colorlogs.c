#include <stdio.h>
#include <stdarg.h>

/** Color codes **/
#define COLOR_RED     "\x1b[31m"
#define COLOR_CRIM    "\x1b[31;1m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_GOLD    "\x1b[33;1m"
#define COLOR_BLUE    "\x1b[34;1m"
#define COLOR_MAGENTA "\x1b[35;1m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_WHITE   "\x1b[37;1m"
#define COLOR_RESET   "\x1b[0m"

void LOG_RED(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_RED);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_CRIM(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_CRIM);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_GREEN(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_GREEN);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_BLUE(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_BLUE);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_YELLOW(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_YELLOW);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_GOLD(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_GOLD);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_MAGENTA(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_MAGENTA);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_CYAN(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_CYAN);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

void LOG_WHITE(const char* format, ...) { 
  va_list args;
  va_start (args, format);
  printf (COLOR_WHITE);
  vprintf (format, args);
  printf (COLOR_RESET);
  va_end (args);
}

