#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include "shape.h"
#include <QString>

/**
 * @file shapefactory.h
 * @brief 图形工厂类的头文件
 * 
 * 这个文件定义了ShapeFactory类，负责创建不同类型的图形对象。
 * 使用工厂模式来封装图形创建的细节，提供统一的接口。
 */

/**
 * @class ShapeFactory
 * @brief 图形工厂类
 * 
 * 提供静态方法来创建不同类型的图形对象，支持通过类型枚举、字符串数据或类型字符串和矩形区域来创建图形。
 */
class ShapeFactory
{
public:
    /**
     * @brief 通过图形类型枚举创建图形
     * @param type 图形类型枚举值
     * @return 创建的图形对象指针，如果类型不支持，返回nullptr
     */
    static Shape *createShape(Shape::ShapeType type);
    
    /**
     * @brief 通过字符串数据创建图形
     * @param data 包含图形数据的字符串，格式为"type,id,x,y,width,height,color,lineWidth,filled,fillColor"
     * @return 创建的图形对象指针，如果数据无效，返回nullptr
     */
    static Shape *createShape(const QString &data);
    
    /**
     * @brief 通过类型字符串和矩形区域创建图形
     * @param typeStr 图形类型的字符串表示
     * @param rect 图形的矩形区域
     * @return 创建的图形对象指针，如果类型不支持，返回nullptr
     */
    static Shape *createShape(const QString &typeStr, const QRectF &rect);
};

#endif // SHAPEFACTORY_H