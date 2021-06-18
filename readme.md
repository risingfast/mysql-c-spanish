#### Project:
    mysql-c-spanish
#### Description:
    Spanish language practice and training
#### Author:
    Geoffrey Jarman
#### Started:
    05-Apr-2021
#### References:
    https://qnaplus.com/how-to-access-mysql-database-from-c-program/
#### Prerequisites:
    None
#### Installaton:
    Create a local repository
    ./mysql-c-spanish from local directory
#### Compilation:
    make, or ...
    gcc -g -o mysql-c-spanish $(mysql_config --cflags) mysql-c-spanish.c $(mysql_config --libs) cs50.c rf50.c
#### Log:
    05-Apr-2021 started
    05-Apr-2021 reviewed all
    05-Apr-2021 consolecode
    05-Apr-2021 change table to Spanish Words
    09-Apr-2021 add clear page and print heading
    30-Apr-2021 list words and list tests for words
    01-May-2021 fix last block pause display
    02-May-2021 implement options
    02-May-2021 clear the buffer on getchar() calls
    12-May-2021 create a function to pause and continue
    12-May-2021 globalize connection to database
    12-May-2021 add spanish word length to test lines
    14-May-2021 implement reference filter option
    15-May-2021 add an 'all' choice for reference filters
    15-May-2021 Implement a group filter with an all option
    16-May-2021 remove Elipsis from option choices for references and groups
    16-May-2021 decode filters and add to list headings
    16-May-2021 restrict tests to a single reference filter and decode from options
    17-May-2021 show word count on reference option list
    17-May-2021 show word count on group option list
    17-May-2021 show filter disabled or enabled on list and test hearders
    17-May-2021 add an option to randomize words
    20-May-2021 Create test logs
    21-May-2021 fix spacing on name column for reference and group names
    27-May-2021 add a countdown to tests
    27-May-2021 add options to suppress text lengths and countdowns
    28-May-2021 create a report of test counts
    28-May-2021 list only finished tests (non-null end times)
    14-Jun-2021 moved to a git project with separate rf50.h and rf50.c files
    15-Jun-2021 new makefile
    16-Jun-2021 password connection check
    16-Jun-2021 standardize string prefixes
    18-Jun-2021 code review and cleanup
#### Enhancements: (0)
