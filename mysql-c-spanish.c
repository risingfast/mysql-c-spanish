//  mysql-c-spanish.c -- console application to maintain and show spanish language translations
//  Author: Geoffrey Jarman
//  Started: 05-Apr-2021
//  Reference:
//      https://qnaplus.com/how-to-access-mysql-database-from-c-program/
//  Compilation
//      gcc -g -o mysql-c-spanish $(mysql_config --cflags) mysql-c-spanish.c $(mysql_config --libs) cs50
//  Log:$
//      05-Apr-2021 started
//      05-Apr-2021 reviewed all
//      05-Apr-2021 consolecode
//      05-Apr-2021 change table to Spanish Words
//      09-Apr-2021 add clear page and print heading
//      30-Apr-2021 list words and list tests for words
//      01-May-2021 fix last block pause display
//      02-May-2021 implement options
//      02-May-2021 clear the buffer on getchar() calls
//      12-May-2021 create a function to pause and continue
//      12-May-2021 globalize connection to database
//      12-May-2021 add spanish word length to test lines
//      14-May-2021 implement reference filter option
//      15-May-2021 add an 'all' choice for reference filters
//      15-May-2021 Implement a group filter with an all option
//      16-May-2021 remove Elipsis from option choices for references and groups
//      16-May-2021 decode filters and add to list headings
//      16-May-2021 restrict tests to a single reference filter and decode from options
//      17-May-2021 show word count on reference option list
//      17-May-2021 show word count on group option list
//      17-May-2021 show filter disabled or enabled on list and test hearders
//      17-May-2021 add an option to randomize words
//      20-May-2021 Create test logs
//      21-May-2021 fix spacing on name column for reference and group names
//      27-May-2021 add a countdown to tests
//      27-May-2021 add options to suppress text lengths and countdowns
//      28-May-2021 create a report of test counts
//      28-May-2021 list only finished tests (non-null end times)
//  Enhancements:

// includes and defines

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs50.h"
#include <ctype.h>
#include <stdbool.h>

#define SQL_LEN_L 5000
#define SQL_LEN_S 1000
#define DFLT_PAGE_LINES 80

// function prototypes

int  fListWordFields(char *);                              // show all spanish words on the console
int  fListTests(char*);                                               // show completed test counts
int  fTestWordFields(char *);                       // test each spanish translation on the console
void fSetOptions(char *);                                         // display and set global options
void fRetitleConsole(char *);                                      // clear and retitle the console
void fPressEnterToContinue();                                                 // pause and continue
void fPrintMainMenu();                                               // print the main menu options
void fChooseReferenceID(char *);                                           // choose a Reference ID
void fChooseGroupID(char *);                                                   // choose a Group ID
int  fStartTest(void);                                  // create a test log and return the Test ID
void fEndTest(int);                                     // create a test log and return the Test ID

// global variables

int  intListPageLines = DFLT_PAGE_LINES;                             // default display page length
char charListPageWidth = 'N';                                                 // display page width
char charReferenceFilterEnabled = 'N';                                  // reference filter enabled
int  intReferenceFilter = 0;                                                 // reference filter ID
char strReferenceFilterDecode[100] = {'\0'};                   // decoded name for reference filter
char charGroupFilterEnabled = 'N';                                          // group filter enabled
char strGroupFilterDecode[100] = {'\0'};                           // decoded name for group filter
char charRandomizeWordLists = 'N';                                // option to randomize word lists
char charShowPhraseLengths = 'N';                                  // option to show phrase lengths
char charShowPhraseCountdown = 'N';                              // option to show phrase countdown
int  intGroupFilter = 0;                                                         // group filter ID

char *server = "192.168.0.13";                                           // mySQL server IP address
char *user = "gjarman";
char *password = "Mpa4egu$";
char *database = "risingfast";                                               // mySQL database name
char strSQL0[SQL_LEN_L] = {'\0'};                                                //SQL query string
char strSQL1[SQL_LEN_S] = {'\0'};                                                //SQL query string
char strSQL2[SQL_LEN_S] = {'\0'};                                                //SQL query string
char strSQL3[SQL_LEN_S] = {'\0'};                                                //SQL query string
char strSQL4[SQL_LEN_S] = {'\0'};                                                //SQL query string

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

// main program

