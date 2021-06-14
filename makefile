# Created: 04/10/2020
# Geoffrey Jarman
# makefile for C drill exercises
# Ref: http://www6.uniovi.es/cscene/CS2/CS2-10.html -- Gnu Make and Multifile projects
#      https://www3.ntu.edu.sg/home/ehchua/programming/cpp/gcc_make.html -- Make tutorial
#      https://www.youtube.com/watch?v=G5dNorAoeCM&feature=youtu.be
# -ggdb option includs gdb friendly debug info in the executable
# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options

# Variables

CC=gcc
CFLAGS=-g -o

all: gf112Fahren2Celcius d_argcArgv d_arrays d_arrays2 d_assertions d_boolean d_bubbleSort d_characters d_clockTimer d_coreDump d_debug d_environment d_exec d_environment d_exec d_extern d_files d_fmtSpecifiers jsFork d_funcPointerCallbacks d_functionPointers d_getInputTest d_helloWorld helloWorld d_loops d_mallocCalloc d_modulus d_multiDimArrays d_pointers d_printf d_random d_spanishWords d_spanishWordsFile d_stringReverse d_strings d_stringsUpper d_stringsOverflow d_template d_threads d_variadic d_whileLoop d_snippets d_hashTable d_linkedList d_directories d_recursion d_mallocMore d_printBinary d_pointersAndArrays d_doublePointers d_dynamicArray d_pointerArithmetic d_regex d_regexscanf d_dynamicScanf d_scanf d_regexWords d_external fetch multVars d_sscanf tcount numVars simplest collectText battleShips showText authenticateUser guessAnIdiom formBook d_mallocString d_AjaxFetch1 d_AjaxFetch2 bookShow d_ajaxPost bookUpdate d_scanfOverflow calcE circle getChar gf103CompoundInterest gf041getchar1 call_cs50Get_String cs50ArrayCall cs50UpperCase cs50HelloWorld d_inlineAssembler d_arraySmallestValue cs50Factorial cs50cipher cs50SwapIntegers cs50AnalyzeSentence cs50PhoneBook cs50Pyramid cs50Merge d_realloc cs50LinkedList cs50LinkedListRandom jsDblPointers jsLibCurl immutableStrings UrlEncoding taxCalc1 gf070ReadChars viewData reverse getPutString1 gf031intsAsChars gf052sizeof1 gf063lower2Upper gf068getChar1 gf070scanfValid1 gf074strSpaces1 gf151palindrome1 gf401scanf1 gf172lowerUpperFn1 gf246randArray1 gf240guessRandom1 gf401fgets1 gf295strArrays1 gf265ArrayString1 gf293inputWithMalloc gf245pointers1 gf245pointers2 gf280pointers1 gf291pointers1 getEnv d_valgrind gf400fileStream1 gf400fileStream2 gf401fileStream1 gf170factorial1 gf252sortArray1 gf266sortStrings1 gf178crapsGame1 gf162fibonacci1 gf338structures1 gf348structures1 sillyCGI sillyCGI1 sillyCGI2 fibonacciRecursive testWithMinunit sdT1 d_Pipes d_envp jsTcpClient dhChapter1_1 dhChapter1_2 dhChapter1_4 dhChapter2_1 dhChapter2_3 dhChapter2_4 dhChapter3_1 dhChapter3_3 dhChapter4_1 dhChapter4_2 d_freeString TCPclient TCPserver HTTPserver HTTPclient HTTPclient2 HTTPHostnameToIP HTTPHostnameToIP2 HTTPHostnameToIP3 gf448Enumerators1

# General pattern make
%: %.c %.h
	$(CC) $(CFLAGS) $@ $^

# Specific pattern makes

d_argcArgv: d_argcArgv.c
	$(CC) $(CFLAGS) $@ $^

d_arrays: d_arrays.c
	$(CC) $(CFLAGS) $@ $^

d_arrays2: d_arrays2.c
	$(CC) $(CFLAGS) $@ $^

d_assertions: d_assertions.c
	$(CC) $(CFLAGS) $@ $^

d_boolean: d_boolean.c
	$(CC) $(CFLAGS) $@ $^

d_bubbleSort: d_bubbleSort.c
	$(CC) $(CFLAGS) $@ $^

d_characters: d_characters.c
	$(CC) $(CFLAGS) $@ $^

d_clockTimer: d_clockTimer.c
	$(CC) $(CFLAGS) $@ $^

d_coreDump: d_coreDump.c
	$(CC) $(CFLAGS) $@ $^

d_debug: d_debug.c
	$(CC) $(CFLAGS) $@ $^

d_environment: d_environment.c
	$(CC) $(CFLAGS) $@ $^

