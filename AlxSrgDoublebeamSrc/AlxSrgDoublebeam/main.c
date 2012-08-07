//
//  main.c
//  AlxSrgDoublebeam
//
//  Created by Alexander Sergeev on 8/3/12.
//  Copyright (c) 2012 Alexander Sergeev. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// This function aims to replace strcmp
// return an integer greater than, equal to, or less than 0,
// according as the string s1 is greater than, equal to, or less than the string s2.
int MyStrCmp(const char *s1, const char *s2) {
	const char * p1 = s1;
	const char * p2 = s2;
	for (;;) {
		const char c1 = *p1;
		const char c2 = *p2;
		if (c1 < c2) {
			return -1;
		} else if (c1 > c2) {
			return 1;
		} else {
			++p1;
      ++p2;
      if (0 == *p1) {
        if (0 == *p2) {
          return 0;
        }
        return -1;
      }
			if (0 == *p2) {
				return 1;
			}
		}
	}
	return 0;
}

// Allocate memory just like malloc do, but calls abort() if smth goes wrong.
void * SafeMAlloc(size_t sz) {
  void * res = malloc(sz);
  //memset(res, 0, sz);
  if (0 == res) {
    printf("Can't alloc memory. Buy more RAM, dude.");
    abort();
  }
  return res;
}

// Swap the content of the buffer "in place".
void Swap(char * str, size_t len) {
	int i = 0;
	for (; i < len/2; ++i) {
		char *swpPos1 = str+i;
		char *swpPos2 = str+len-i-1;
		const char b = *swpPos1;
		*swpPos1 = *swpPos2;
		*swpPos2 = b;
	}
}

// Allocate swapped string.
// For example, allocSwappedStr("abc") returns "cba"
// Don't forget to free the result
char * allocSwappedStr(const char * str) {
	const size_t sz = strlen(str)+1;
	char * buff = SafeMAlloc(sz);
	strncpy(buff, str, sz);
	Swap(buff, sz-1);
	return buff;
}

// Helper for allocMergedStr()
char * mergeChar(char * buff, const char ** source) {
	const char chr = **source;
	if (0 != chr) {
		*buff = chr;
		++buff;
		++*source;
	}
	return buff;
}


// Allocate merged string.
// For example, allocMergedStr("abcd", "ef") returns "aebfcd"
// Don't forget to free the result
char * allocMergedStr(const char * s1, const char * s2) {
	size_t sz = strlen(s1) + strlen(s2) + 1;
	char * buff = SafeMAlloc(sz);
	const char * p1 = s1;
	const char * p2 = s2;
	char * out = buff;
	while (*p1 != 0 || *p2 != 0) {
		out = mergeChar(out, &p1);
		out = mergeChar(out, &p2);
	}
	buff[sz-1] = 0;
	return buff;
}

// ===================== TEST STUFF =====================

// Test swap function
void TestSwap(const char * testString) {
	char * buff = allocSwappedStr(testString);
	printf("orig: [%s] swapped: [%s]\n", testString, buff);
	free(buff);
}

// Test comparasion function
void TestCmp(const char * str1, const char * str2) {

	const int strcmpValue = strcmp(str1, str2); // Yes, I use strcmp here, but just for checking my own function results
	int myValue = MyStrCmp(str1, str2);

	int testFailed = ((strcmpValue < 0) != (myValue < 0) || (strcmpValue > 0) != (myValue > 0));

  const char * resStr = "=";
	if (myValue > 0) {
		resStr = ">";
	} else if (myValue < 0) {
		resStr = "<";
	}

	printf("[%s] %s [%s] %s\n", str1, resStr, str2, (testFailed ? "!!!" : "OK"));
}

// ^^^^^^^^^^^^^^^^^^^^^^^ TEST STUFF ^^^^^^^^^^^^^^^^^^^^^^^

int DoTheJob(const char * str1, const char *str2) {
	const int r = MyStrCmp(str1, str2);
	if (r < 0) {
		char * swp1 = allocSwappedStr(str1);
		char * swp2 = allocSwappedStr(str2);
		printf("%s\n%s\n",swp1, swp2);
		free(swp1);
		free(swp2);
	} else if (r > 0) {
		char * swp1 = allocSwappedStr(str1);
		char * swp2 = allocSwappedStr(str2);
		char * mrg = allocMergedStr(swp2, swp1);
		printf("%s\n",mrg);
		free(mrg);
		free(swp1);
		free(swp2);
	} else {
		char * mrg = allocMergedStr(str1, str2);
		printf("%s\n",mrg);
		free(mrg);
	}
  return r;
}

// Execute tests
void DoTests() {
  TestSwap("");
  TestSwap("a");
  TestSwap("ab");
  TestSwap("abc");
  TestSwap("abcd");

  TestCmp("aaa", "aaa");
  TestCmp("aaa", "aab");
  TestCmp("xyz", "aab");
  TestCmp("a", "aa");
  TestCmp("aaa", "aabccc");
  TestCmp("aaa", "aabccc");
  TestCmp("", "aabccc");
  TestCmp("", "");
  TestCmp("abcdefg", "xyz");

  DoTheJob("abc", "abc");
  DoTheJob("abc", "uvwxyz");
  DoTheJob("uvwxyz", "abc");
}

int main (int argc, char ** argv) {
	if (argc < 3) { // Mismatched arguments, print usage
		printf("Usage: %s string1 string2\n", argv[0]);
    //DoTests();
	} else {
		DoTheJob(argv[1], argv[2]);
	}
	return 0;
}
