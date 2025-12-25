#ifndef SHAPE_H
#define SHAPE_H

#include <QPainter>
#include <QColor>
#include <QRectF>
#include <QString>

/**
 * @file shape.h
 * @brief 图形基类的头文件
 * 
 * 这个文件定义了所有图形的抽象基类Shape，
 * 提供了绘制、移动、选择等通用接口。
 */

/**
 * @class Shape
 * @brief 图形抽象基类
 * 
 * 所有具体的图形类（如矩形、椭圆等）都继承自这个类。
 * 提供了图形操作的通用接口和属性。
 */
class Shape
{
public:
    /**
     * @enum ShapeType
     * @brief 图形类型枚举
     * 
     * 定义了支持的图形类型，包括椭圆、矩形、线条和多边形。
     */
    enum ShapeType { 
        Ellipse,  ///< 椭圆
        Rectangle, ///< 矩形
        Line,      ///< 线条
        Polygon    ///< 多边形
    };

    /**
     * @brief Shape类的构造函数
     * 
     * 初始化图形的默认属性，如ID、类型、颜色、线宽等。
     */
    Shape();
    
    /**
     * @brief Shape类的析构函数
     * 
     * 虚析构函数，确保派生类的资源能够被正确释放。
     */
    virtual ~Shape();

    /**
     * @brief 绘制图形
     * @param painter 绘图工具
     * 
     * 纯虚函数，派生类必须实现此函数来绘制具体的图形。
     */
    virtual void draw(QPainter *painter) = 0;
    
    /**
     * @brief 绘制选中状态的图形
     * @param painter 绘图工具
     * 
     * 绘制图形的选中状态，如边框和控制点。
     */
    virtual void drawSelected(QPainter *painter);
    
    /**
     * @brief 保存图形数据到字符串
     * @return 包含图形数据的字符串
     * 
     * 纯虚函数，派生类必须实现此函数来保存图形数据。
     */
    virtual QString save() const = 0;
    
    /**
     * @brief 从字符串加载图形数据
     * @param data 包含图形数据的字符串
     * 
     * 纯虚函数，派生类必须实现此函数来加载图形数据。
     */
    virtual void load(const QString &data) = 0;

    /**
     * @brief 判断点是否在图形内
     * @param point 要判断的点
     * @return 如果点在图形内，返回true，否则返回false
     * 
     * 纯虚函数，派生类必须实现此函数来判断点是否在图形内。
     */
    virtual bool contains(const QPointF &point) const = 0;
    
    /**
     * @brief 移动图形
     * @param offset 移动的偏移量
     * 
     * 移动图形的位置。
     */
    virtual void move(const QPointF &offset);
    
    /**
     * @brief 调整图形大小
     * @param newRect 新的矩形区域
     * 
     * 调整图形的大小和位置。
     */
    virtual void resize(const QRectF &newRect);

    /**
     * @brief 获取图形ID
     * @return 图形的ID
     */
    int getId() const;
    
    /**
     * @brief 设置图形ID
     * @param id 新的ID
     */
    void setId(int id);

    /**
     * @brief 获取图形类型
     * @return 图形类型
     */
    ShapeType getType() const;
    
    /**
     * @brief 设置图形类型
     * @param type 新的图形类型
     */
    void setType(ShapeType type);

    /**
     * @brief 获取图形颜色
     * @return 图形的颜色
     */
    QColor getColor() const;
    
    /**
     * @brief 设置图形颜色
     * @param color 新的颜色
     */
    void setColor(const QColor &color);

    /**
     * @brief 获取图形线宽
     * @return 图形的线宽
     */
    int getLineWidth() const;
    
    /**
     * @brief 设置图形线宽
     * @param width 新的线宽
     */
    void setLineWidth(int width);

    /**
     * @brief 获取图形的边界矩形
     * @return 图形的边界矩形
     */
    QRectF getBoundingRect() const;
    
    /**
     * @brief 设置图形的边界矩形
     * @param rect 新的边界矩形
     */
    void setBoundingRect(const QRectF &rect);

    /**
     * @brief 判断图形是否被选中
     * @return 如果图形被选中，返回true，否则返回false
     */
    bool isSelected() const;
    
    /**
     * @brief 设置图形是否被选中
     * @param selected 是否选中
     */
    void setSelected(bool selected);

    /**
     * @brief 判断图形是否填充
     * @return 如果图形填充，返回true，否则返回false
     */
    bool isFilled() const;
    
    /**
     * @brief 设置图形是否填充
     * @param filled 是否填充
     */
    void setFilled(bool filled);

    /**
     * @brief 获取图形填充颜色
     * @return 图形的填充颜色
     */
    QColor getFillColor() const;
    
    /**
     * @brief 设置图形填充颜色
     * @param color 新的填充颜色
     */
    void setFillColor(const QColor &color);

protected:
    int m_id;              ///< 图形的唯一标识符
    ShapeType m_type;      ///< 图形类型
    QColor m_color;        ///< 图形颜色
    int m_lineWidth;       ///< 图形线宽
    QRectF m_boundingRect; ///< 图形的边界矩形
    bool m_selected;       ///< 图形是否被选中
    bool m_filled;         ///< 图形是否填充
    QColor m_fillColor;    ///< 图形填充颜色

    static int s_nextId;   ///< 静态变量，用于生成唯一ID
};

#endif // SHAPE_H