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
        next_->setPosition(next_->getPosition() + sf::Vector2f(margin_.x, 0.0f));
    }
    else
    {
        next_->setPosition({tl_.x, next_->getPosition().y + margin_.y});
    }
    
    return old;
}

bool RectangleBricksLayout::hasSpace() const
{
    return hasSpaceVertical() || hasSpaceHorizontal();
}

bool RectangleBricksLayout::hasSpaceHorizontal() const
{
    return (next_->getPosition().x + margin_.x <= br_.x);
}

bool RectangleBricksLayout::hasSpaceVertical() const
{
    return (next_->getPosition().y + margin_.y <= br_.y);
}

void RectangleBricksLayout::reset()
{
    setNext(std::make_shared<Brick>(tl_.x, tl_.y));
}