int main(int argc, char *argv[])
{
// variable declarations

    char charMainChoice = '0';                                                  // main menu choice
    char *strPrgNme = argv[0] + 2;

    fRetitleConsole(strPrgNme);

// Initialize a connection and connect to the database

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0))
    {
        printf("\n");
        printf("Failed to connect to MySQL Server %s in module %s()", server, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        fPressEnterToContinue();
        printf("\n");
        return -1;
    }

// print the main menu to the console

    while (strchr("5xX", charMainChoice) == NULL)
    {
        fRetitleConsole(strPrgNme);
        fPrintMainMenu();
        charMainChoice = GetChar();
        if(strchr("1lL", charMainChoice) != NULL)
        {
            printf("\n");
            fListWordFields(strPrgNme);
            printf("\n");
        }
        if(strchr("2sS", charMainChoice) != NULL)
        {
            printf("\n");
            fListTests(strPrgNme);
            printf("\n");
        }
        else if(strchr("3tT", charMainChoice) != NULL)
        {
            printf("\n");
            fTestWordFields(strPrgNme);
            printf("\n");
        }
        else if(strchr("4pP", charMainChoice) != NULL)
        {
            printf("\n");
            fSetOptions(strPrgNme);
            printf("\n");
        }
    }

    mysql_close(conn);
    printf("\n");
    return 0;
}

int fListWordFields(char *strPrgNme)
{

// variable declarations

    int i = 0;                                                                      // loop counter
    int intRowCount = 0;                                   // number of rows printed to the console
    int intColCount = 0;                                   // number of columns returned in a query
    int *intLengths = NULL;                                               // array of column widths
    int intRowsInResult = 0;                                          // rows of data in result set
    char charResp = '0';                                       // console response to test for exit

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Words");
    printf("\n\n");

// define and send SQL query

    sprintf(strSQL1, "SELECT `Word ID`"
                         ", `Word English`"
                         ", `Word Spanish`"
                         "FROM `Spanish Words`"
                        " WHERE 1 = 1");

    if(charReferenceFilterEnabled == 'Y')
    {
        sprintf(strSQL2, " AND `Reference ID` = %d", intReferenceFilter);
    }
    else if(charReferenceFilterEnabled == 'N')
    {
        strSQL2[0] = '\0';
    }

    if(charGroupFilterEnabled == 'Y')
    {
        sprintf(strSQL3, " AND `Group ID` = %d", intGroupFilter);
    }
    else if(charGroupFilterEnabled == 'N')
    {
        strSQL3[0] = '\0';
    }

    if(charRandomizeWordLists == 'Y')
    {
        sprintf(strSQL4, " ORDER BY RAND()");
    }
    else if(charRandomizeWordLists == 'N')
    {
        sprintf(strSQL4, " ORDER BY `Word ID` ASC");
    }

    sprintf(strSQL0, "%s%s%s%s", strSQL1, strSQL2, strSQL3, strSQL4);

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return -1;
    }

// count the rows returned

    res = mysql_store_result(conn);
// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }

// check at least one row of results is returned

    intRowsInResult = (int)mysql_num_rows(res);
    if(intRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }
    else
    {
        printf("Rows in list: %d", intRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", intReferenceFilter, strReferenceFilterDecode, intGroupFilter, strGroupFilterDecode, charRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    intColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    intLengths = (int *)calloc(intColCount, sizeof(int));
 
    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < intColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print the contents of the Spanish Words table to the consolee

    printf("Entries in the table SpanishWords:");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        for (i = 0; i < intColCount; i++)
        {
            if(i == 0)
            {
                printf("%*s ", 5, row[i]);
            }
            else
            {
                printf(" %*s ", (int) strlen(row[i] + 1), row[i] ? row[i] : "NULL");
                if (i == 1)
                {
                    printf(" ... ");
                }
            }

        }

        if (intRowCount >= intListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((charResp = toupper(getchar())) != '\n')
            {
                if(charResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return 1;
                }
                continue;
            }
            intRowCount = 0;
            fRetitleConsole(strPrgNme);
            printf("\n");
            printf("Main Menu > List Words");
            printf("\n\n");
            printf("Rows in list: %d", intRowsInResult);
            printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", intReferenceFilter, strReferenceFilterDecode, intGroupFilter, strGroupFilterDecode, charRandomizeWordLists);
            printf("\n\n");
            printf("Entries in the table SpanishWords:");
            printf("\n");
        }
        printf("\n");
    }

    mysql_free_result(res);
    printf("\n");
    fPressEnterToContinue();
    printf("\n\n");
    return 0;
}

