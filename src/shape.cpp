#include "shape.h"

int Shape::s_nextId = 1;

Shape::Shape()
    : m_id(s_nextId++),
      m_type(Ellipse),
      m_color(Qt::black),
      m_lineWidth(2),
      m_boundingRect(),
      m_selected(false),
      m_filled(false),
      m_fillColor(Qt::white)
{
}

Shape::~Shape()
{
}

void Shape::move(const QPointF &offset)
{
    m_boundingRect.translate(offset);
}

void Shape::resize(const QRectF &newRect)
{
    m_boundingRect = newRect;
}

void Shape::drawSelected(QPainter *painter)
{
    painter->save();

    // 绘制虚线边框
    QPen pen(Qt::blue, 1, Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(m_boundingRect.adjusted(-2, -2, 2, 2));

    // 绘制八个控制点
    const int handleSize = 4;
    QBrush handleBrush(Qt::white);
    QPen handlePen(Qt::blue, 1);
    painter->setBrush(handleBrush);
    painter->setPen(handlePen);

    // 四个角的控制点
    painter->drawRect(QRectF(m_boundingRect.topLeft().x() - handleSize, 
                             m_boundingRect.topLeft().y() - handleSize, 
                             handleSize * 2, handleSize * 2));
    
    painter->drawRect(QRectF(m_boundingRect.topRight().x() - handleSize, 
                             m_boundingRect.topRight().y() - handleSize, 
                             handleSize * 2, handleSize * 2));
    
    painter->drawRect(QRectF(m_boundingRect.bottomLeft().x() - handleSize, 
                             m_boundingRect.bottomLeft().y() - handleSize, 
                             handleSize * 2, handleSize * 2));
    
    painter->drawRect(QRectF(m_boundingRect.bottomRight().x() - handleSize, 
                             m_boundingRect.bottomRight().y() - handleSize, 
                             handleSize * 2, handleSize * 2));

    painter->restore();
}

int Shape::getId() const
{
    return m_id;
}

void Shape::setId(int id)
{
    m_id = id;
}

Shape::ShapeType Shape::getType() const
{
    return m_type;
}

void Shape::setType(ShapeType type)
{
    m_type = type;
}

QColor Shape::getColor() const
{
    return m_color;
}

void Shape::setColor(const QColor &color)
{
    m_color = color;
}

int Shape::getLineWidth() const
{
    return m_lineWidth;
}

void Shape::setLineWidth(int width)
{
    m_lineWidth = width;
}

QRectF Shape::getBoundingRect() const
{
    return m_boundingRect;
}

void Shape::setBoundingRect(const QRectF &rect)
{
    m_boundingRect = rect;
}

bool Shape::isSelected() const
{
    return m_selected;
}

void Shape::setSelected(bool selected)
{
    m_selected = selected;
}

bool Shape::isFilled() const
{
    return m_filled;
}

void Shape::setFilled(bool filled)
{
    m_filled = filled;
}

QColor Shape::getFillColor() const
{
    return m_fillColor;
}

void Shape::setFillColor(const QColor &color)
{
    m_fillColor = color;
}