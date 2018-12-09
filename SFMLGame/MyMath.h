#pragma once
#include <SFML\Graphics.hpp>

constexpr double eps = 0.001;


template <typename T>
struct Segment
{
	Segment(sf::Vector2<T> p1, sf::Vector2<T> p2) : A(p1), B(p2) {}

	sf::Vector2<T> A;
	sf::Vector2<T> B;
};

template <typename T>
struct Line
{
	Line(sf::Vector2<T> p1, sf::Vector2<T> p2) : A(p1), B(p2) {}

	sf::Vector2<T> A;
	sf::Vector2<T> B;
};

template <typename T>
struct Ray
{
	Ray(sf::Vector2<T> p1, sf::Vector2<T> p2) : A(p1), B(p2) {}

	sf::Vector2<T> A;
	sf::Vector2<T> B;
};


inline bool operator==(const sf::RectangleShape& l, const sf::RectangleShape& r)
{
	return l.getOrigin() == r.getOrigin() &&
		l.getPosition() == r.getPosition() &&
		l.getRotation() == r.getRotation() &&
		l.getScale() == r.getScale() &&
		l.getSize() == r.getSize();
}


template <typename T>
T length(sf::Vector2<T> v)
{
	return std::pow(v.x * v.x + v.y * v.y, 0.5);
}

template <typename T>
T length(sf::Vector3<T> v)
{
	return std::pow(v.x * v.x + v.y * v.y + v.z * v.z, 0.5);
}

template <typename T>
sf::Vector2<T> normalize(sf::Vector2<T> v)
{
	auto l = length(v);
	auto ret = v;
	ret.x /= l;
	ret.y /= l;
	return ret;
}

template <typename T>
sf::Vector3<T> normalize(sf::Vector3<T> v)
{
	auto l = length(v);
	auto ret = v;
	ret.x /= l;
	ret.y /= l;
	ret.z /= l;
	return ret;
}

template <typename T>
T scalarMultiplication(sf::Vector2<T> l, sf::Vector2<T> r)
{
	return l.x * r.x + l.y * r.y;
}

template <typename T>
sf::Vector2<T> closestPoint(sf::Vector2<T> p, Segment<T> s)
{
	sf::Vector2<T> segmentVector = s.B - s.A;
	sf::Vector2<T> vectorAtoPoint = p - s.A;
	sf::Vector2<T> vectorBtoPoint = p - s.B;
	T scalarA = scalarMultiplication(segmentVector, vectorAtoPoint);
	T scalarB = scalarMultiplication(segmentVector, vectorBtoPoint);

	//если проекция на прямую, на которой лежит отрезок,
	//находится вне отрезка (скалярная магия),
	//возвращаем расстояние до ближайшей точки отрезка
	if (scalarA < 0 && scalarB < 0 ||
		scalarA > 0 && scalarB > 0)
	{
		return (std::abs(scalarA) < std::abs(scalarB)) ? s.A : s.B;
	}
	else
	{
		T y = (p.y * (s.B.y - s.A.y) * (s.B.y - s.A.y) + s.A.y * (s.B.x - s.A.x) * (s.B.x - s.A.x) - (s.B.x - s.A.x) * (s.A.x - p.x) * (s.B.y - s.A.y)) /
			((s.B.y - s.A.y) * (s.B.y - s.A.y) + (s.B.x - s.A.x) * (s.B.x - s.A.x));
		T x = 0;
		if (s.B.y - s.A.y != 0)
		{
			x = ((s.B.x - s.A.x) * (y - s.A.y)) / (s.B.y - s.A.y) + s.A.x;
		}
		else
		{
			x = p.x - ((s.B.y - s.A.y) * (y - p.y)) / (s.B.x - s.A.x);
		}
		return sf::Vector2<T>(x, y);
	}
}