int fTestWordFields(char *strPrgNme)
{

// variable declarations

    int i = 0;                                                                      // loop counter
    int intRowCount = 0;                                   // number of rows printed to the console
    int intColCount = 0;                                   // number of columns returned in a query
    int *intLengths = NULL;                                               // array of column widths
    int intRowsInResult = 0;                                          // rows of data in result set
    int intCurrentTestID;                                                        // current test ID
    char charResp = '0';                                       // console response to test for exit
    char strStats[10] = {'\0'};                                                       // statistics

// create a test record

    intCurrentTestID = fStartTest();

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Test Words");
    printf("\n\n");

// define and send SQL query

    sprintf(strSQL1, "SELECT `Word ID`"
                         ", `Word English`"
                         ", `Word Spanish`"
                         "FROM `Spanish Words`"
                        " WHERE 1 = 1");

    if(charReferenceFilterEnabled == 'Y')
    {
        sprintf(strSQL2, " AND `Reference ID` = %d", intReferenceFilter);
    }
    else if(charReferenceFilterEnabled == 'N')
    {
        strSQL2[0] = '\0';
    }

    if(charGroupFilterEnabled == 'Y')
    {
        sprintf(strSQL3, " AND `Group ID` = %d", intGroupFilter);
    }
    else if(charGroupFilterEnabled =='N')
    {
        strSQL3[0] = '\0';
    }

    if(charRandomizeWordLists == 'Y')
    {
        sprintf(strSQL4, " ORDER BY RAND()");
    }
    else if(charRandomizeWordLists == 'N')
    {
        sprintf(strSQL4, " ORDER BY `Word ID` ASC");
    }

    sprintf(strSQL0, "%s%s%s%s", strSQL1, strSQL2, strSQL3, strSQL4);

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return -1;
    }

// store the results of the query

    res = mysql_store_result(conn);

// check for result errors

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }

// check at least one row of results is returned

    intRowsInResult = (int)mysql_num_rows(res);
    if(intRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }
    else
    {
        printf("Rows in list: %d", intRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", intReferenceFilter, strReferenceFilterDecode, intGroupFilter, strGroupFilterDecode, charRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    intColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    intLengths = (int *)calloc(intColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < intColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print each row of the Spanish Words table with a pause for verbal response

    printf("Test spanish translations (press enter for spanish, or e(x)it:)");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        for (i = 0; i < intColCount; i++)
        {
            if (i == 0)
            {
                printf("%5s ", row[i]);
            }
            else if(i == 1)
            {
                if((charShowPhraseLengths == 'Y') && (charShowPhraseCountdown == 'Y'))
                {
                    sprintf(strStats, "(%d) (%d)", (int) strlen(row[i+1]), intRowsInResult--);
                }
                if((charShowPhraseLengths == 'Y') && (charShowPhraseCountdown == 'N'))
                {
                    sprintf(strStats, "(%d)", (int) strlen(row[i+1]));
                }
                if((charShowPhraseLengths == 'N') && (charShowPhraseCountdown == 'Y'))
                {
                    sprintf(strStats, "(%d)", intRowsInResult--);
                }
                if((charShowPhraseLengths == 'N') && (charShowPhraseCountdown == 'N'))
                {
                    sprintf(strStats, "%s", "");
                }
                printf(" %-*s %s ... ", (int) strlen(row[i]), row[i] ? row[i] : "NULL", strStats);
            }
            else 
            {
                while((charResp = toupper(getchar())) != '\n')
                {
                    if(charResp == 'X')
                    {
                        mysql_free_result(res);
                        return 1;
                    }
                    continue;
                }
                printf("       esp> %-*s", (int) strlen(row[i]), row[i] ? row[i] : "NULL");
            }
        }

        if (intRowCount >= intListPageLines/2)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((charResp = toupper(getchar())) != '\n')
            {
                if(charResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return 1;
                }
                continue;
            }
            intRowCount = 0;
            fRetitleConsole(strPrgNme);
            printf("\n");
            printf("Main Menu > Test Words");
            printf("\n\n");
            printf("Rows in list: %d", intRowsInResult);
            printf(" (Reference Filter: %d %s, Group Filter: %d %s)", intReferenceFilter, strReferenceFilterDecode, intGroupFilter, strGroupFilterDecode);
            printf("\n\n");
            printf("Test Spanish Translations (press enter for spanish, or e)x)it:");
            printf("\n");
        }
        printf("\n");
    }
    
    mysql_free_result(res);
    fEndTest(intCurrentTestID);

    printf("\n");
    fPressEnterToContinue();
    printf("\n\n");
    return 0;
}

