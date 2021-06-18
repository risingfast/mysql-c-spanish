#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs50.h"
#include <ctype.h>
#include <stdbool.h>
#include "rf50.h"

#define SQL_LEN_L 5000
#define SQL_LEN_S 1000
#define DFLT_PAGE_LINES 80

// function prototypes

int  fListWordFields(char *);                                                  // show all spanish words on the console
int  fListTests(char*);                                                                   // show completed test counts
int  fTestWordFields(char *);                                           // test each spanish translation on the console
void fSetOptions(char *);                                                             // display and set global options
void fPressEnterToContinue();                                                                     // pause and continue
void fPrintMainMenu();                                                                   // print the main menu options
void fChooseReferenceID(char *);                                                               // choose a Reference ID
void fChooseGroupID(char *);                                                                       // choose a Group ID
int  fStartTest(void);                                                      // create a test log and return the Test ID
void fEndTest(int);                                                         // create a test log and return the Test ID
void fGetPwdFromConsole(void);                                                       // get a password from the console
void fRetitleConsole(char *);                                                          // clear and retitle the console

// global variables

int  iListPageLines = DFLT_PAGE_LINES;                                                   // default display page length
char cListPageWidth = 'N';                                                                        // display page width
char cReferenceFilterEnabled = 'N';                                                         // reference filter enabled
int  iReferenceFilter = 0;                                                                       // reference filter ID
char sReferenceFilterDecode[100] = {'\0'};                                         // decoded name for reference filter
char cGroupFilterEnabled = 'N';                                                                 // group filter enabled
char sGroupFilterDecode[100] = {'\0'};                                                 // decoded name for group filter
char cRandomizeWordLists = 'N';                                                       // option to randomize word lists
char cShowPhraseLengths = 'N';                                                         // option to show phrase lengths
char cShowPhraseCountdown = 'N';                                                     // option to show phrase countdown
int  iGroupFilter = 0;                                                                               // group filter ID

char *sServer = "192.168.0.13";                                                              // mySQL server IP address
char *sUsername = "gjarman";
char sPassword[20] = {'\0'};
char *sDatabase = "risingfast";                                                                  // mySQL database name
char caSQL0[SQL_LEN_L] = {'\0'};                                                                    // SQL query string
char caSQL1[SQL_LEN_S] = {'\0'};                                                                    // SQL query string
char caSQL2[SQL_LEN_S] = {'\0'};                                                                    // SQL query string
char caSQL3[SQL_LEN_S] = {'\0'};                                                                    // SQL query string
char caSQL4[SQL_LEN_S] = {'\0'};                                                                    // SQL query string

MYSQL *conn;

// main program

int main(int argc, char *argv[])
{
// variable declarations

    char cMainChoice = '0';                                                                         // main menu choice
    char *strPrgNme = strcat(argv[0] + 2, " - Spanish Language Practice Drills");

    fRetitleConsole(strPrgNme);
    fGetPwdFromConsole();
    if(strcmp("BadSoExit", sPassword) == 0)
    {
        printf("\n");
        return EXIT_FAILURE;
    }
    fRetitleConsole(strPrgNme);

// Initialize a connection and connect to the database

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sServer, sUsername, sPassword, sDatabase, 0, NULL, 0))
    {
        printf("\n");
        printf("Failed to connect to MySQL Server %s in module %s()", sServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        fPressEnterToContinue();
        printf("\n");
        return EXIT_FAILURE;
    }

// print the main menu to the console

    while (strchr("5xX", cMainChoice) == NULL)
    {
        fRetitleConsole(strPrgNme);
        fPrintMainMenu();
        cMainChoice = GetChar();
        if(strchr("1lL", cMainChoice) != NULL)
        {
            printf("\n");
            fListWordFields(strPrgNme);
            printf("\n");
        }
        if(strchr("2sS", cMainChoice) != NULL)
        {
            printf("\n");
            fListTests(strPrgNme);
            printf("\n");
        }
        else if(strchr("3tT", cMainChoice) != NULL)
        {
            printf("\n");
            fTestWordFields(strPrgNme);
            printf("\n");
        }
        else if(strchr("4pP", cMainChoice) != NULL)
        {
            printf("\n");
            fSetOptions(strPrgNme);
            printf("\n");
        }
    }

    mysql_close(conn);
    system("clear");
    return EXIT_SUCCESS;
}

