#include "Polygon.h"

Polygon::Polygon(const std::vector<QPointF> &vertices) {
    vertices_ = vertices;
}

std::vector<QPointF> Polygon::getVertices() {
    return vertices_;
}

void Polygon::AddVertex(const QPointF &vertex) {
    vertices_.emplace_back(vertex);
}

void Polygon::UpdateLastVertex(const QPointF &new_vertex) {
    vertices_.pop_back();
    vertices_.emplace_back(new_vertex);
}

void Polygon::EraseLastVertex() {
    vertices_.pop_back();
}

std::optional<QPointF> Polygon::IntersectRay(const Ray &ray) {
    std::vector<QPointF> intersections;
    double r_dx = ray.getEnd().x() - ray.getBegin().x();
    double r_dy = ray.getEnd().y() - ray.getBegin().y();
    double r_px = ray.getBegin().x();
    double r_py = ray.getBegin().y();
    for (int i = 0; i < vertices_.size(); ++i) {
        double s_px = vertices_[i].x();
        double s_py = vertices_[i].y();
        double s_dx, s_dy;
        if (i != vertices_.size() - 1) {
            s_dx = vertices_[i + 1].x() - vertices_[i].x();
            s_dy = vertices_[i + 1].y() - vertices_[i].y();
        } else {
            s_dx = vertices_[0].x() - vertices_[i].x();
            s_dy = vertices_[0].y() - vertices_[i].y();
        }
        if ((r_dx == s_dx) && (r_dy == s_dy)) {
            continue;
        }
        if (r_dx == 0) {
            r_dx = 1;
        }
        double T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
        double T1 = (s_px + s_dx * T2 - r_px) / r_dx;
        if ((T1 > 0) && (T2 >= 0) && (T2 <= 1)) {
            intersections.emplace_back(r_px + T1 * r_dx, r_py + T1 * r_dy);
        }
    }
    if (intersections.empty()) {
        return std::nullopt;
    }
    QPointF nearestPoint = intersections[0];
    double distance = sqrt(pow(nearestPoint.x() - ray.getBegin().x(), 2) + pow(nearestPoint.y() - ray.getBegin().y(), 2));
    for (auto intersection : intersections) {
        double dist = sqrt(pow(intersection.x() - ray.getBegin().x(), 2) + pow(intersection.y() - ray.getBegin().y(), 2));
        if (dist <= distance) {
            distance = dist;
            nearestPoint = intersection;
        }
    }
    return nearestPoint;
}