void fSetOptions(char *strPrgNme)
{
    char charOptionChoice = '0';
    char charOldListPageWidth = '0';
    char *strListPageLines = NULL;
    char *endptr = NULL;

    while (strchr("8xX", charOptionChoice) == NULL)
    {
        fRetitleConsole(strPrgNme);
        printf("\n");
        printf("Main Menu > Set Options");
        printf("\n\n");
        
        printf("1. Page (L)ines:  %d", intListPageLines);
        printf("\n");
        if (charListPageWidth == 'N')
        {
            printf("2. List (W)idth:  Narrow");
        }
        else if (charListPageWidth == 'W')
        {
            printf("2. List (W)idth:  Wide");
        }
        printf("\n");
        if(charReferenceFilterEnabled == 'Y')
        {
            printf("3. (R)eference Filter: Enabled with value: %d %s", intReferenceFilter, strReferenceFilterDecode);
        } 
        else
        {
            printf("3. (R)eference Filter: Disabled with value: %d %s",  intReferenceFilter, strReferenceFilterDecode);
        }
        printf("\n");
        if(charGroupFilterEnabled == 'Y')
        {
            printf("4. (G)roup Filter:     Enabled with value: %d %s", intGroupFilter, strGroupFilterDecode);
        }
        else
        {
            printf("4. (G)roup Filter:     Disabled with value: %d %s", intGroupFilter, strGroupFilterDecode);
        }
        printf("\n");
        if(charRandomizeWordLists == 'N')
        {
            printf("5. Randomi(z)e Word Lists: No");
        }
        else if (charRandomizeWordLists == 'Y')
        {
            printf("5. Randomi(z)e Word Lists: Yes");
        }
        printf("\n");
        if(charShowPhraseLengths == 'N')
        {
            printf("6. Show Phrase (L)engths: No");
        }
        else if (charShowPhraseLengths == 'Y')
        {
            printf("6. Show Phrase (L)engths: Yes");
        }
        printf("\n");
        if(charShowPhraseCountdown == 'N')
        {
            printf("7. Show Phrase (C)ountdown: No");
        }
        else if (charShowPhraseCountdown == 'Y')
        {
            printf("7. Show Phrase (C)ountdown: Yes");
        }
        printf("\n");
        printf("8. E(x)it Options");
        printf("\n\n");
        printf("Choose: ");
        
        charOptionChoice = GetChar();
    
        if (strchr("1Ll", charOptionChoice) != NULL)
        {
            printf("\n");
            printf("Page (L)ines or e(x)it: ");
            strListPageLines = GetString();
            if(strcmp(strListPageLines, "x") == 0)
            {
                return;
            }
            else
            {
                intListPageLines = (int) strtol(strListPageLines, &endptr, 10);
                if(intListPageLines == 0)
                {
                    printf("\n");
                    printf("Invalid number. Setting value to %d", DFLT_PAGE_LINES);
                    printf("\n\n");
                    intListPageLines = DFLT_PAGE_LINES;
                    fPressEnterToContinue();
                }
            }
            charOptionChoice = '0';
        }
        else if(strchr("2wW", charOptionChoice) != NULL)
        {
            charOldListPageWidth = charListPageWidth;
            charListPageWidth = '0';
            while(strchr("NWX", charListPageWidth) == NULL)
            {
                printf("\n");
                printf("List Page Width (W)ide or (N)arrow or e(x)it: ");
                charListPageWidth = toupper(GetChar());
            }
            if(charListPageWidth == 'X')
            {
                charListPageWidth = charOldListPageWidth;
                charOptionChoice = '0';
            }
            charOptionChoice = '0';
        }
        else if(strchr("3rR", charOptionChoice) != NULL)
        {
            printf("\n");
            fChooseReferenceID(strPrgNme);
            charOptionChoice = '0';
        }
        else if(strchr("4gG", charOptionChoice) != NULL)
        {
            printf("\n");
            fChooseGroupID(strPrgNme);
            charOptionChoice = '0';
        }
        else if(strchr("5zZ", charOptionChoice) != NULL)
        {
            printf("\n");
            do {
                if(charRandomizeWordLists == 'N')
                {
                    printf("Randomi(z)e Word Lists (Y)es or (N)o: ");
                }
                else if (charRandomizeWordLists == 'Y')
                {
                    printf("Randomi(z)e Word Lists (Y)es or (N)o: ");
                }
                charRandomizeWordLists = toupper(GetChar());
                charOptionChoice = '0';
            } while(strchr("NY", charRandomizeWordLists) == NULL);
        }
        else if(strchr("6lL", charOptionChoice) != NULL)
        {
            printf("\n");
            do {
                printf("Show Phrase Lengths (Y)es or (N)o: ");
                charShowPhraseLengths = toupper(GetChar());
                charOptionChoice = '0';
            } while(strchr("NY", charShowPhraseLengths) == NULL);
        }
        else if(strchr("7cC", charOptionChoice) != NULL)
        {
            printf("\n");
            do {
                printf("Show Phrase Countdown (Y)es or (N)o: ");
                charShowPhraseCountdown = toupper(GetChar());
                charOptionChoice = '0';
            } while(strchr("NY", charShowPhraseCountdown) == NULL);
        }
    }
    return;
}

