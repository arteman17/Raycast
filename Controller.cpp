#include "Controller.h"

void Controller::setLightSource(const QPointF &lightSource) {
    lightSource_ = lightSource;
}

QPointF Controller::getLightSource() {
    return lightSource_;
}

////////////////////////////////////////////////////////////////////

const std::vector<Polygon> &Controller::GetPolygons() {
    return polygons_;
}

void Controller::AddPolygon(const Polygon &polygon) {
    polygons_.emplace_back(polygon);
}

void Controller::AddVertexToLastPolygon(const QPoint &new_vertex) {
    polygons_[polygons_.size() - 1].AddVertex(new_vertex);
}

void Controller::UpdateLastPolygon(const QPoint &new_vertex) {
    polygons_[polygons_.size() - 1].UpdateLastVertex(new_vertex);
}

void Controller::EraseLastVertexInLastPolygon() {
    if (polygons_.size() != 1) {
        if (polygons_[polygons_.size() - 1].getVertices().size() == 1) {
            polygons_.pop_back();
            newPol_ = true;
        } else {
            polygons_[polygons_.size() - 1].EraseLastVertex();
        }
    }
}

void Controller::drawPolygons(QPainter& painter) {
    QPainterPath path;
    for (Polygon i: polygons_) {
        path.moveTo(i.getVertices().back());
        for (int j = 0; j < i.getVertices().size(); ++j) {
            path.lineTo(i.getVertices()[j]);
        }
    }
    painter.drawPath(path);
}

////////////////////////////////////////////////////////////////////

std::vector<Ray> Controller::CastRays() {
    std::vector<Ray> rays;
    for (Polygon& polygon : polygons_) {
        for (int j = 0; j < polygon.getVertices().size(); ++j) {
            Ray ray(lightSource_, polygon.getVertices()[j]);
            rays.emplace_back(ray);
            rays.emplace_back(ray.Rotate(0.0001));
            rays.emplace_back(ray.Rotate(-0.0001));
        }
    }
    std::sort(rays.begin(), rays.end(), [](const auto& lhs, const auto& rhs){return lhs.getAngle() < rhs.getAngle();});
    return rays;
}

void Controller::IntersectRays(std::vector<Ray> &rays) {
    for (Ray &ray : rays) {
        std::vector<QPointF> inter;
        for (Polygon polygon : polygons_) {
            if (polygon.IntersectRay(ray).has_value()) {
                inter.emplace_back(polygon.IntersectRay(ray).value());
            }
        }
        if (inter.empty()) return;
        QPointF newEnd;
        double endDist = sqrt(pow(inter[0].x() - ray.getBegin().x(), 2) + pow(inter[0].y() - ray.getBegin().y(), 2));
        for (QPointF interPoint : inter) {
            double dist = sqrt(pow(interPoint.x() - ray.getBegin().x(), 2) + pow(interPoint.y() - ray.getBegin().y(), 2));
            if (dist <= endDist) {
                newEnd = interPoint;
                endDist = dist;
            }
        }
        ray.setEnd(newEnd);
    }
}

void Controller::RemoveAdjacentRays(std::vector<Ray> &rays) {
    std::vector<Ray> newRays;
    newRays.push_back(rays[0]);
    int i = 0;
    for (int j = i + 1; j < rays.size(); ++j) {
        if (std::abs(newRays[i].getAngle() - rays[j].getAngle()) < 0.00001) {
            continue;
        } else {
            i++;
            newRays.push_back(rays[j]);
        }
    }
    rays.clear();
    rays = newRays;
}

Polygon Controller::CreateLightArea() {
    std::vector<Ray> rays = CastRays();
    IntersectRays(rays);
    //RemoveAdjacentRays(rays);
    std::vector<QPointF> vertices;
    vertices.reserve(rays.size());
    for (auto ray : rays) {
        vertices.emplace_back(ray.getEnd());
    }
    Polygon polygon(vertices);
    return polygon;
}

void Controller::drawLightAreas(QPainter& painter) {
    if(!lightSource_.isNull()) {
        for (QPointF staticLight : staticLights_) {
            setLightSource(staticLight);
            lightArea_ = CreateLightArea();
            QPainterPath area;
            area.moveTo(lightArea_.getVertices().back());
            for (int j = 0; j < lightArea_.getVertices().size(); ++j) {
                area.lineTo(lightArea_.getVertices()[j]);
            }
            QRadialGradient grad(lightSource_, 300);
            grad.setStops(QGradientStops({{0, QColor(0, 0, 255, 100)}, {1, Qt::transparent}}));
            painter.fillPath(area, grad);
        }
        for (auto lightSource : lightSources_) {
            setLightSource(lightSource);
            lightArea_ = CreateLightArea();
            QPainterPath area;
            area.moveTo(lightArea_.getVertices().back());
            for (int j = 0; j < lightArea_.getVertices().size(); ++j) {
                area.lineTo(lightArea_.getVertices()[j]);
            }
            QRadialGradient grad(lightSource_, 400);
            grad.setStops(QGradientStops({{0, QColor(255, 0, 0, 28)}, {1, Qt::transparent}}));
            painter.fillPath(area, grad);
        }
    }

}

////////////////////////////////////////////////////////////////////

void Controller::setLightSources(QPointF light_source) {
    lightSources_.clear();
    lightSources_.push_back(light_source);

    double angle = 0;
    for (int i = 0; i < 8; ++i) {
        lightSources_.emplace_back(light_source.x() + 20 * cos(angle), light_source.y() + 20 * sin(angle));
        angle +=  2 * M_PI / 8;
    }
}

std::vector<QPointF> Controller::getLightSources() {
    return lightSources_;
}

////////////////////////////////////////////////////////////////////

void Controller::addStaticLights(const QPointF &staticLight) {
    staticLights_.emplace_back(staticLight);
}

std::vector<QPointF> Controller::getStaticLights() {
    return staticLights_;
}

void Controller::EraseLastStaticLights() {
    if (!staticLights_.empty()) {
        staticLights_.pop_back();
    }
}

void Controller::drawStaticLights(QPainter& painter) {
    for (QPointF staticLight : staticLights_) {
        painter.drawEllipse(staticLight, 1.5, 1.5);
    }
}

////////////////////////////////////////////////////////////////////

bool Controller::getNewPol() {
    return newPol_;
}

void Controller::setNewPol(bool newNewPol) {
    newPol_ = newNewPol;
}
