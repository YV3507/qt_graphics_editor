#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

/**
 * @file rectangle.h
 * @brief 矩形类的头文件
 * 
 * 这个文件定义了矩形图形类，继承自Shape基类。
 * 矩形是一种基本的图形类型，具有四个直角的四边形。
 */

/**
 * @class Rectangle
 * @brief 矩形图形类
 * 
 * 表示一个矩形图形，可以被绘制、移动、选择等。
 * 矩形由其边界矩形定义，具有四个直角。
 */
class Rectangle : public Shape
{
public:
    /**
     * @brief Rectangle类的默认构造函数
     * 
     * 创建一个默认的矩形对象，初始位置和大小为零。
     */
    Rectangle();
    
    /**
     * @brief Rectangle类的构造函数
     * @param rect 矩形的边界矩形
     * 
     * 创建一个具有指定边界矩形的矩形对象。
     */
    Rectangle(const QRectF &rect);
    
    /**
     * @brief Rectangle类的析构函数
     * 
     * 虚析构函数，确保资源能够被正确释放。
     */
    ~Rectangle() override;

    /**
     * @brief 绘制矩形
     * @param painter 绘图工具
     * 
     * 重写基类的draw方法，绘制矩形图形。
     */
    void draw(QPainter *painter) override;
    
    /**
     * @brief 保存矩形数据到字符串
     * @return 包含矩形数据的字符串
     * 
     * 重写基类的save方法，将矩形的属性保存为字符串。
     */
    QString save() const override;
    
    /**
     * @brief 从字符串加载矩形数据
     * @param data 包含矩形数据的字符串
     * 
     * 重写基类的load方法，从字符串中加载矩形的属性。
     */
    void load(const QString &data) override;

    /**
     * @brief 判断点是否在矩形内
     * @param point 要判断的点
     * @return 如果点在矩形内，返回true，否则返回false
     * 
     * 重写基类的contains方法，判断点是否在矩形内。
     */
    bool contains(const QPointF &point) const override;

private:
    /**
     * @brief 创建矩形的路径
     * @return 矩形的路径
     * 
     * 创建一个表示矩形的QPainterPath对象，用于绘制和判断点是否在矩形内。
     */
    QPainterPath createPath() const;
};

#endif // RECTANGLE_H