void fRetitleConsole(char *strPrgNme)
{
    (void) system("clear");
    printf("%s -- Library maintenance using mySQL.", strPrgNme);
    printf("\n");
    return;
}

void fPressEnterToContinue()
{
    printf("Press enter to continue ");
    while(getchar() != '\n')
    {
        continue;
    }
    return;
}

void fPrintMainMenu()
{
    printf("\n");
    printf("Main Menu");
    printf("\n\n");
    printf("1. (L)ist Words");
    printf("\n");
    printf("2. Li(s)t Finished Tests");
    printf("\n");
    printf("3. (T)est Words");
    printf("\n");
    printf("4. O(p)tions");
    printf("\n");
    printf("5. E(x)it");
    printf("\n\n");
    printf("Choice: ");
    return;
}

void fChooseReferenceID(char *strPrgNme)
{
// variable declarations

    int i = 0;                                                                      // loop counter
    int intRowCount = 0;                                   // number of rows printed to the console
    int intColCount = 0;                                   // number of columns returned in a query
    int intReferenceIDChoice = 0;                           // Reference ID choice from the console
    int intRowsInResult = 0;                                          // rows of data in result set
    int intMaxWidth = 0;                              // maximum width of the Reference Name column
    char charResp = '0';                                       // console response to test for exit
    bool bReferenceIDFound = false;                      // flag for checking Reference ID is valid
    char strTemp[10] = {'\0'};                 // temporary storage to evaluate Reference ID filter

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Set Options > Reference Filter");
    printf("\n\n");

// enable or disable the reference filter
    charReferenceFilterEnabled = '0';
    while(strchr("YyNn", charReferenceFilterEnabled) == NULL)
    {
        printf("Enable Reference Filter (Y)es or (N)o: ");
        charReferenceFilterEnabled = toupper(GetChar());
    }
    printf("\n");

    if (strchr("nN", charReferenceFilterEnabled) != NULL)
    {
        return;
    }

// define and send SQL query

    sprintf(strSQL0, "SELECT R.`Reference ID`"
                          ", R.`Reference Name`"
                   ", COUNT(W.`Word ID`) AS Words"
                    " FROM `Spanish References` R"
                    " LEFT JOIN `Spanish Words` W"
                    " ON R.`Reference ID` = W.`Reference ID`"
                    " GROUP BY R.`Reference ID`, R.`Reference Name`"
                    " ORDER BY `Reference ID` ASC");


    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// count the rows returned

    res = mysql_store_result(conn);

// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// check at least one row of results is returned

    intRowsInResult = (int)mysql_num_rows(res);
    if(intRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }
    else
    {
        printf("Rows in list: %d", intRowsInResult);
        printf("\n\n");
    }

// fetch the number of columns in the result

    intColCount = mysql_num_fields(res);

// fetch the maximum width of the Reference Names column

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print the contents of the Spanish References table to the console

    printf("Entries in the table `Spanish References' and word counts:");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        for (i = 0; i < intColCount; i++)
        {
            if(i == 0)
            {
                printf("%*s ", 5, row[i]);
            }
            else if(i == 1)
            {
                printf(" %-*s ", intMaxWidth + 2, row[i] ? row[i] : "NULL");
            }
            else if(i == 2)
            {
                printf("%5s", row[i]);
            }
        }

        if (intRowCount >= intListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((charResp = toupper(getchar())) != '\n')
            {
                if(charResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return;
                }
                continue;
            }
            intRowCount = 0;
        }
        printf("\n");
    }

    while(bReferenceIDFound == false)
    {
        printf("\n");
        printf("Choose a Reference ID: ");
        intReferenceFilter = GetInt();

        mysql_data_seek(res, 0);

        while(row = mysql_fetch_row(res))
        {
            sprintf(strTemp, "%d", intReferenceFilter);
            if(strcmp(row[0], strTemp) == 0)
            {
                bReferenceIDFound = true;
                strcpy(strReferenceFilterDecode, row[1]);
                break;
            }
        }
        if(bReferenceIDFound == false)
        {
            printf("\n");
            printf("Invalid choice for Reference ID");
            printf("\n");
        }
    }
    printf("\n\n");
    mysql_free_result(res);
    return;
}

