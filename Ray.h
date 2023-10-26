#pragma once

#include <QPointF>
#include <cmath>

class Ray {
public:
    Ray(const QPointF& begin, const QPointF& end);

    [[nodiscard]] Ray Rotate(double angle) const;

    [[nodiscard]] QPointF getBegin() const;
    [[nodiscard]] QPointF getEnd() const;
    [[nodiscard]] double getAngle() const;

    void setBegin(QPointF newBegin);
    void setEnd(QPointF newEnd);
    void setAngle(double angle);
private:
    QPointF begin_;
    QPointF end_;
    double angle_;
};