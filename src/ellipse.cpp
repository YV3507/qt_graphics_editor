#include "ellipse.h"
#include <QPainterPath>
#include <QStringList>

Ellipse::Ellipse()
{
    setType(Shape::Ellipse);
}

Ellipse::Ellipse(const QRectF &rect)
{
    setType(Shape::Ellipse);
    setBoundingRect(rect);
}

Ellipse::~Ellipse()
{
}

void Ellipse::draw(QPainter *painter)
{
    painter->save();

    // 设置画笔
    QPen pen(m_color, m_lineWidth);
    if (m_selected) {
        pen.setStyle(Qt::DashLine);
    }
    painter->setPen(pen);

    // 设置画刷
    if (m_filled) {
        painter->setBrush(m_fillColor);
    } else {
        painter->setBrush(Qt::NoBrush);
    }

    // 绘制椭圆
    painter->drawEllipse(m_boundingRect);

    // 如果选中，绘制控制点
    if (m_selected) {
        painter->setPen(Qt::black);
        painter->setBrush(Qt::white);
        QRectF handle = QRectF(-4, -4, 8, 8);

        // 四个角的控制点
        handle.moveCenter(m_boundingRect.topLeft());
        painter->drawRect(handle);
        handle.moveCenter(m_boundingRect.topRight());
        painter->drawRect(handle);
        handle.moveCenter(m_boundingRect.bottomLeft());
        painter->drawRect(handle);
        handle.moveCenter(m_boundingRect.bottomRight());
        painter->drawRect(handle);
    }

    painter->restore();
}

QString Ellipse::save() const
{
    // 格式: ellipse,id,x,y,width,height,color,lineWidth,filled,fillColor
    QString colorStr = m_color.name(QColor::HexRgb);
    QString fillColorStr = m_fillColor.name(QColor::HexRgb);

    return QString("ellipse,%1,%2,%3,%4,%5,%6,%7,%8,%9")
            .arg(m_id)
            .arg(m_boundingRect.x())
            .arg(m_boundingRect.y())
            .arg(m_boundingRect.width())
            .arg(m_boundingRect.height())
            .arg(colorStr)
            .arg(m_lineWidth)
            .arg(m_filled ? "true" : "false")
            .arg(fillColorStr);
}

void Ellipse::load(const QString &data)
{
    QStringList parts = data.split(',');
    if (parts.size() != 10) {
        return;
    }

    m_id = parts[1].toInt();
    m_boundingRect.setRect(
        parts[2].toDouble(),
        parts[3].toDouble(),
        parts[4].toDouble(),
        parts[5].toDouble()
    );
    m_color = QColor(parts[6]);
    m_lineWidth = parts[7].toInt();
    m_filled = (parts[8] == "true");
    m_fillColor = QColor(parts[9]);
}

bool Ellipse::contains(const QPointF &point) const
{
    return createPath().contains(point);
}

QPainterPath Ellipse::createPath() const
{
    QPainterPath path;
    path.addEllipse(m_boundingRect);
    return path;
}
