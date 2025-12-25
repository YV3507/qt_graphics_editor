#include "shapefactory.h"
#include "ellipse.h"
#include "rectangle.h"
#include <QStringList>

Shape *ShapeFactory::createShape(Shape::ShapeType type)
{
    switch (type) {
    case Shape::Ellipse:
        return new Ellipse();
    case Shape::Rectangle:
        return new Rectangle();
    default:
        return nullptr;
    }
}

Shape *ShapeFactory::createShape(const QString &data)
{
    QStringList parts = data.split(',');
    if (parts.isEmpty()) {
        return nullptr;
    }

    QString typeStr = parts[0];
    Shape *shape = nullptr;

    if (typeStr == "ellipse") {
        shape = new Ellipse();
    } else if (typeStr == "rectangle") {
        shape = new Rectangle();
    } else {
        return nullptr;
    }

    shape->load(data);
    return shape;
}

Shape *ShapeFactory::createShape(const QString &typeStr, const QRectF &rect)
{
    Shape *shape = nullptr;

    if (typeStr == "ellipse" || typeStr == "Ellipse") {
        shape = new Ellipse(rect);
    } else if (typeStr == "rectangle" || typeStr == "Rectangle") {
        shape = new Rectangle(rect);
    }

    return shape;
}