d_exec: d_exec.c
	$(CC) $(CFLAGS) $@ $^

d_extern: d_extern.c
	$(CC) $(CFLAGS) $@ $^

d_files: d_files.c
	$(CC) $(CFLAGS) $@ $^

d_fmtSpecifiers: d_fmtSpecifiers.c
	$(CC) $(CFLAGS) $@ $^

jsFork: jsFork.c
	$(CC) $(CFLAGS) $@ $^

d_funcPointerCallbacks: d_funcPointerCallbacks.c
	$(CC) $(CFLAGS) $@ $^

d_functionPointers: d_functionPointers.c
	$(CC) $(CFLAGS) $@ $^

d_getInputTest: d_getInputTest.c d_getInput.c
	$(CC) $(CFLAGS) $@ $^

d_helloWorld: d_helloWorld.c
	$(CC) $(CFLAGS) $@ $^

helloWorld: helloWorld.c
	$(CC) $(CFLAGS) $@ $^

d_loops: d_loops.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_mallocCalloc: d_mallocCalloc.c
	$(CC) $(CFLAGS) $@ $^

d_modulus: cs50.c d_modulus.c
	$(CC) $(CFLAGS) $@ $^

d_multiDimArrays: d_multiDimArrays.c
	$(CC) $(CFLAGS) $@ $^

d_pointers: d_pointers.c
	$(CC) $(CFLAGS) $@ $^

d_printf: d_printf.c
	$(CC) $(CFLAGS) $@ $^

d_random: d_random.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_spanishWords: d_spanishWords.c d_getInput.c
	$(CC) $(CFLAGS) $@ $^

d_spanishWordsFile: d_spanishWordsFile.c
	$(CC) $(CFLAGS) $@ $^

d_stringReverse: d_stringReverse.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_strings: d_strings.c
	$(CC) $(CFLAGS) $@ $^

d_stringsUpper: d_stringsUpper.c
	$(CC) $(CFLAGS) $@ $^

d_stringsOverflow: d_stringsOverflow.c
	$(CC) $(CFLAGS) $@ $^

d_template: d_template.c
	$(CC) $(CFLAGS) $@ $^

d_threads: d_threads.c
	$(CC) $(CFLAGS) $@ $^ -lpthread

d_variadic: d_variadic.c
	$(CC) $(CFLAGS) $@ $^ -lpthread

d_whileLoop: d_whileLoop.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_snippets: d_snippets.c
	$(CC) $(CFLAGS) $@ $^

d_hashTable: d_hashTable.c
	$(CC) $(CFLAGS) $@ $^

d_linkedList: d_linkedList.c
	$(CC) $(CFLAGS) $@ $^

d_directories: d_directories.c d_getInput.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_recursion: d_recursion.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_mallocMore: d_mallocMore.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_printBinary: d_printBinary.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_pointersAndArrays: d_pointersAndArrays.c
	$(CC) $(CFLAGS) $@ $^

d_doublePointers: d_doublePointers.c
	$(CC) $(CFLAGS) $@ $^

d_dynamicArray: d_dynamicArray.c
	$(CC) $(CFLAGS) $@ $^

d_pointerArithmetic: d_pointerArithmetic.c
	$(CC) $(CFLAGS) $@ $^

d_regex: d_regex.c
	$(CC) $(CFLAGS) $@ $^

d_dynamicScanf: d_dynamicScanf.c
	$(CC) $(CFLAGS) $@ $^

d_scanf: d_scanf.c
	$(CC) $(CFLAGS) $@ $^

d_regexscanf: d_regexscanf.c
	$(CC) $(CFLAGS) $@ $^

d_regexWords: d_regexWords.c d_getInput.c
	$(CC) $(CFLAGS) $@ $^

d_external: d_external.c
	$(CC) $(CFLAGS) $@ $^

fetch: fetch.c
	$(CC) $(CFLAGS) $@ $^

multVars: multVars.c
	$(CC) $(CFLAGS) $@ $^

d_sscanf: d_sscanf.c
	$(CC) $(CFLAGS) $@ $^

tcount: tcount.c cs50.c
	$(CC) $(CFLAGS) $@ $^

numVars: numVars.c
	$(CC) $(CFLAGS) $@ $^

simplest: simplest.c
	$(CC) $(CFLAGS) $@ $^

collectText: collectText.c
	$(CC) $(CFLAGS) $@ $^

battleShips: battleShips.c d_getInput.c
	$(CC) $(CFLAGS) $@ $^

showText: showText.c
	$(CC) $(CFLAGS) $@ $^

authenticateUser: authenticateUser.c
	$(CC) $(CFLAGS) $@ $^

guessAnIdiom: guessAnIdiom.c
	$(CC) $(CFLAGS) $@ $^

