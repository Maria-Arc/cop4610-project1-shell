# cop4610-project1-shell

Project 1 - Build a shell with fancy features
COP4610 Project 1: Shell

Group 11
MEMBERS Maria Arcis - [mea23a] Rebecca Gorman - [rkg23a] Nicholas Brion - [npb22c]

DIVISION OF LABOR (BEFORE) Part 1: Prompt - Maria, Rebecca Part 2: Environment Variables - Rebecca, Nicholas Part 3: Tilde Expansion - Maria, Rebecca Part 4: $PATH Search - Maria, Rebecca Part 5: External Command Execution - Rebecca, Nicholas Part 6: I/O Redirection - Rebecca, Nicholas Part 7: Piping - Maria, Nicholas Part 8: Background Processing - Maria, Nicholas Part 9: Internal Command Execution - Maria, Nicholas

DIVISION OF LABOR (AFTER) Part 1: Prompt - Maria Part 2: Environment Variables - Rebecca Part 3: Tilde Expansion - Maria Part 4: $PATH Search - Maria and Rebecca Part 5: External Command Execution - Rebecca Part 6: I/O Redirection - Rebecca Part 7: Piping - Nicholas Part 8: Background Processing - Nicholas Part 9: Internal Command Execution - Nicholas

FILE LISTING Makefile builds the shell README.md this file bin/shell executable (created by make) obj/ object files (created by make) include/ builtins.h, env.h, executor.h, expand.h, external.h, jobs.h, pipeline.h, redirection.h, shell.h src/ main.c entry point, prompt, main loop tokenizer.c splits input into tokens env.c environment variables expand.c tilde expansion external.c $PATH search and external commands redirection.c I/O redirection pipeline.c piping jobs.c background processing builtins.c internal commands executor.c runs parsed commands

HOW TO COMPILE AND RUN: make (builds and puts the executable in bin/) ./bin/shell (runs the shell) make clean (removes build files)

DEVELOPMENT LOG
Sep 24-25 - Pull request #1 (Maria) Prompt, tilde expansion, and $PATH search (Parts 1, 3, and 4), which the group then reviewed together.

Sep 27-28 - Pull request #2 (Rebecca) Environment variables, external command execution, and I/O redirection (Parts 2, 5, and 6).

Sep 28 - Pull request #3 (Nicholas) Piping, background processing, and internal command execution (Parts 7, 8, and 9).4

GROUP MEETINGS
Meeting 1 - [9/17] - in person - We read through the project spec and split the nine parts between the three of us. Maria took the prompt, tilde expansion, and $PATH search. Rebecca took environment variables, external command execution, and I/O redirection. Nicholas took piping, background processing, and internal commands. We also agreed on the folder layout (src/, include/, obj/, bin/) and that each part gets its own .c and .h file so we would not have merge conflicts.

Meeting 2 - [9/24] - in person - We checked progress. Maria walked us through the finished prompt and tilde expansion code so everyone could review it, and $PATH search was almost done, with Rebecca helping finish it. Rebecca planned to finish environment variables and external command execution next, then I/O redirection. Nicholas worked on piping, background processing, and internal commands. We also went over the README and grading rubric and divided up the remaining cleanup (comments, line width, removing debug prints).
