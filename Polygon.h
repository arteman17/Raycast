#pragma once

#include <vector>
#include <QPointF>
#include <optional>

#include "Ray.h"

class Polygon {
public:
    explicit Polygon(const std::vector<QPointF>& vertices);

    std::vector<QPointF> getVertices();

    void AddVertex(const QPointF& vertex);
    void UpdateLastVertex(const QPointF& new_vertex);
    void EraseLastVertex();

    std::optional<QPointF> IntersectRay(const Ray& ray);
private:
    std::vector<QPointF> vertices_;
};
