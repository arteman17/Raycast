#include "Ray.h"

Ray::Ray(const QPointF &begin, const QPointF &end) {
    begin_ = begin;
    end_ = end;
    if (begin.x() == end.x()) {
        if (begin.y() <= end.y()) {
            angle_ = M_PI_2;
        } else {
            angle_ = 3 * M_PI_2;
        }
    } else if (begin.y() == end.y()) {
        if (begin.x() <= end.x()) {
            angle_ = 0;
        } else {
            angle_ = M_PI;
        }
    } else if((end.x() - begin.x()) > 0) {
        if ((end.y() - begin.y()) > 0) {
            angle_ = atan((end.y() - begin.y()) / (end.x() - begin.x()));
        } else {
            angle_ = atan((end.y() - begin.y()) / (end.x() - begin.x())) + 2 * M_PI;
        }
    } else if((end.x() - begin.x()) < 0) {
        if ((end.y() - begin.y()) > 0) {
            angle_ = atan((end.y() - begin.y()) / (end.x() - begin.x())) + M_PI;
        } else {
            angle_ = atan((end.y() - begin.y()) / (end.x() - begin.x())) + M_PI;
        }
    }
}

Ray Ray::Rotate(double angle) const {
    QPointF newEnd(end_.x() * cos(angle) - end_.y() * sin(angle), end_.x() * sin(angle)  + end_.y() * cos(angle));
    Ray newRay(begin_, newEnd);
    return newRay;
}

QPointF Ray::getBegin() const {
    return begin_;
}

QPointF Ray::getEnd() const {
    return end_;
}

double Ray::getAngle() const {
    return angle_;
}

void Ray::setBegin(QPointF newBegin) {
    Ray temp = Ray(newBegin, end_);
    *this = temp;
}

void Ray::setEnd(QPointF newEnd) {
    Ray temp = Ray(begin_, newEnd);
    *this = temp;
}

void Ray::setAngle(double angle) {
   Ray temp = this->Rotate(angle - angle_);
    *this = temp;
}
