#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct options {
  char* input_filename;
  char* formula;
} Options;

Options * options_parse_args (int argc, char *argv[]);

void options_free (Options *options);

#endif /* OPTIONS_H */