template <typename T>
T distance(sf::Vector2<T> p, Segment<T> s)
{
	sf::Vector2<T> segmentVector = s.B - s.A;
	sf::Vector2<T> vectorAtoPoint = p - s.A;
	sf::Vector2<T> vectorBtoPoint = p - s.B;
	T scalarA = scalarMultiplication(segmentVector, vectorAtoPoint);
	T scalarB = scalarMultiplication(segmentVector, vectorBtoPoint);

	//если проекция на прямую, на которой лежит отрезок,
	//находится вне отрезка (скалярная магия),
	//возвращаем расстояние до ближайшей точки отрезка
	if (scalarA < 0 && scalarB < 0 ||
		scalarA > 0 && scalarB > 0)
	{
		if (std::abs(scalarA) < std::abs(scalarB))
		{
			return length(vectorAtoPoint);
		}
		else
		{
			return length(vectorBtoPoint);
		}
	}
	else
	{
		auto unitVector = segmentVector / length(segmentVector);
		sf::Vector3<T> geometricMul(0, 0, unitVector.x * vectorAtoPoint.y -
										  unitVector.y * vectorAtoPoint.x);
		return length(geometricMul);
	}
}

//checks if point lies on a line
template <typename T>
bool isContaining(Line<T> l, sf::Vector2<T> p)
{
	return std::abs((p.x - l.A.x) * (l.B.y - l.A.y) - (l.B.x - l.A.x) * (p.y - l.A.y)) < eps;
}

//checks if point lies on a ray
template <typename T>
bool isContaining(Ray<T> r, sf::Vector2<T> p)
{
	return isContaining(Line<T>(r.A, r.B), p) &&
			((p.x - r.A.x) * (r.B.x - r.A.x) >= -eps &&
				(p.y - r.A.y) * (r.B.y - r.A.y) >= -eps);
}

//checks if point lies on a segment
template <typename T>
bool isContaining(Segment<T> s, sf::Vector2<T> p)
{
	return isContaining(Line<T>(s.A, s.B), p) && isInside(p, s.A, s.B);
}

//checks if point is inside of rectangle created by two points
template <typename T>
bool isInside(sf::Vector2<T> p, sf::Vector2<T> boundP1, sf::Vector2<T> boundP2)
{
	return ((p.x - std::min(boundP1.x, boundP2.x) >= -eps) &&
		    (p.x - std::max(boundP1.x, boundP2.x) <= eps)) &&
		   ((p.y - std::min(boundP1.y, boundP2.y) >= -eps) &&
		    (p.y - std::max(boundP1.y, boundP2.y) <= eps));
}

//checks if line divides two points
template <typename T>
bool isDividing(Segment<T> s, sf::Vector2<T> p1, sf::Vector2<T> p2)
{
	//this is z coordinate of a result of vector multiplication
	//of vector {s.A, s.B} and {s.B, p1}
	T z1 = (s.B.x - s.A.x) * (p1.y - s.B.y) - (s.B.y - s.A.y) * (p1.x - s.B.x);
	//this is z coordinate of a result of vector multiplication
	//of vector {s.A, s.B} and {s.B, p2}
	T z2 = (s.B.x - s.A.x) * (p2.y - s.B.y) - (s.B.y - s.A.y) * (p2.x - s.B.x);

	return std::min(z1, z2) <= 0 && std::max(z1, z2) >= 0;
}

//check if one line is parallel to another
template <typename T>
bool isParallel(Line<T> l1, Line<T> l2)
{
	T A1, B1, A2, B2;
	A1 = l1.B.y - l1.A.y;
	B1 = l1.A.x - l1.B.x;
	A2 = l2.B.y - l2.A.y;
	B2 = l2.A.x - l2.B.x;

	return std::abs(A1 * B2 - B1 * A2) < eps;
}

//check if two points of first segment are on different sides
//of second segment and vice versa
template <typename T>
bool isIntersected(Segment<T> s1, Segment<T> s2)
{
	return isDividing(s1, s2.A, s2.B) && isDividing(s2, s1.A, s1.B);
}

//returns an intersection point of two lines if there is one
//bool indicates if an intersection was found
//sf::Vector2<T> contains intersection point if bool is true
template <typename T>
std::pair<bool, sf::Vector2<T>> getIntersectionPoint(Line<T> l1, Line<T> l2)
{
	if (isParallel(l1, l2))
	{
		return std::pair<bool, sf::Vector2<T>>(false, sf::Vector2<T>{ 0, 0 });
	}

	T A1, B1, C1, A2, B2, C2;
	A1 = l1.B.y - l1.A.y;
	B1 = l1.A.x - l1.B.x;
	C1 = l1.A.y * (l1.B.x - l1.A.x) - l1.A.x * (l1.B.y - l1.A.y);
	A2 = l2.B.y - l2.A.y;
	B2 = l2.A.x - l2.B.x;
	C2 = l2.A.y * (l2.B.x - l2.A.x) - l2.A.x * (l2.B.y - l2.A.y);

	T x = (C1 * B2 - B1 * C2) / (B1 * A2 - A1 * B2);
	T y = -(A1 * x + C1) / B1;
	return std::pair<bool, sf::Vector2<T>>(true, sf::Vector2<T>{ x, y });
}

