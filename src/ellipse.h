#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "shape.h"

/**
 * @file ellipse.h
 * @brief 椭圆类的头文件
 * 
 * 这个文件定义了椭圆图形类，继承自Shape基类。
 * 椭圆是一种基本的图形类型，是平面上到两个定点的距离之和为常数的点的轨迹。
 */

/**
 * @class Ellipse
 * @brief 椭圆图形类
 * 
 * 表示一个椭圆图形，可以被绘制、移动、选择等。
 * 椭圆由其边界矩形定义，边界矩形的中心是椭圆的中心，
 * 边界矩形的宽和高分别是椭圆的长轴和短轴的两倍。
 */
class Ellipse : public Shape
{
public:
    /**
     * @brief Ellipse类的默认构造函数
     * 
     * 创建一个默认的椭圆对象，初始位置和大小为零。
     */
    Ellipse();
    
    /**
     * @brief Ellipse类的构造函数
     * @param rect 椭圆的边界矩形
     * 
     * 创建一个具有指定边界矩形的椭圆对象。
     */
    Ellipse(const QRectF &rect);
    
    /**
     * @brief Ellipse类的析构函数
     * 
     * 虚析构函数，确保资源能够被正确释放。
     */
    ~Ellipse() override;

    /**
     * @brief 绘制椭圆
     * @param painter 绘图工具
     * 
     * 重写基类的draw方法，绘制椭圆图形。
     */
    void draw(QPainter *painter) override;
    
    /**
     * @brief 保存椭圆数据到字符串
     * @return 包含椭圆数据的字符串
     * 
     * 重写基类的save方法，将椭圆的属性保存为字符串。
     */
    QString save() const override;
    
    /**
     * @brief 从字符串加载椭圆数据
     * @param data 包含椭圆数据的字符串
     * 
     * 重写基类的load方法，从字符串中加载椭圆的属性。
     */
    void load(const QString &data) override;

    /**
     * @brief 判断点是否在椭圆内
     * @param point 要判断的点
     * @return 如果点在椭圆内，返回true，否则返回false
     * 
     * 重写基类的contains方法，判断点是否在椭圆内。
     */
    bool contains(const QPointF &point) const override;

private:
    /**
     * @brief 创建椭圆的路径
     * @return 椭圆的路径
     * 
     * 创建一个表示椭圆的QPainterPath对象，用于绘制和判断点是否在椭圆内。
     */
    QPainterPath createPath() const;
};

#endif // ELLIPSE_H