formBook: formBook.c
	$(CC) $(CFLAGS) $@ $^

d_mallocString: d_mallocString.c
	$(CC) $(CFLAGS) $@ $^

d_AjaxFetch1: d_AjaxFetch1.c
	$(CC) $(CFLAGS) $@ $^

d_AjaxFetch2: d_AjaxFetch2.c
	$(CC) $(CFLAGS) $@ $^

bookShow: bookShow.c
	$(CC) $(CFLAGS) $@ $^

d_ajaxPost: d_ajaxPost.c
	$(CC) $(CFLAGS) $@ $^

bookUpdate: bookUpdate.c
	$(CC) $(CFLAGS) $@ $^

d_scanfOverflow: d_scanfOverflow.c cs50.c
	$(CC) $(CFLAGS) $@ $^

calcE: calcE.c
	$(CC) $(CFLAGS) $@ $^ -lm

circle: circle.c
	$(CC) $(CFLAGS) $@ $^

getChar: getChar.c
	$(CC) $(CFLAGS) $@ $^

gf103CompoundInterest: gf103CompoundInterest.c d_getInput.c
	$(CC) $(CFLAGS) $@ $^ -lm

gf041getchar1: gf041getchar1.c
	$(CC) $(CFLAGS) $@ $^ -lm

call_cs50Get_String: call_cs50Get_String.c cs50Get_String.c
	$(CC) $(CFLAGS) $@ $^

cs50ArrayCall: cs50ArrayCall.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50UpperCase: cs50UpperCase.c cs50.c rf50.c
	$(CC) $(CFLAGS) $@ $^

cs50HelloWorld: cs50HelloWorld.c
	$(CC) $(CFLAGS) $@ $^

d_inlineAssembler: d_inlineAssembler.c
	$(CC) $(CFLAGS) $@ $^

d_arraySmallestValue: d_arraySmallestValue.c
	$(CC) $(CFLAGS) $@ $^

cs50Factorial: cs50Factorial.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf170factorial1: gf170factorial1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50cipher: cs50cipher.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50SwapIntegers: cs50SwapIntegers.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50AnalyzeSentence: cs50AnalyzeSentence.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50PhoneBook: cs50PhoneBook.c cs50.c rf50.c
	$(CC) $(CFLAGS) $@ $^

cs50Pyramid: cs50Pyramid.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50Merge: cs50Merge.c cs50.c
	$(CC) $(CFLAGS) $@ $^

d_realloc: d_realloc.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf112Fahren2Celcius: gf112Fahren2Celcius.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50LinkedList: cs50LinkedList.c cs50.c
	$(CC) $(CFLAGS) $@ $^

cs50LinkedListRandom: cs50LinkedListRandom.c cs50.c
	$(CC) $(CFLAGS) $@ $^

jsDblPointers: jsDblPointers.c
	$(CC) $(CFLAGS) $@ $^

jsLibCurl: jsLibCurl.c -lcurl 
	$(CC) $(CFLAGS) $@ $^

count: count.c
	$(CC) $(CFLAGS) $@ $^

immutableStrings: immutableStrings.c
	$(CC) $(CFLAGS) $@ $^

UrlEncoding: UrlEncoding.c
	$(CC) $(CFLAGS) $@ $^

taxCalc1: taxCalc1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf070ReadChars: gf070ReadChars.c cs50.c
	$(CC) $(CFLAGS) $@ $^

viewData: viewData.c
	$(CC) $(CFLAGS) $@ $^

reverse: reverse.c cs50.c
	$(CC) $(CFLAGS) $@ $^

getPutString1: getPutString1.c
	$(CC) $(CFLAGS) $@ $^

gf031intsAsChars: gf031intsAsChars.c
	$(CC) $(CFLAGS) $@ $^

gf052sizeof1: gf052sizeof1.c
	$(CC) $(CFLAGS) $@ $^

gf063lower2Upper: gf063lower2Upper.c
	$(CC) $(CFLAGS) $@ $^

gf068getChar1: gf068getChar1.c
	$(CC) $(CFLAGS) $@ $^

gf070scanfValid1: gf070scanfValid1.c
	$(CC) $(CFLAGS) $@ $^

gf074strSpaces1: gf074strSpaces1.c
	$(CC) $(CFLAGS) $@ $^

gf151palindrome1: gf151palindrome1.c
	$(CC) $(CFLAGS) $@ $^

gf401scanf1: gf401scanf1.c
	$(CC) $(CFLAGS) $@ $^

gf172lowerUpperFn1: gf172lowerUpperFn1.c
	$(CC) $(CFLAGS) $@ $^

gf246randArray1: gf246randArray1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf240guessRandom1: gf240guessRandom1.c
	$(CC) $(CFLAGS) $@ $^