int fListWordFields(char *strPrgNme)
{

// variable declarations

    int i = 0;                                                                                          // loop counter
    int iRowCount = 0;                                                         // number of rows printed to the console
    int iColCount = 0;                                                         // number of columns returned in a query
    int *iLengths = NULL;                                                                     // array of column widths
    int iRowsInResult = 0;                                                                // rows of data in result set
    char cResp = '0';                                                           // console response to test for exit

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Words");
    printf("\n\n");

// define and send SQL query

    sprintf(caSQL1, "SELECT `Word ID`"
                         ", `Word English`"
                         ", `Word Spanish`"
                         "FROM `Spanish Words`"
                        " WHERE 1 = 1");

    if(cReferenceFilterEnabled == 'Y')
    {
        sprintf(caSQL2, " AND `Reference ID` = %d", iReferenceFilter);
    }
    else if(cReferenceFilterEnabled == 'N')
    {
        caSQL2[0] = '\0';
    }

    if(cGroupFilterEnabled == 'Y')
    {
        sprintf(caSQL3, " AND `Group ID` = %d", iGroupFilter);
    }
    else if(cGroupFilterEnabled == 'N')
    {
        caSQL3[0] = '\0';
    }

    if(cRandomizeWordLists == 'Y')
    {
        sprintf(caSQL4, " ORDER BY RAND()");
    }
    else if(cRandomizeWordLists == 'N')
    {
        sprintf(caSQL4, " ORDER BY `Word ID` ASC");
    }

    sprintf(caSQL0, "%s%s%s%s", caSQL1, caSQL2, caSQL3, caSQL4);

    if (mysql_query(conn, caSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// count the rows returned

    res = mysql_store_result(conn);
// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// check at least one row of results is returned

    iRowsInResult = (int)mysql_num_rows(res);
    if(iRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }
    else
    {
        printf("Rows in list: %d", iRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", iReferenceFilter, sReferenceFilterDecode, iGroupFilter, sGroupFilterDecode, cRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    iColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    iLengths = (int *)calloc(iColCount, sizeof(int));
 
    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < iColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
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
        iRowCount++;
        for (i = 0; i < iColCount; i++)
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

        if (iRowCount >= iListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((cResp = toupper(getchar())) != '\n')
            {
                if(cResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return EXIT_SUCCESS;
                }
                continue;
            }
            iRowCount = 0;
            fRetitleConsole(strPrgNme);
            printf("\n");
            printf("Main Menu > List Words");
            printf("\n\n");
            printf("Rows in list: %d", iRowsInResult);
            printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", iReferenceFilter, sReferenceFilterDecode, iGroupFilter, sGroupFilterDecode, cRandomizeWordLists);
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
    return EXIT_SUCCESS;
}

int fTestWordFields(char *strPrgNme)
{

// variable declarations

    int i = 0;                                                                                          // loop counter
    int iRowCount = 0;                                                         // number of rows printed to the console
    int iColCount = 0;                                                         // number of columns returned in a query
    int *iLengths = NULL;                                                                     // array of column widths
    int iRowsInResult = 0;                                                                // rows of data in result set
    int iCurrentTestID;                                                                              // current test ID
    char cResp = '0';                                                           // console response to test for exit
    char strStats[10] = {'\0'};                                                                           // statistics

    MYSQL_RES *res;
    MYSQL_ROW row;

// create a test record

    iCurrentTestID = fStartTest();

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Test Words");
    printf("\n\n");

// define and send SQL query

    sprintf(caSQL1, "SELECT `Word ID`"
                         ", `Word English`"
                         ", `Word Spanish`"
                         "FROM `Spanish Words`"
                        " WHERE 1 = 1");

    if(cReferenceFilterEnabled == 'Y')
    {
        sprintf(caSQL2, " AND `Reference ID` = %d", iReferenceFilter);
    }
    else if(cReferenceFilterEnabled == 'N')
    {
        caSQL2[0] = '\0';
    }

    if(cGroupFilterEnabled == 'Y')
    {
        sprintf(caSQL3, " AND `Group ID` = %d", iGroupFilter);
    }
    else if(cGroupFilterEnabled =='N')
    {
        caSQL3[0] = '\0';
    }

    if(cRandomizeWordLists == 'Y')
    {
        sprintf(caSQL4, " ORDER BY RAND()");
    }
    else if(cRandomizeWordLists == 'N')
    {
        sprintf(caSQL4, " ORDER BY `Word ID` ASC");
    }

    sprintf(caSQL0, "%s%s%s%s", caSQL1, caSQL2, caSQL3, caSQL4);

    if (mysql_query(conn, caSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// store the results of the query

    res = mysql_store_result(conn);

// check for result errors

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// check at least one row of results is returned

    iRowsInResult = (int)mysql_num_rows(res);
    if(iRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }
    else
    {
        printf("Rows in list: %d", iRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", iReferenceFilter, sReferenceFilterDecode, iGroupFilter, sGroupFilterDecode, cRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    iColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    iLengths = (int *)calloc(iColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < iColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
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
        iRowCount++;
        for (i = 0; i < iColCount; i++)
        {
            if (i == 0)
            {
                printf("%5s ", row[i]);
            }
            else if(i == 1)
            {
                if((cShowPhraseLengths == 'Y') && (cShowPhraseCountdown == 'Y'))
                {
                    sprintf(strStats, "(%d) (%d)", (int) strlen(row[i+1]), iRowsInResult--);
                }
                if((cShowPhraseLengths == 'Y') && (cShowPhraseCountdown == 'N'))
                {
                    sprintf(strStats, "(%d)", (int) strlen(row[i+1]));
                }
                if((cShowPhraseLengths == 'N') && (cShowPhraseCountdown == 'Y'))
                {
                    sprintf(strStats, "(%d)", iRowsInResult--);
                }
                if((cShowPhraseLengths == 'N') && (cShowPhraseCountdown == 'N'))
                {
                    sprintf(strStats, "%s", "");
                }
                printf(" %-*s %s ... ", (int) strlen(row[i]), row[i] ? row[i] : "NULL", strStats);
            }
            else 
            {
                while((cResp = toupper(getchar())) != '\n')
                {
                    if(cResp == 'X')
                    {
                        mysql_free_result(res);
                        return EXIT_SUCCESS;
                    }
                    continue;
                }
                printf("       esp> %-*s", (int) strlen(row[i]), row[i] ? row[i] : "NULL");
            }
        }

        if (iRowCount >= iListPageLines/2)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((cResp = toupper(getchar())) != '\n')
            {
                if(cResp == 'X')
                {
                    while(getchar() != '\n')
                    {
                        continue;
                    }
                    mysql_free_result(res);
                    return EXIT_SUCCESS;
                }
                continue;
            }
            iRowCount = 0;
            fRetitleConsole(strPrgNme);
            printf("\n");
            printf("Main Menu > Test Words");
            printf("\n\n");
            printf("Rows in list: %d", iRowsInResult);
            printf(" (Reference Filter: %d %s, Group Filter: %d %s)", iReferenceFilter, sReferenceFilterDecode, iGroupFilter, sGroupFilterDecode);
            printf("\n\n");
            printf("Test Spanish Translations (press enter for spanish, or e)x)it:");
            printf("\n");
        }
        printf("\n");
    }
    
    mysql_free_result(res);
    fEndTest(iCurrentTestID);

    printf("\n");
    fPressEnterToContinue();
    printf("\n\n");
    return EXIT_SUCCESS;
}

void fSetOptions(char *strPrgNme)
{
    char cOptionChoice = '0';
    char cOldListPageWidth = '0';
    char *sListPageLines = NULL;
    char *endptr = NULL;

    while (strchr("8xX", cOptionChoice) == NULL)
    {
        fRetitleConsole(strPrgNme);
        printf("\n");
        printf("Main Menu > Set Options");
        printf("\n\n");
        
        printf("1. Page (L)ines:  %d", iListPageLines);
        printf("\n");
        if (cListPageWidth == 'N')
        {
            printf("2. List (W)idth:  Narrow");
        }
        else if (cListPageWidth == 'W')
        {
            printf("2. List (W)idth:  Wide");
        }
        printf("\n");
        if(cReferenceFilterEnabled == 'Y')
        {
            printf("3. (R)eference Filter: Enabled with value: %d %s", iReferenceFilter, sReferenceFilterDecode);
        } 
        else
        {
            printf("3. (R)eference Filter: Disabled with value: %d %s",  iReferenceFilter, sReferenceFilterDecode);
        }
        printf("\n");
        if(cGroupFilterEnabled == 'Y')
        {
            printf("4. (G)roup Filter:     Enabled with value: %d %s", iGroupFilter, sGroupFilterDecode);
        }
        else
        {
            printf("4. (G)roup Filter:     Disabled with value: %d %s", iGroupFilter, sGroupFilterDecode);
        }
        printf("\n");
        if(cRandomizeWordLists == 'N')
        {
            printf("5. Randomi(z)e Word Lists: No");
        }
        else if (cRandomizeWordLists == 'Y')
        {
            printf("5. Randomi(z)e Word Lists: Yes");
        }
        printf("\n");
        if(cShowPhraseLengths == 'N')
        {
            printf("6. Show Phrase (L)engths: No");
        }
        else if (cShowPhraseLengths == 'Y')
        {
            printf("6. Show Phrase (L)engths: Yes");
        }
        printf("\n");
        if(cShowPhraseCountdown == 'N')
        {
            printf("7. Show Phrase (C)ountdown: No");
        }
        else if (cShowPhraseCountdown == 'Y')
        {
            printf("7. Show Phrase (C)ountdown: Yes");
        }
        printf("\n");
        printf("8. E(x)it Options");
        printf("\n\n");
        printf("Choose: ");
        
        cOptionChoice = GetChar();
    
        if (strchr("1Ll", cOptionChoice) != NULL)
        {
            printf("\n");
            printf("Page (L)ines or e(x)it: ");
            sListPageLines = GetString();
            if(strcmp(sListPageLines, "x") == 0)
            {
                return;
            }
            else
            {
                iListPageLines = (int) strtol(sListPageLines, &endptr, 10);
                if(iListPageLines == 0)
                {
                    printf("\n");
                    printf("Invalid number. Setting value to %d", DFLT_PAGE_LINES);
                    printf("\n\n");
                    iListPageLines = DFLT_PAGE_LINES;
                    fPressEnterToContinue();
                }
            }
            cOptionChoice = '0';
        }
        else if(strchr("2wW", cOptionChoice) != NULL)
        {
            cOldListPageWidth = cListPageWidth;
            cListPageWidth = '0';
            while(strchr("NWX", cListPageWidth) == NULL)
            {
                printf("\n");
                printf("List Page Width (W)ide or (N)arrow or e(x)it: ");
                cListPageWidth = toupper(GetChar());
            }
            if(cListPageWidth == 'X')
            {
                cListPageWidth = cOldListPageWidth;
                cOptionChoice = '0';
            }
            cOptionChoice = '0';
        }
        else if(strchr("3rR", cOptionChoice) != NULL)
        {
            printf("\n");
            fChooseReferenceID(strPrgNme);
            cOptionChoice = '0';
        }
        else if(strchr("4gG", cOptionChoice) != NULL)
        {
            printf("\n");
            fChooseGroupID(strPrgNme);
            cOptionChoice = '0';
        }
        else if(strchr("5zZ", cOptionChoice) != NULL)
        {
            printf("\n");
            do {
                if(cRandomizeWordLists == 'N')
                {
                    printf("Randomi(z)e Word Lists (Y)es or (N)o: ");
                }
                else if (cRandomizeWordLists == 'Y')
                {
                    printf("Randomi(z)e Word Lists (Y)es or (N)o: ");
                }
                cRandomizeWordLists = toupper(GetChar());
                cOptionChoice = '0';
            } while(strchr("NY", cRandomizeWordLists) == NULL);
        }
        else if(strchr("6lL", cOptionChoice) != NULL)
        {
            printf("\n");
            do {
                printf("Show Phrase Lengths (Y)es or (N)o: ");
                cShowPhraseLengths = toupper(GetChar());
                cOptionChoice = '0';
            } while(strchr("NY", cShowPhraseLengths) == NULL);
        }
        else if(strchr("7cC", cOptionChoice) != NULL)
        {
            printf("\n");
            do {
                printf("Show Phrase Countdown (Y)es or (N)o: ");
                cShowPhraseCountdown = toupper(GetChar());
                cOptionChoice = '0';
            } while(strchr("NY", cShowPhraseCountdown) == NULL);
        }
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

    int i = 0;                                                                                          // loop counter
    int iRowCount = 0;                                                         // number of rows printed to the console
    int iColCount = 0;                                                         // number of columns returned in a query
    int iReferenceIDChoice = 0;                                                 // Reference ID choice from the console
    int iRowsInResult = 0;                                                                // rows of data in result set
    int iMaxWidth = 0;                                                    // maximum width of the Reference Name column
    char cResp = '0';                                                              // console response to test for exit
    bool bReferenceIDFound = false;                                          // flag for checking Reference ID is valid
    char sTemp[10] = {'\0'};                                       // temporary storage to evaluate Reference ID filter

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Set Options > Reference Filter");
    printf("\n\n");

// enable or disable the reference filter
    cReferenceFilterEnabled = '0';
    while(strchr("YyNn", cReferenceFilterEnabled) == NULL)
    {
        printf("Enable Reference Filter (Y)es or (N)o: ");
        cReferenceFilterEnabled = toupper(GetChar());
    }
    printf("\n");

    if (strchr("nN", cReferenceFilterEnabled) != NULL)
    {
        return;
    }

// define and send SQL query

    sprintf(caSQL0, "SELECT R.`Reference ID`"
                          ", R.`Reference Name`"
                   ", COUNT(W.`Word ID`) AS Words"
                    " FROM `Spanish References` R"
                    " LEFT JOIN `Spanish Words` W"
                    " ON R.`Reference ID` = W.`Reference ID`"
                    " GROUP BY R.`Reference ID`, R.`Reference Name`"
                    " ORDER BY `Reference ID` ASC");


    if (mysql_query(conn, caSQL0))
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
        return;
    }

// check at least one row of results is returned

    iRowsInResult = (int)mysql_num_rows(res);
    if(iRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }
    else
    {
        printf("Rows in list: %d", iRowsInResult);
        printf("\n\n");
    }

// fetch the number of columns in the result

    iColCount = mysql_num_fields(res);

// fetch the maximum width of the Reference Names column

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

// reset the pointer in the result set to the start

    mysql_data_seek(res, 0);

// print the contents of the Spanish References table to the console

    printf("Entries in the table `Spanish References' and word counts:");
    printf("\n\n");

    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        for (i = 0; i < iColCount; i++)
        {
            if(i == 0)
            {
                printf("%*s ", 5, row[i]);
            }
            else if(i == 1)
            {
                printf(" %-*s ", iMaxWidth + 2, row[i] ? row[i] : "NULL");
            }
            else if(i == 2)
            {
                printf("%5s", row[i]);
            }
        }

        if (iRowCount >= iListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((cResp = toupper(getchar())) != '\n')
            {
                if(cResp == 'X')
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
            iRowCount = 0;
        }
        printf("\n");
    }

    while(bReferenceIDFound == false)
    {
        printf("\n");
        printf("Choose a Reference ID: ");
        iReferenceFilter = GetInt();

        mysql_data_seek(res, 0);

        while(row = mysql_fetch_row(res))
        {
            sprintf(sTemp, "%d", iReferenceFilter);
            if(strcmp(row[0], sTemp) == 0)
            {
                bReferenceIDFound = true;
                strcpy(sReferenceFilterDecode, row[1]);
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

    int i = 0;                                                                                          // loop counter
    int iRowCount = 0;                                                         // number of rows printed to the console
    int iColCount = 0;                                                         // number of columns returned in a query
    int iGroupIDChoice = 0;                                                         // Group ID choice from the console
    int iRowsInResult = 0;                                                                // rows of data in result set
    int iMaxWidth = 0;                                                        // maximum width of the Group Name column
    char cResp = '0';                                                              // console response to test for exit
    bool bGroupIDFound = false;                                                   // flag for checking GroupID is valid
    char sTemp[10] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Set Options > Group Filter");
    printf("\n\n");

// enable or disable the group filter

    cGroupFilterEnabled = '0';
    while(strchr("YyNn", cGroupFilterEnabled) == NULL)
    {
        printf("Enable Group Filter (Y)es or (N)o: ");
        cGroupFilterEnabled = toupper(GetChar());
    }
    printf("\n");

    if (strchr("nN", cGroupFilterEnabled) != NULL)
    {
        return;
    }

// define and send SQL query

    sprintf(caSQL0, "SELECT G.`Group ID`"
                          ", G.`Group Name`"
                          ", COUNT(W.`Word ID`) AS Words"
                    " FROM `Spanish Groups` G"
                    " LEFT JOIN `Spanish Words` W ON G.`Group ID` = W.`Group ID`"
                    " GROUP BY G.`Group ID`, G.`Group Name`"
                    " ORDER BY G.`Group ID` ASC");

    if (mysql_query(conn, caSQL0))
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
        return;
    }

// check at least one row of results is returned

    iRowsInResult = (int)mysql_num_rows(res);
    if(iRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }
    else
    {
        printf("Rows in list: %d", iRowsInResult);
        printf("\n\n");
    }

// fetch the number of columns in the result

    iColCount = mysql_num_fields(res);

// fetch the maximum width of the Group Name column

 
    while(row = mysql_fetch_row(res))
    {
        if (row[1] != NULL)
        {
            if (strlen(row[1]) > iMaxWidth)
            {
                 iMaxWidth = strlen(row[1]);
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
        iRowCount++;
        for (i = 0; i < iColCount; i++)
        {
            if(i == 0)
            {
                printf("%*s ", 5, row[i]);
            }
            else if(1 == 1)
            {
                printf(" %-*s ", iMaxWidth, row[i] ? row[i] : "NULL");
            }
            else if(i == 2)
            {
                printf("%5s", row[i]);
            }
        }

        if (iRowCount >= iListPageLines)
        {
            printf("\n\n");
            printf("Press enter to continue or e(x)it ");
            while((cResp = toupper(getchar())) != '\n')
            {
                if(cResp == 'X')
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
            iRowCount = 0;
        }
        printf("\n");
    }

    while(bGroupIDFound == false)
    {
        printf("\n");
        printf("Choose a Group ID: ");
        iGroupFilter = GetInt();

        mysql_data_seek(res, 0);

        while(row = mysql_fetch_row(res))
        {
            sprintf(sTemp, "%d", iGroupFilter);
            if(strcmp(row[0], sTemp) == 0)
            {
                bGroupIDFound = true;
                strcpy(sGroupFilterDecode, row[1]);
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

    int iRandomizeWordLists = 0;                                                // boolean for radomize word lists flag
    int iCurrTestID = 0;                                                                             // current Test iD

    MYSQL_RES *res;
    MYSQL_ROW row;

// set the randomize flag

    if(cRandomizeWordLists == 'Y')
    {
        iRandomizeWordLists = 1;
    }
    else
    {
        iRandomizeWordLists = 0;
    }

// define and send SQL query

    sprintf(caSQL0, "INSERT INTO `Spanish Tests`"
                                " (`Start Time`"
                                ", `Reference ID`"
                                ", `Group ID`"
                                ", `Randomized`)"
                     " values (now()"
                     ", %d"
                     ", %d"
                     ", %d)"
                     , iReferenceFilter, iGroupFilter, iRandomizeWordLists);

    if (mysql_query(conn, caSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    sprintf(caSQL0, "SELECT MAX(`Test ID`)"
                    " FROM `Spanish Tests`");

    if (mysql_query(conn, caSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// fetch the results

    res = mysql_store_result(conn);

// check res for a null value

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    row = mysql_fetch_row(res);
    mysql_free_result(res);
    return (int) atol(row[0]);
}

void fEndTest(int iCurrentTestID)
{

    sprintf(caSQL0, "UPDATE `Spanish Tests`"
                    " SET `finish Time` = NOW()"
                    " WHERE `test id` = %d", iCurrentTestID);

    if (mysql_query(conn, caSQL0))
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

    int i = 0;                                                                                          // loop counter
    int iRowCount = 0;                                                         // number of rows printed to the console
    int iColCount = 0;                                                         // number of columns returned in a query
    int *iLengths = NULL;                                                                     // array of column widths
    int iRowsInResult = 0;                                                                // rows of data in result set
    char cResp = '0';                                                              // console response to test for exit

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console and print a heading

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Tests");
    printf("\n\n");

// define and send SQL query

//    sprintf(caSQL1, "SELECT `Word ID`"
//                         ", `Word English`"
//                         ", `Word Spanish`"
//                         "FROM `Spanish Words`"
//                        " WHERE 1 = 1");

    sprintf(caSQL0, "SELECT  t.`reference id`"
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

    if (mysql_query(conn, caSQL0))
    {
        printf("Failed to execute mysql_query() in function %s().", __func__);
        printf("\n\n");
        printf("Error text: %s", mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// count the rows returned

    res = mysql_store_result(conn);
// store the results of the query

    if (res == (MYSQL_RES *) NULL)
    {
        printf("Error calling mysql_store_result() in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// check at least one row of results is returned

    iRowsInResult = (int)mysql_num_rows(res);
    if(iRowsInResult < 1)
    {
        printf("Error - No rows in query result in function %s()", __func__);
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }
    else
    {
        printf("Rows in list: %d", iRowsInResult);
        printf(" (Reference Filter: %d %s, Group Filter: %d %s, Randomize: %c)", iReferenceFilter, sReferenceFilterDecode, iGroupFilter, sGroupFilterDecode, cRandomizeWordLists);
        printf("\n\n");
    }

// fetch the number of columns in the result

    iColCount = mysql_num_fields(res);

// fetch the width of each column in the result

    iLengths = (int *)calloc(iColCount, sizeof(int));
 
    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < iColCount; k++)
        {
            if (row[k] != NULL)
            {
                if (strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
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
        iRowCount++;
        for (i = 0; i < iColCount; i++)
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
    return EXIT_SUCCESS;
}

void fGetPwdFromConsole(void)
{
    char *sEnteredPwd = NULL;;

    printf("\n");
    do
    {
        printf("Password to connect to mysqlDB (or E(x)it): ");
        sEnteredPwd = GetString();
        if((strlen(sEnteredPwd) == 1) && (strchr("xX", sEnteredPwd[0]) != NULL))
        {
            strcpy(sPassword, "BadSoExit");
            break;
        }
        else
        {
            conn = mysql_init(NULL);

            if (!mysql_real_connect(conn, sServer, sUsername, sEnteredPwd, sDatabase, 0, NULL, 0))
            {
                printf("\n");
//                printf("Failed to connect to MySQL Server %s in module %s()", sSserver, __func__);
                printf("Failed to connect to MySQL server with entered password");
                printf("\n\n");
//                printf("Error: %s\n", mysql_error(conn));
//                printf("\n");
                fPressEnterToContinue();
                printf("\n");
//                printf("\n");
            }
            else
            {
                strcpy(sPassword, sEnteredPwd);
                mysql_close(conn);
            }
        }
    } while(strcmp(sPassword, sEnteredPwd) != 0);

    return;
}

