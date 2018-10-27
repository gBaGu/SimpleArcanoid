#include "BricksLayout.h"

#include <stdexcept>


BricksLayout::BricksLayout()
{
    reset();
}

void BricksLayout::reset()
{
    setNext(std::make_shared<Brick>(0.0f, 0.0f));
}


RectangleBricksLayout::RectangleBricksLayout(Point tl, Point br, Point margin)
    : BricksLayout(), tl_(tl), br_(br), margin_(margin)
{
    reset();
}

std::shared_ptr<Brick> RectangleBricksLayout::getNext()
{
    if (!hasNext())
    {
        throw std::runtime_error("No more space in layout.");
    }
    hasNext_ = hasSpace();
    
    std::shared_ptr<Brick> old = std::make_shared<Brick>(*next_);
    if (hasSpaceHorizontal())
    {
        next_->setPosition(calculateNextX(), next_->getPosition().y);
    }
    else
    {
        next_->setPosition({ tl_.x + Brick::DEFAULT_WIDTH / 2, calculateNextY() });
    }
    
    return old;
}

bool RectangleBricksLayout::hasSpace() const
{
    return hasSpaceVertical() || hasSpaceHorizontal();
}

bool RectangleBricksLayout::hasSpaceHorizontal() const
{
    return (calculateNextX() <= br_.x);
}

bool RectangleBricksLayout::hasSpaceVertical() const
{
    return (calculateNextY() <= br_.y);
}

void RectangleBricksLayout::reset()
{
    setNext(std::make_shared<Brick>(tl_.x + Brick::DEFAULT_WIDTH / 2,
		tl_.y + Brick::DEFAULT_HEIGHT / 2));
}

float RectangleBricksLayout::calculateNextX() const
{
	return next_->getPosition().x + Brick::DEFAULT_WIDTH + margin_.x;
}

float RectangleBricksLayout::calculateNextY() const
{
	return next_->getPosition().y + Brick::DEFAULT_HEIGHT + margin_.y;
}