gf401fgets1: gf401fgets1.c
	$(CC) $(CFLAGS) $@ $^

gf295strArrays1: gf295strArrays1.c
	$(CC) $(CFLAGS) $@ $^

gf265ArrayString1: gf265ArrayString1.c
	$(CC) $(CFLAGS) $@ $^

gf293inputWithMalloc: gf293inputWithMalloc.c
	$(CC) $(CFLAGS) $@ $^

gf245pointers1: gf245pointers1.c
	$(CC) $(CFLAGS) $@ $^

gf245pointers2: gf245pointers2.c
	$(CC) $(CFLAGS) $@ $^

gf280pointers1: gf280pointers1.c
	$(CC) $(CFLAGS) $@ $^

gf291pointers1: gf291pointers1.c
	$(CC) $(CFLAGS) $@ $^

getEnv: getEnv.c
	$(CC) $(CFLAGS) $@ $^

d_valgrind: d_valgrind.c
	$(CC) $(CFLAGS) $@ $^

gf400fileStream1: gf400fileStream1.c
	$(CC) $(CFLAGS) $@ $^

gf400fileStream2: gf400fileStream2.c
	$(CC) $(CFLAGS) $@ $^

gf401fileStream1: gf401fileStream1.c
	$(CC) $(CFLAGS) $@ $^

gf252sortArray1: gf252sortArray1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf266sortStrings1: gf266sortStrings1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf178crapsGame1: gf178crapsGame1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf162fibonacci1: gf162fibonacci1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf338structures1: gf338structures1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

gf348structures1: gf348structures1.c cs50.c
	$(CC) $(CFLAGS) $@ $^

sillyCGI: sillyCGI.c
	$(CC) $(CFLAGS) $@ $^

sillyCGI1: sillyCGI1.c
	$(CC) $(CFLAGS) $@ $^

sillyCGI2: sillyCGI2.c
	$(CC) $(CFLAGS) $@ $^

fibonacciRecursive: fibonacciRecursive.c
	$(CC) $(CFLAGS) $@ $^

testWithMinunit: testWithMinunit.c
	$(CC) $(CFLAGS) $@ $^

sdT1: sdT1.c
	$(CC) $(CFLAGS) $@ $^

d_Pipes: d_Pipes.c
	$(CC) $(CFLAGS) $@ $^

d_envp: d_envp.c
	$(CC) $(CFLAGS) $@ $^

jsTcpClient: jsTcpClient.c
	$(CC) $(CFLAGS) $@ $^

dhChapter1_1: dhChapter1_1.c
	$(CC) $(CFLAGS) $@ $^

dhChapter1_2: dhChapter1_2.c
	$(CC) $(CFLAGS) $@ $^

dhChapter1_4: dhChapter1_4.c
	$(CC) $(CFLAGS) $@ $^

dhChapter2_1: dhChapter2_1.c
	$(CC) $(CFLAGS) $@ $^

dhChapter2_3: dhChapter2_3.c
	$(CC) $(CFLAGS) $@ $^

dhChapter2_4: dhChapter2_4.c
	$(CC) $(CFLAGS) $@ $^

dhChapter3_1: dhChapter3_1.c -lm
	$(CC) $(CFLAGS) $@ $^

dhChapter3_3: dhChapter3_3.c
	$(CC) $(CFLAGS) $@ $^

dhChapter4_1: dhChapter4_1.c
	$(CC) $(CFLAGS) $@ $^

dhChapter4_2: dhChapter4_2.c
	$(CC) $(CFLAGS) $@ $^

d_freeString: d_freeString.c rf50.c
	$(CC) $(CFLAGS) $@ $^

TCPclient: TCPclient.c
	$(CC) $(CFLAGS) $@ $^

TCPserver: TCPserver.c
	$(CC) $(CFLAGS) $@ $^

HTTPclient: HTTPclient.c
	$(CC) $(CFLAGS) $@ $^

HTTPserver: HTTPserver.c
	$(CC) $(CFLAGS) $@ $^

HTTPclient2: HTTPclient2.c
	$(CC) $(CFLAGS) $@ $^

HTTPHostnameToIP: HTTPHostnameToIP.c
	$(CC) $(CFLAGS) $@ $^

HTTPHostnameToIP2: HTTPHostnameToIP2.c
	$(CC) $(CFLAGS) $@ $^

HTTPHostnameToIP3: HTTPHostnameToIP3.c
	$(CC) $(CFLAGS) $@ $^

gf448Enumerators1: gf448Enumerators1.c
	$(CC) $(CFLAGS) $@ $^

# clean or delete all generated intermediate files
clean:
	rm d_*.o d_*.i d_*.i
