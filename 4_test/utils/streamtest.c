#include <string.h>

#include <utils/stream.h>
#include <CuTest.h>

void Stream_new_shouldReturnNonNull(CuTest* tc)
{
    Stream* s = Stream_new("test");
    CuAssertTrue(tc, s != NULL);
    Stream_delete(s);
}

void Stream_newCopied_shouldReturnNonNull(CuTest* tc)
{
    Stream* s = Stream_newCopied("test");
    CuAssertTrue(tc, s != NULL);
    Stream_delete(s);
}

void Stream_eof_shouldBeTrueForEmptyString(CuTest* tc)
{
    Stream* s = Stream_new("");
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_eof_shouldBeFalseForNonEmptyString(CuTest* tc)
{
    Stream* s = Stream_new("test");
    CuAssertTrue(tc, !Stream_eof(s));
    Stream_delete(s);
}

void Stream_eof_shouldBeFalseForWsString(CuTest* tc)
{
    Stream* s = Stream_new("   ");
    CuAssertTrue(tc, !Stream_eof(s));
    Stream_delete(s);
}

void Stream_eatWhiteSpace_shouldConsumeWsString(CuTest* tc)
{
    Stream* s = Stream_new("   ");
    Stream_eatWhiteSpace(s);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_readChars_shouldConsumeChars(CuTest* tc)
{
    Stream* s = Stream_new("123");
    char* str = Stream_readChars(s, 3);
    CuAssertTrue(tc, Stream_eof(s));
    free(str);
    Stream_delete(s);
}

void Stream_readChars_shouldConsumeRightNumberOfChars(CuTest* tc)
{
    Stream* s = Stream_new("12");
    char* str = Stream_readChars(s, 1);
    free(str);
    CuAssertTrue(tc, !Stream_eof(s));
    str = Stream_readChars(s, 1);
    free(str);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_readChars_shouldReturnRightChars(CuTest* tc)
{
    Stream* s = Stream_new("123");
    char* str = Stream_readChars(s, 2);
    CuAssertTrue(tc, strcmp(str, "12") == 0);
    free(str);
    Stream_delete(s);
}

void Stream_eatWhiteSpace_shouldConsumeWsFromString(CuTest* tc)
{
    Stream* s = Stream_new("   a");
    Stream_eatWhiteSpace(s);
    char* str = Stream_readChars(s, 1);
    CuAssertTrue(tc, strcmp(str, "a") == 0);
    free(str);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_eatWhiteSpace_shouldConsumeTabChars(CuTest* tc)
{
    Stream* s = Stream_new("	");
    Stream_eatWhiteSpace(s);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_eatWhiteSpace_shouldConsumeNewlines(CuTest* tc)
{
    Stream* s = Stream_new("\n");
    Stream_eatWhiteSpace(s);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_eatWhiteSpace_shouldConsumeMixedWs(CuTest* tc)
{
    Stream* s = Stream_new("	 \n \n	");
    Stream_eatWhiteSpace(s);
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_readToken_shouldReturnNullWhenTokenNotFound(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    CuAssertTrue(tc, !Stream_readToken(s, "two"));
    Stream_delete(s);
}

void Stream_readToken_shouldOnlyReadSeparatedTokens(CuTest* tc)
{
    Stream* s = Stream_new("onetwo");
    CuAssertTrue(tc, !Stream_readToken(s, "one"));
    Stream_delete(s);
}

void Stream_readToken_shouldNotConsumeCharsWhenTokenNotFound(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    Stream_readToken(s, "two");
    CuAssertTrue(tc, Stream_readToken(s, "one"));
    Stream_delete(s);
}

void Stream_readToken_shouldNotConsumeCharsOnNonSeparatedTokens(CuTest* tc)
{
    Stream* s = Stream_new("onetwo");
    Stream_readToken(s, "one");
    CuAssertTrue(tc, Stream_readToken(s, "onetwo"));
    Stream_delete(s);
}

void Stream_readToken_shouldFindOnlyTokenInStream(CuTest* tc)
{
    Stream* s = Stream_new("one");
    CuAssertTrue(tc, Stream_readToken(s, "one"));
    Stream_delete(s);
}

void Stream_readToken_shouldFindFirstToken(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    CuAssertTrue(tc, Stream_readToken(s, "one"));
    Stream_delete(s);
}

void Stream_readToken_shouldConsumeOnlyTokenInString(CuTest* tc)
{
    Stream* s = Stream_new("one");
    Stream_readToken(s, "one");
    CuAssertTrue(tc, Stream_eof(s));
    Stream_delete(s);
}

void Stream_readToken_shouldConsumeTokenAndWhiteSpace(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    Stream_readToken(s, "one");
    char* string = Stream_readChars(s, 3);
    CuAssertTrue(tc, strcmp(string, "two") == 0);
    Stream_delete(s);
}

void Stream_readToken_shouldIgnoreBeginningWhiteSpace(CuTest* tc)
{
    Stream* s = Stream_new("  one two");
    CuAssertTrue(tc, Stream_readToken(s, "one"));
    Stream_delete(s);
}

void Stream_readToken_shouldIncludeWsChars(CuTest* tc)
{
    Stream* s = Stream_new("space tab	newline\n trailing");
    CuAssertTrue(tc, Stream_readToken(s, "space tab	newline\n"));
    CuAssertTrue(tc, Stream_readToken(s, "trailing"));
    Stream_delete(s);
}

void Stream_readUntilWs_shouldStopAtSpace(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    char* str = Stream_readUntilWs(s);
    CuAssertTrue(tc, strcmp(str, "one") == 0);
    CuAssertTrue(tc, Stream_readToken(s, "two"));
    free(str);
    Stream_delete(s);
}

void Stream_readUntilWs_shouldStopAtTab(CuTest* tc)
{
    Stream* s = Stream_new("one	two");
    char* str = Stream_readUntilWs(s);
    CuAssertTrue(tc, strcmp(str, "one") == 0);
    CuAssertTrue(tc, Stream_readToken(s, "two"));
    free(str);
    Stream_delete(s);
}

void Stream_readUntilWs_shouldStopAtNewline(CuTest* tc)
{
    Stream* s = Stream_new("one\ntwo");
    char* str = Stream_readUntilWs(s);
    CuAssertTrue(tc, strcmp(str, "one") == 0);
    CuAssertTrue(tc, Stream_readToken(s, "two"));
    free(str);
    Stream_delete(s);
}

void Stream_readString_shouldStopAtWs(CuTest* tc)
{
    Stream* s = Stream_new("one two");
    char* str = Stream_readString(s);
    CuAssertTrue(tc, strcmp(str, "one") == 0);
    CuAssertTrue(tc, Stream_readToken(s, "two"));
    free(str);
    Stream_delete(s);
}

void Stream_readString_shouldReadFromStartingQuoteToEndingQuote(CuTest* tc)
{
    Stream* s = Stream_new("\"one two\" three");
    char* str = Stream_readString(s);
    CuAssertTrue(tc, Stream_readToken(s, "three"));
    free(str);
    Stream_delete(s);
}

void Stream_readString_shouldIncludeQuotes(CuTest* tc)
{
    Stream* s = Stream_new("\"one two\" three");
    char* str = Stream_readString(s);
    CuAssertTrue(tc, strcmp(str, "\"one two\"") == 0);
    free(str);
    Stream_delete(s);
}

void Stream_readString_shouldIgnoreNonStartingQuotes(CuTest* tc)
{
    Stream* s = Stream_new("one\"two\"three");
    char* str = Stream_readString(s);
    CuAssertTrue(tc, strcmp(str, "one\"two\"three") == 0);
    free(str);
    Stream_delete(s);
}

void Stream_readInt_shouldReturnZeroOnNonDigit(CuTest* tc)
{
    Stream* s = Stream_new("one1");
    int i = Stream_readInt(s);
    char* str = Stream_readString(s);
    CuAssertTrue(tc, i == 0 && strcmp(str, "one1") == 0);
    free(str);
    Stream_delete(s);
}

void Stream_readInt_shouldParseDigit(CuTest* tc)
{
    Stream* s = Stream_new("1");
    int i = Stream_readInt(s);
    CuAssertTrue(tc, i == 1 && Stream_eof(s));
    Stream_delete(s);
}

void Stream_readInt_shouldParseDigitsUpToNonDigit(CuTest* tc)
{
    Stream* s = Stream_new("12a");
    int i = Stream_readInt(s);
    char* str = Stream_readString(s);
    CuAssertTrue(tc, i == 12 && strcmp(str, "a") == 0);
    free(str);
    Stream_delete(s);
}

CuSuite* StreamTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Stream_new_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, Stream_newCopied_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, Stream_eof_shouldBeTrueForEmptyString);
	SUITE_ADD_TEST(suite, Stream_eof_shouldBeFalseForNonEmptyString);
	SUITE_ADD_TEST(suite, Stream_eof_shouldBeFalseForWsString);
	SUITE_ADD_TEST(suite, Stream_eatWhiteSpace_shouldConsumeWsString);
	SUITE_ADD_TEST(suite, Stream_readChars_shouldConsumeChars);
	SUITE_ADD_TEST(suite, Stream_readChars_shouldConsumeRightNumberOfChars);
	SUITE_ADD_TEST(suite, Stream_readChars_shouldReturnRightChars);
	SUITE_ADD_TEST(suite, Stream_eatWhiteSpace_shouldConsumeWsFromString);
	SUITE_ADD_TEST(suite, Stream_eatWhiteSpace_shouldConsumeTabChars);
	SUITE_ADD_TEST(suite, Stream_eatWhiteSpace_shouldConsumeNewlines);
	SUITE_ADD_TEST(suite, Stream_eatWhiteSpace_shouldConsumeMixedWs);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldReturnNullWhenTokenNotFound);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldOnlyReadSeparatedTokens);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldNotConsumeCharsWhenTokenNotFound);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldNotConsumeCharsOnNonSeparatedTokens);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldFindOnlyTokenInStream);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldFindFirstToken);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldConsumeOnlyTokenInString);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldConsumeTokenAndWhiteSpace);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldIgnoreBeginningWhiteSpace);
	SUITE_ADD_TEST(suite, Stream_readToken_shouldIncludeWsChars);
	SUITE_ADD_TEST(suite, Stream_readUntilWs_shouldStopAtSpace);
	SUITE_ADD_TEST(suite, Stream_readUntilWs_shouldStopAtTab);
	SUITE_ADD_TEST(suite, Stream_readUntilWs_shouldStopAtNewline);
	SUITE_ADD_TEST(suite, Stream_readString_shouldStopAtWs);
	SUITE_ADD_TEST(suite, Stream_readString_shouldReadFromStartingQuoteToEndingQuote);
	SUITE_ADD_TEST(suite, Stream_readString_shouldIncludeQuotes);
	SUITE_ADD_TEST(suite, Stream_readString_shouldIgnoreNonStartingQuotes);
	SUITE_ADD_TEST(suite, Stream_readInt_shouldReturnZeroOnNonDigit);
	SUITE_ADD_TEST(suite, Stream_readInt_shouldParseDigit);
	SUITE_ADD_TEST(suite, Stream_readInt_shouldParseDigitsUpToNonDigit);

	return suite;
}