//returns an intersection point of ray and segment if there is one
//bool indicates if an intersection was found
//sf::Vector2<T> contains intersection point if bool is true
template <typename T>
std::pair<bool, sf::Vector2<T>> getIntersectionPoint(Ray<T> r, Segment<T> s)
{
	auto p = getIntersectionPoint(Line<T>(r.A, r.B), Line<T>(s.A, s.B));
	if (!p.first)
	{
		return p;
	}

	//if not on ray on not on segment there is no intersection
	if (!((p.second.x - r.A.x) * (r.B.x - r.A.x) >= 0 &&
			(p.second.y - r.A.y) * (r.B.y - r.A.y) >= 0) ||
		!isInside(p.second, s.A, s.B))
	{
		p.first = false;
	}
	return p;
}

template <typename T>
std::pair<bool, sf::Vector2<T>> getIntersectionPoint(Segment<T> s1, Segment<T> s2)
{
	auto intersection = getIntersectionPoint(Line<float>(s1.A, s1.B),
		Line<float>(s2.A, s2.B));
	if (!intersection.first)
	{
		return intersection;
	}

	if (!isInside(intersection.second, s1.A, s1.B) ||
		!isInside(intersection.second, s2.A, s2.B))
	{
		intersection.first = false;
	}
	return intersection;
}

template <typename T>
bool isInsideByCrossingNumber(sf::Vector2<T> p, const sf::RectangleShape& rect)
{
	sf::Vector2<T> rayVector(1, 0);

	std::vector<sf::Vector2<T>> points;
	for (int i = 0; i < rect.getPointCount(); i++)
	{
		auto point = rect.getPoint(i);
		auto transform = rect.getTransform();
		point = transform.transformPoint(point);
		points.push_back(point);
	}
	//if point lies on one of the sides
	//there is no need to count crossings
	for (auto pIt = points.begin(); pIt != points.end(); pIt++)
	{
		auto nextIt = (std::next(pIt) == points.end()) ? points.begin() : std::next(pIt);
		if (isContaining(Segment<T>(*pIt, *nextIt), p))
		{
			return true;
		}
	}

	const auto firstPointNotOnRayIt = std::find_if_not(points.begin(), points.end(),
		[=](sf::Vector2<T> vertex) { return isContaining(Ray<T>(p, p + rayVector), vertex); });
	//this means that ALL of the vertices lie on the ray
	//considering point doesn`t lie on any of the vertices. See the cycle above
	if (firstPointNotOnRayIt == points.end())
	{
		return false;
	}

	auto pointIt = firstPointNotOnRayIt;
	size_t crossings = 0;
	do
	{
		auto nextPointIt = std::find_if_not(pointIt + 1, points.end(),
			[=](sf::Vector2<T> vertex) { return isContaining(Ray<T>(p, p + rayVector), vertex); });
		if (nextPointIt == points.end())
		{
			nextPointIt = firstPointNotOnRayIt;
		}

		sf::Vector2<T> p1 = *pointIt;
		sf::Vector2<T> p2 = *nextPointIt;
		//if ray does not intersecting any vertices between p1 and p2
		//check for intersection between ray and segment (p1, p2)
		if (nextPointIt == pointIt + 1 ||
			(pointIt == std::prev(points.end()) && nextPointIt == points.begin()))
		{
			if (getIntersectionPoint(Ray<T>(p, p + rayVector), Segment<T>(p1, p2)).first)
			{
				crossings += 1;
			}
		}
		//we know that ray intersects one or more vertice between p1 and p2
		//so we must just check if p1 and p2 are on different sides of ray
		else
		{
			if (isDividing(Segment<T>(p, p + rayVector), p1, p2))
			{
				crossings += 1;
			}
		}
		pointIt = nextPointIt;
	} while (pointIt != firstPointNotOnRayIt);

	return crossings % 2;
}