void fChooseGroupID(char *strPrgNme)
{
// variable declarations

    int i = 0;                                                                      // loop counter
    int intRowCount = 0;                                   // number of rows printed to the console
    int intColCount = 0;                                   // number of columns returned in a query
    int intGroupIDChoice = 0;                                   // Group ID choice from the console
    int intRowsInResult = 0;                                          // rows of data in result set
    int intMaxWidth = 0;                                  // maximum width of the Group Name column
    char charResp = '0';                                       // console response to test for exit
    bool bGroupIDFound = false;                               // flag for checking GroupID is valid
    char strTemp[10] = {'\0'};

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Set Options > Group Filter");
    printf("\n\n");

// enable or disable the group filter

    charGroupFilterEnabled = '0';
    while(strchr("YyNn", charGroupFilterEnabled) == NULL)
    {
        printf("Enable Group Filter (Y)es or (N)o: ");
        charGroupFilterEnabled = toupper(GetChar());
    }
    printf("\n");

    if (strchr("nN", charGroupFilterEnabled) != NULL)
    {
        return;
    }

// define and send SQL query

    sprintf(strSQL0, "SELECT G.`Group ID`"
                          ", G.`Group Name`"
                          ", COUNT(W.`Word ID`) AS Words"
                    " FROM `Spanish Groups` G"
                    " LEFT JOIN `Spanish Words` W ON G.`Group ID` = W.`Group ID`"
                    " GROUP BY G.`Group ID`, G.`Group Name`"
                    " ORDER BY G.`Group ID` ASC");

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// count the rows returned

    res = mysql_store_result(conn);
// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// check at least one row of results is returned

    intRowsInResult = (int)mysql_num_rows(res);
    if(intRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }
    else
    {
        printf("Rows in list: %d", intRowsInResult);
        printf("\n\n");
    }

// fetch the number of columns in the result

    intColCount = mysql_num_fields(res);

// fetch the maximum width of the Group Name column

 
    while(row = mysql_fetch_row(res))
    {
        if (row[1] != NULL)
        {
            if (strlen(row[1]) > intMaxWidth)
            {
                 intMaxWidth = strlen(row[1]);
            }
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print the contents of the Spanish Groups table to the console

    printf("Entries in the table `Spanish Groups':");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        for (i = 0; i < intColCount; i++)
        {
            if(i == 0)
            {
                printf("%*s ", 5, row[i]);
            }
            else if(1 == 1)
            {
                printf(" %-*s ", intMaxWidth, row[i] ? row[i] : "NULL");
            }
            else if(i == 2)
            {
                printf("%5s", row[i]);
            }
        }

        if (intRowCount >= intListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((charResp = toupper(getchar())) != '\n')
            {
                if(charResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return;
                }
                continue;
            }
            intRowCount = 0;
        }
        printf("\n");
    }

    while(bGroupIDFound == false)
    {
        printf("\n");
        printf("Choose a Group ID: ");
        intGroupFilter = GetInt();

        mysql_data_seek(res, 0);

        while(row = mysql_fetch_row(res))
        {
            sprintf(strTemp, "%d", intGroupFilter);
            if(strcmp(row[0], strTemp) == 0)
            {
                bGroupIDFound = true;
                strcpy(strGroupFilterDecode, row[1]);
                break;
            }
        }
        if(bGroupIDFound == false)
        {
            printf("\n");
            printf("Invalid Choice for Group ID");
            printf("\n");
        }
    }
    printf("\n\n");
    mysql_free_result(res);
    return;
}


int fStartTest(void)
{

// variable declarations

      int intRandomizeWordLists = 0;                          // boolean for radomize word lists flag
      int intCurrTestID = 0;                                                       // current Test iD

// set the randomize flag

    if(charRandomizeWordLists == 'Y')
    {
        intRandomizeWordLists = 1;
    }
    else
    {
        intRandomizeWordLists = 0;
    }

// define and send SQL query

    sprintf(strSQL0, "INSERT INTO `Spanish Tests`"
                                " (`Start Time`"
                                ", `Reference ID`"
                                ", `Group ID`"
                                ", `Randomized`)"
                     " values (now()"
                     ", %d"
                     ", %d"
                     ", %d)"
                     , intReferenceFilter, intGroupFilter, intRandomizeWordLists);

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return -1;
    }

    sprintf(strSQL0, "SELECT MAX(`Test ID`)"
                    " FROM `Spanish Tests`");

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return -1;
    }

