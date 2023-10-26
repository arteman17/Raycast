#pragma once

#include <QPointF>
#include <vector>
#include <QPainter>
#include <QPainterPath>

#include "Polygon.h"
#include "Ray.h"

class Controller {
public:
    void setLightSource(const QPointF& lightSource);
    QPointF getLightSource();

    const std::vector<Polygon>& GetPolygons();
    void AddPolygon(const Polygon& polygon);
    void AddVertexToLastPolygon(const QPoint& new_vertex);
    void UpdateLastPolygon(const QPoint& new_vertex);
    void EraseLastVertexInLastPolygon();
    void drawPolygons(QPainter&);

    std::vector<Ray> CastRays();
    void IntersectRays(std::vector<Ray>& rays);
    void RemoveAdjacentRays(std::vector<Ray>& rays);
    Polygon CreateLightArea();
    void drawLightAreas(QPainter&);

    void setLightSources(QPointF);
    std::vector<QPointF> getLightSources();

    void addStaticLights(const QPointF&);
    std::vector<QPointF> getStaticLights();
    void EraseLastStaticLights();
    void drawStaticLights(QPainter&);

    bool getNewPol();
    void setNewPol(bool);
private:
    std::vector<Polygon> polygons_;
    QPointF lightSource_;
    bool newPol_ = true;
    Polygon lightArea_ = Polygon({});
    std::vector<QPointF> lightSources_{};
    std::vector<QPointF> staticLights_{};
};