#include <gtest\gtest.h>
#include <SFML\Graphics.hpp>

#include "MyMath.h"


//isInsideByCrossingNumber tests
TEST(isInsideByCrossingNumber, Point_30_30_Rect_0_0_60_60)
{
	sf::Vector2f point(30, 30);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_0_0_Rect_0_0_60_60)
{
	sf::Vector2f point(0, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_0_10_Rect_0_0_60_60)
{
	sf::Vector2f point(0, 10);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_0_60_Rect_0_0_60_60)
{
	sf::Vector2f point(0, 60);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_20_60_Rect_0_0_60_60)
{
	sf::Vector2f point(20, 60);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_60_60_Rect_0_0_60_60)
{
	sf::Vector2f point(60, 60);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_60_40_Rect_0_0_60_60)
{
	sf::Vector2f point(60, 40);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_60_0_Rect_0_0_60_60)
{
	sf::Vector2f point(60, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_50_0_Rect_0_0_60_60)
{
	sf::Vector2f point(50, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_0_0_Rect_0_0_60_60_Rotation_30)
{
	sf::Vector2f point(0, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	rect.rotate(30.0f);
	ASSERT_TRUE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_MINUS1_0_Rect_0_0_60_60)
{
	sf::Vector2f point(-1, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	ASSERT_FALSE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_63_1_Rect_55_47_100_38)
{
	sf::Vector2f point(63, 1);
	sf::RectangleShape rect(sf::Vector2f(100, 38));
	rect.setPosition(55, 47);
	ASSERT_FALSE(isInsideByCrossingNumber(point, rect));
}

TEST(isInsideByCrossingNumber, Point_MINUS1_0_Rect_0_0_60_60_Rotation_30)
{
	sf::Vector2f point(-1, 0);
	sf::RectangleShape rect(sf::Vector2f(60, 60));
	rect.rotate(30.0f);
	ASSERT_FALSE(isInsideByCrossingNumber(point, rect));
}


//closestPoint tests
TEST(closestPoint, Point_0_2dot5_Segment_0_0_4_2)
{
	sf::Vector2f point(0.0, 2.5);
	Segment<float> s({ 0.0, 0.0 }, { 4.0, 2.0 });
	auto res = closestPoint(point, s);
	ASSERT_EQ(res, sf::Vector2f(1.0, 0.5));
}

TEST(closestPoint, Point_0_10_Segment_0_0_4_0)
{
	sf::Vector2f point(0.0, 10.0);
	Segment<float> s({ 0.0, 0.0 }, { 4.0, 0.0 });
	auto res = closestPoint(point, s);
	ASSERT_EQ(res, sf::Vector2f(0.0, 0.0));
}

TEST(closestPoint, Point_MINUS10_0_Segment_0_0_4_0)
{
	sf::Vector2f point(-10.0, 0.0);
	Segment<float> s({ 0.0, 0.0 }, { 4.0, 0.0 });
	auto res = closestPoint(point, s);
	ASSERT_EQ(res, sf::Vector2f(0.0, 0.0));
}

TEST(closestPoint, Point_0_2_Segment_4_5_MINUS3_6)
{
	sf::Vector2f point(0.0, 2.0);
	Segment<float> s({ 4.0, 5.0 }, { -3.0, 6.0 });
	auto res = closestPoint(point, s);
	ASSERT_EQ(res, sf::Vector2f(0.5, 5.5));
}

TEST(closestPoint, Point_1_0_Segment_4_5_4_0)
{
	sf::Vector2f point(1.0, 0.0);
	Segment<float> s({ 4.0, 5.0 }, { 4.0, 0.0 });
	auto res = closestPoint(point, s);
	ASSERT_EQ(res, sf::Vector2f(4.0, 0.0));
}