// fetch the results

    res = mysql_store_result(conn);

// check res for a null value

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }

    row = mysql_fetch_row(res);
    mysql_free_result(res);

    return (int) atol(row[0]);
}

void fEndTest(int intCurrentTestID)
{

    sprintf(strSQL0, "UPDATE `Spanish Tests`"
                    " SET `finish Time` = NOW()"
                    " WHERE `test id` = %d", intCurrentTestID);

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

    return;
}

int fListTests(char *strPrgNme)
{

// variable declarations

    int i = 0;                                                                      // loop counter
    int intRowCount = 0;                                   // number of rows printed to the console
    int intColCount = 0;                                   // number of columns returned in a query
    int *intLengths = NULL;                                               // array of column widths
    int intRowsInResult = 0;                                          // rows of data in result set
    char charResp = '0';                                       // console response to test for exit

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Tests");
    printf("\n\n");

// define and send SQL query

//    sprintf(strSQL1, "SELECT `Word ID`"
//                         ", `Word English`"
//                         ", `Word Spanish`"
//                         "FROM `Spanish Words`"
//                        " WHERE 1 = 1");

    sprintf(strSQL0, "SELECT  t.`reference id`"
                            ", r.`reference name`"
                            ", g.`group id`"
                            ", g.`group name`"
                            ", if(t.`randomized` = 1, \"Yes\", \"No\") randomized"
                            ", count(t.`test id`) as count"
                     " FROM risingfast.`Spanish Tests` t"
                     " LEFT JOIN risingfast.`Spanish References` r ON t.`reference id`= r.`reference id`"
                     " LEFT JOIN risingfast.`Spanish Groups` g ON t.`group id` = g.`group id`"
                     " WHERE t.`finish time` IS NOT NULL"
                     " GROUP BY t.`reference id`, r.`reference name`, t.`randomized`, g.`group name`, g.`group id`"
                     " ORDER BY r.`reference name`");

    if (mysql_query(conn, strSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return -1;
    }

// count the rows returned

    res = mysql_store_result(conn);
// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }

// check at least one row of results is returned

    intRowsInResult = (int)mysql_num_rows(res);
    if(intRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return -1;
    }
    else
    {
        printf("Rows in list: %d", intRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", intReferenceFilter, strReferenceFilterDecode, intGroupFilter, strGroupFilterDecode, charRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    intColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    intLengths = (int *)calloc(intColCount, sizeof(int));
 
    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < intColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print the contents of the Spanish Words table to the consolee

    printf("Entries in the table SpanishWords:");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        for (i = 0; i < intColCount; i++)
        {
            if(i == 1)
            {
                printf("%-*s ", 20, row[i]);
            }
            else
            {
                printf("%*s ", 5, row[i]);
            }
        }
        printf("\n");
    }

    mysql_free_result(res);
    printf("\n");
    fPressEnterToContinue();
    printf("\n\n");
    return 0;
}
