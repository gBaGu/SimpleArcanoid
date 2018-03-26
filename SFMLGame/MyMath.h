#pragma once
#include <SFML\Graphics.hpp>


template <typename T>
struct Segment
{
	Segment(sf::Vector2<T> p1, sf::Vector2<T> p2) : A(p1), B(p2) {}

	sf::Vector2<T> A;
	sf::Vector2<T> B;
};

template <typename T>
T length(sf::Vector2<T> s)
{
	return std::pow(s.x * s.x + s.y * s.y, 0.5);
}

template <typename T>
T length(sf::Vector3<T> s)
{
	return std::pow(s.x * s.x + s.y * s.y + s.z * s.z, 0.5);
}

template <typename T>
T scalarMultiplication(sf::Vector2<T> l, sf::Vector2<T> r)
{
	return l.x * r.x + l.y * r.y;
}

template <typename T>
T distance(sf::Vector2<T> p, Segment<T> s)
{
	sf::Vector2<T> segmentVector = s.B - s.A;
	sf::Vector2<T> vectorAtoPoint = p - s.A;
	sf::Vector2<T> vectorBtoPoint = p - s.B;
	T scalarA = scalarMultiplication(segmentVector, vectorAtoPoint);
	T scalarB = scalarMultiplication(segmentVector, vectorBtoPoint);

	//если проекция на прямую, на которой лежит отрезок, находится вне отрезка (скалярная магия),
	//возвращаем расстояние до ближайшей точки отрезка
	if (scalarA < 0 && scalarB < 0 ||
		scalarA > 0 && scalarB > 0)
	{
		sf::Vector2<T>& vectorToPoint = (std::abs(scalarA) < std::abs(scalarB) ? vectorAtoPoint : vectorBtoPoint);
		return length(vectorToPoint);
	}
	else
	{
		auto unitVector = segmentVector / length(segmentVector);
		sf::Vector3<T> geometricMul(0, 0, unitVector.x * vectorAtoPoint.y - unitVector.y * vectorAtoPoint.x);
		return length(geometricMul);
	}
}