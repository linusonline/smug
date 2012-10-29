#include <CuTest.h>
#include <engine/body.h>

void Body_newRectangle_returnsNonNull(CuTest* tc)
{
    Body* b = Body_newRectangle(0, 0);
    CuAssertTrue(tc, b != NULL);
    Body_delete(b);
}

void Body_newRectangle_shouldReturnBodyOfRightSize(CuTest* tc)
{
    Body* b = Body_newRectangle(1, 2);
    CuAssertTrue(tc, Body_getWidth(b) == 1);
    CuAssertTrue(tc, Body_getHeight(b) == 2);
    Body_delete(b);
}

void Body_setPosition_shouldSetRightPosition(CuTest* tc)
{
    Body* b = Body_newRectangle(0, 0);
    Body_setPosition(b, 1, 2);
    CuAssertTrue(tc, Body_getX(b) == 1);
    CuAssertTrue(tc, Body_getY(b) == 2);
    Body_delete(b);
}

void Body_colliding_shouldWorkWithDisjointRectsNW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 0, 0);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsNNW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 1, 0);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsN(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 2, 0);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsNNE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 3, 0);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsNE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 4, 0);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsENE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 4, 1);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 4, 2);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsESE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 4, 3);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsSE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 4, 4);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsSSE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 3, 4);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsS(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 2, 4);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsSSW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 1, 4);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsSW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 0, 4);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsWSW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 0, 3);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 0, 2);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithDisjointRectsWNW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(2, 2);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 2, 2);
    Body_setPosition(b2, 0, 1);
    CuAssertTrue(tc, !Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsNW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 0, 0);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsN(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 2, 0);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsNE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 4, 0);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 4, 2);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsSE(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 4, 4);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsS(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 2, 4);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsSW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 0, 4);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsW(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 0, 2);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    Body_delete(b1);
    Body_delete(b2);
}

void Body_colliding_shouldWorkWithJointRectsCompletelyInside(CuTest* tc)
{
    Body* b1 = Body_newRectangle(4, 4);
    Body* b2 = Body_newRectangle(2, 2);
    Body_setPosition(b1, 1, 1);
    Body_setPosition(b2, 2, 2);
    CuAssertTrue(tc, Body_colliding(b1, b2));
    CuAssertTrue(tc, Body_colliding(b2, b1));
    Body_delete(b1);
    Body_delete(b2);
}

CuSuite* BodyTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Body_newRectangle_returnsNonNull);
	SUITE_ADD_TEST(suite, Body_newRectangle_shouldReturnBodyOfRightSize);
	SUITE_ADD_TEST(suite, Body_setPosition_shouldSetRightPosition);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsNW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsNNW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsN);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsNNE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsNE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsENE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsESE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsSE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsSSE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsS);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsSSW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsSW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsWSW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithDisjointRectsWNW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsNW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsN);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsNE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsSE);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsS);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsSW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsW);
	SUITE_ADD_TEST(suite, Body_colliding_shouldWorkWithJointRectsCompletelyInside);

	return suite;
}
