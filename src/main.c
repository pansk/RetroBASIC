/* main for RetroBASIC
 Copyright (C) 2020 Maury Markowitz
 
 This file is part of RetroBASIC.
 
 RetroBASIC is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.
 
 RetroBASIC is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with RetroBASIC; see the file COPYING.  If not, write to
 the Free Software Foundation, 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.  */

#include "retrobasic.h"
#include "parse.h"
#include "statistics.h"

static char *source_file = "";
static char *input_file = "";
static char *print_file = "";

/* simple version info for --version command line option */
static void print_version()
{
  puts("RetroBASIC 1.1");
}

/* usage, both for the user and for documenting the code below */
static void print_usage(char *argv[])
{
  printf("Usage: %s [-hvsngu] [-a number] [-t spaces] [-r seed] [-p | -w stats_file] [-o output_file] [-i input_file] source_file\n", argv[0]);
  puts("Options:");
  puts("  -h, --help: print this description");
  puts("  -v, --version: print version info");
  puts("  -u, --upper-case: convert all input to upper case");
  puts("  -a, --array-base: minimum array index, normally 1");
  puts("  -s, --slicing: turn on string slicing (turning off string arrays)");
  puts("  -n, --no-run: don't run the program after parsing");
  puts("  -g, --goto-next: if a branch target doesn't exist, go to the next line");
  puts("  -t, --tabs: set the number of spaces for comma-separated items");
  puts("  -r, --random: seed the random number generator");
  puts("  -p, --print-stats: when the program exits, print statistics");
  puts("  -w, --write-stats: on exit, write statistics to a file");
  puts("  -o, --output-file: redirect PRINT and PUT to the named file");
  puts("  -i, --input-file: redirect INPUT and GET from the named file");
}

static struct option program_options[] =
{
  {"help", no_argument, NULL, 'h'},
  {"version", no_argument, NULL, 'v'},
  {"upper-case", no_argument, NULL, 'u'},
  {"array-base", required_argument, NULL, 'a'},
  {"tabs", required_argument, NULL, 't'},
  {"random", required_argument, NULL, 'r'},
  {"slicing", no_argument, NULL, 's'},
  {"goto-next", no_argument, NULL, 'g'},
  {"input-file", required_argument, NULL, 'i'},
  {"output-file", required_argument,  NULL, 'o'},
  {"print-stats", no_argument, NULL, 'p'},
  {"write-stats", required_argument, NULL, 'w'},
  {"no-run", no_argument, NULL, 'n'},
  {0, 0, 0, 0}
};

void parse_options(int argc, char *argv[])
{
  int option_index = 0;
  int printed_help = FALSE;
  
  while(1) {
    // eat an option and exit if we're done
    int c = getopt_long(argc, argv, "hvua:t:r:i:o:w:spn", program_options, &option_index); // should match the items above, but with flag-setters excluded
    if (c == -1) break;
    
    switch (c) {
      case 0:
        // flag-setting options return 0 - these are s, p and n
        if (program_options[option_index].flag != 0)
          break;
        
      case 'h':
        print_usage(argv);
        printed_help = TRUE;
        break;
        
      case 'v':
        print_version();
        printed_help = TRUE;
        break;
        
      case 'u':
        upper_case = TRUE;
        break;
        
      case 'g':
        goto_next_highest = TRUE;
        break;
        
      case 'n':
        run_program = FALSE;
        break;
        
      case 's':
        string_slicing = TRUE;
        break;
        
      case 'p':
        print_stats = TRUE;
        break;
        
      case 'a':
        array_base = (int)strtol(optarg, 0, INT_MAX);;
        break;
        
      case 't':
        tab_columns = (int)strtol(optarg, 0, INT_MAX);;
        break;
        
      case 'i':
        input_file = optarg;
        break;
        
      case 'o':
        print_file = optarg;
        break;
        
      case 'w':
        write_stats = 1;
        stats_file = optarg;
        break;
        
      case 'r':
        random_seed = strtol(optarg, 0, INT_MAX);
        
      default:
        abort();
    }
  } // while
  
  // now see if there's a filename
  if (optind < argc)
    // we'll just assume one file if any
    source_file = argv[argc - 1];
  else
    // not always a failure, we might have just been asked for usage
    if (printed_help)
      exit(EXIT_SUCCESS);
    else
      exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  extern int yyparse(void);
  extern FILE *yyin;
  
  // turn this on to add verbose debugging
#if YYDEBUG
  yydebug = 1;
#endif
  
  // call setup to create the state needed to parse the file
  setup();

  // parse the options and make sure we got a filename somewhere
  parse_options(argc, argv);
  
  // open the file...
  yyin = fopen(source_file, "r");
  // and see if it exists
  if (yyin == NULL) {
    if (errno == ENOENT) {
      fprintf(stderr, "File not found or no filename provided.");
      exit(EXIT_FAILURE);
    } else {
      fprintf(stderr, "Error %i when opening file.", errno);
      exit(EXIT_FAILURE);
    }
  }
  // if we were able to open the file, parse it
  yyparse();
  
  // run all the lines together into a single continuous list
  // by pointing the ->next for each line to the head of the next
  // non-empty line. that way we don't have to search through the line
  // array for the next non-null entry during the run loop, we just
  // keep stepping through the ->next until we fall off the end
  {
    // look for the first entry in the lines array with a non-empty statement list
    int first_line = 0;
    while ((first_line < MAXLINE - 1) && (interpreter_state.lines[first_line] == NULL))
      first_line++;
    
    // that statement is going to be the head of the list when we're done
    GList *first_statement = interpreter_state.lines[first_line];
    
    // now find the next non-null line and concat it to the first one, and repeat
    for (int i = first_line + 1; (i < MAXLINE); i++) {
      if (interpreter_state.lines[i])
        first_statement = g_list_concat(first_statement, interpreter_state.lines[i]);
    }
    
    // and set the resulting list back into the first line
    // NOTE: do we need to do this? isn't this already there?
    interpreter_state.lines[first_line] = first_statement;
    // and keep track of this for posterity
    interpreter_state.first_line = first_line;
    
    // a program runs from the first line, so...
    interpreter_state.current_statement = first_statement;          // the first statement
    interpreter_state.current_data_statement = first_statement;     // the DATA can point anywhere
    interpreter_state.current_data_element = NULL;                  // the element within the DATA is nothing
  }
  
  // the cursor starts in col 0
  interpreter_state.cursor_column = 0;
  
  // seed the random with the provided number or randomize it
  if (random_seed > -1)
    srand(random_seed);
  else
    srand((unsigned int)time(0));
  
  // and go!
  if (run_program)
    run();
  
  // we're done, print/write desired stats
  if (print_stats || write_stats)
    print_statistics();
  
  // and exit
  exit(EXIT_SUCCESS);
}