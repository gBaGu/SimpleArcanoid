#include <gtest\gtest.h>
#include <SFML\Graphics.hpp>

#include "Ball.h"


TEST(Ball_isIntersecting, Ball_C_31dot5940533_140dot203354_R_10_Rect_0_0_800_600)
{
	Ball ball(31.5940533, 140.203354);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_TRUE(ball.isIntersecting(rect));
}

TEST(Ball_isIntersecting, Ball_C_800_600_R_10_Rect_0_0_800_600)
{
	Ball ball(800, 600);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_TRUE(ball.isIntersecting(rect));
}

TEST(Ball_isIntersecting, Ball_C_MINUS2_MINUS5_R_10_Rect_0_0_800_600)
{
	Ball ball(-2, -5);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_TRUE(ball.isIntersecting(rect));
}

TEST(Ball_isIntersecting, Ball_C_801_60_R_10_Rect_0_0_800_600)
{
	Ball ball(801, 60);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_TRUE(ball.isIntersecting(rect));
}

TEST(Ball_isIntersecting, Ball_C_821_60_R_10_Rect_0_0_800_600)
{
	Ball ball(821, 60);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_FALSE(ball.isIntersecting(rect));
}

TEST(Ball_isIntersecting, Ball_C_810_610_R_10_Rect_0_0_800_600)
{
	Ball ball(810, 610);
	sf::RectangleShape rect(sf::Vector2f(800, 600));
	ASSERT_FALSE(ball.isIntersecting(rect));
}