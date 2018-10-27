#pragma once
#include <memory>

#include <SFML/Graphics.hpp>

#include "Brick.h"

using Point = sf::Vector2f;


class BricksLayout
{
public:
    BricksLayout();

    virtual std::shared_ptr<Brick> getNext() = 0;
    virtual bool hasNext() const { return hasNext_; };
    virtual bool hasSpace() const = 0;
    virtual void reset();

protected:
    void setNext(std::shared_ptr<Brick> b) { next_ = b; }
    std::shared_ptr<Brick> next_;
    bool hasNext_ = true;
};


class RectangleBricksLayout : public BricksLayout
{
public:
    RectangleBricksLayout(Point tl, Point br, Point margin);

    virtual std::shared_ptr<Brick> getNext();

    virtual bool hasSpace() const;
    bool hasSpaceHorizontal() const;
    bool hasSpaceVertical() const;
    virtual void reset();

private:
	float calculateNextX() const;
	float calculateNextY() const;

    Point tl_;
    Point br_;
    Point margin_;
};