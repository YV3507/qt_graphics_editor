#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include <QWidget>
#include <QList>
#include <QPointF>
#include <QPainterPath>
#include "shape.h"

/**
 * @file drawingarea.h
 * @brief 绘图区域类的头文件
 * 
 * 这个文件定义了绘图区域类，负责图形的绘制和用户交互。
 * DrawingArea是应用程序的核心组件，处理用户的绘图操作、选择、移动、调整大小等交互。
 */

/**
 * @class DrawingArea
 * @brief 绘图区域类
 * 
 * 负责图形的绘制、用户交互（如选择、移动、创建图形等）。
 * 管理图形的集合、撤销/重做栈、编辑模式等。
 */
class DrawingArea : public QWidget
{
    Q_OBJECT

public:
    /**
     * @enum EditMode
     * @brief 编辑模式枚举
     * 
     * 定义了绘图区域的不同编辑模式，包括绘制、选择、移动和调整大小。
     */
    enum EditMode { 
        Draw,   ///< 绘制模式，用于创建新图形
        Select, ///< 选择模式，用于选择图形
        Move,   ///< 移动模式，用于移动图形
        Resize  ///< 调整大小模式，用于调整图形大小
    };
    
    /**
     * @enum OperationType
     * @brief 操作类型枚举
     * 
     * 定义了用于撤销/重做系统的不同操作类型。
     */
    enum OperationType { 
        AddShape,      ///< 添加图形操作
        DeleteShape,   ///< 删除图形操作
        ModifyShape,   ///< 修改图形操作
        MoveShape,     ///< 移动图形操作
        ResizeShape,   ///< 调整图形大小操作
        LayerChange    ///< 图层变更操作
    };

    /**
     * @struct Operation
     * @brief 操作结构体
     * 
     * 用于存储撤销/重做系统的操作信息。
     */
    struct Operation {
        OperationType type;    ///< 操作类型
        Shape *shape;          ///< 操作涉及的图形
        Shape *oldShape;       ///< 用于存储修改前的形状
        int oldIndex;          ///< 用于存储移动前的索引
        int newIndex;          ///< 用于存储移动后的索引
    };

    /**
     * @brief DrawingArea类的构造函数
     * @param parent 父窗口
     * 
     * 初始化绘图区域，设置默认属性和状态。
     */
    DrawingArea(QWidget *parent = nullptr);
    
    /**
     * @brief DrawingArea类的析构函数
     * 
     * 清理资源，释放所有图形对象。
     */
    ~DrawingArea() override;

    /**
     * @brief 设置当前要创建的图形类型
     * @param type 图形类型
     */
    void setCurrentShapeType(Shape::ShapeType type);
    
    /**
     * @brief 获取当前要创建的图形类型
     * @return 当前图形类型
     */
    Shape::ShapeType getCurrentShapeType() const;
    
    /**
     * @brief 设置编辑模式
     * @param mode 编辑模式
     */
    void setEditMode(EditMode mode);
    
    /**
     * @brief 获取当前编辑模式
     * @return 当前编辑模式
     */
    EditMode getEditMode() const;
    
    /**
     * @brief 设置当前颜色
     * @param color 颜色
     */
    void setCurrentColor(const QColor &color);
    
    /**
     * @brief 获取当前颜色
     * @return 当前颜色
     */
    QColor getCurrentColor() const;
    
    /**
     * @brief 设置当前线宽
     * @param width 线宽
     */
    void setCurrentLineWidth(int width);
    
    /**
     * @brief 获取当前线宽
     * @return 当前线宽
     */
    int getCurrentLineWidth() const;
    
    /**
     * @brief 设置当前是否填充
     * @param filled 是否填充
     */
    void setCurrentFilled(bool filled);
    
    /**
     * @brief 获取当前是否填充
     * @return 当前是否填充
     */
    bool getCurrentFilled() const;
    
    /**
     * @brief 设置当前填充颜色
     * @param color 填充颜色
     */
    void setCurrentFillColor(const QColor &color);
    
    /**
     * @brief 获取当前填充颜色
     * @return 当前填充颜色
     */
    QColor getCurrentFillColor() const;

    /**
     * @brief 清除所有图形
     * 
     * 删除所有图形，清空撤销/重做栈。
     */
    void clearAll();
    
    /**
     * @brief 保存图形到文件
     * @param filename 文件名
     * @return 如果保存成功，返回true，否则返回false
     */
    bool saveToFile(const QString &filename);
    
    /**
     * @brief 从文件加载图形
     * @param filename 文件名
     * @return 如果加载成功，返回true，否则返回false
     */
    bool loadFromFile(const QString &filename);

    /**
     * @brief 获取选中的图形列表
     * @return 选中的图形列表
     */
    QList<Shape *> selectedShapes() const;
    
    /**
     * @brief 选择所有图形
     */
    void selectAll();
    
    /**
     * @brief 清除选择
     */
    void clearSelection();
    
    /**
     * @brief 删除选中的图形
     */
    void deleteSelectedShapes();

    /**
     * @brief 将选中的图形上移一层
     */
    void moveSelectedShapesUp();
    
    /**
     * @brief 将选中的图形下移一层
     */
    void moveSelectedShapesDown();
    
    /**
     * @brief 将选中的图形移到顶层
     */
    void moveSelectedShapesToTop();
    
    /**
     * @brief 将选中的图形移到底层
     */
    void moveSelectedShapesToBottom();

    /**
     * @brief 判断是否可以撤销
     * @return 如果可以撤销，返回true，否则返回false
     */
    bool canUndo() const;
    
    /**
     * @brief 判断是否可以重做
     * @return 如果可以重做，返回true，否则返回false
     */
    bool canRedo() const;
    
    /**
     * @brief 撤销上一步操作
     */
    void undo();
    
    /**
     * @brief 重做下一步操作
     */
    void redo();
    
    /**
     * @brief 设置最大撤销步数
     * @param steps 最大撤销步数
     */
    void setMaxUndoSteps(int steps);
    
    /**
     * @brief 获取最大撤销步数
     * @return 最大撤销步数
     */
    int getMaxUndoSteps() const;

signals:
    /**
     * @brief 当图形被选中时发出的信号
     * @param shape 被选中的图形
     */
    void shapeSelected(Shape *shape);
    
    /**
     * @brief 当选择的图形改变时发出的信号
     */
    void selectionChanged();

protected:
    /**
     * @brief 重写绘图事件
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent *event) override;
    
    /**
     * @brief 重写鼠标按下事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 重写鼠标移动事件
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    
    /**
     * @brief 重写鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    
    /**
     * @brief 重写按键事件
     * @param event 按键事件
     */
    void keyPressEvent(QKeyEvent *event) override;

private:
    QList<Shape *> m_shapes;              ///< 图形列表
    Shape::ShapeType m_currentShapeType;  ///< 当前要创建的图形类型
    EditMode m_editMode;                  ///< 当前编辑模式
    QColor m_currentColor;                ///< 当前颜色
    int m_currentLineWidth;               ///< 当前线宽
    bool m_currentFilled;                 ///< 当前是否填充
    QColor m_currentFillColor;            ///< 当前填充颜色

    // 临时图形（橡皮筋效果）
    Shape *m_tempShape;       ///< 临时图形，用于绘制橡皮筋效果
    QPointF m_startPoint;     ///< 绘制开始点
    QPointF m_endPoint;       ///< 绘制结束点
    bool m_isDrawing;         ///< 是否正在绘制

    // 选择和编辑相关
    QList<Shape *> m_selectedShapes;    ///< 选中的图形列表
    QPointF m_lastMousePos;             ///< 上一次鼠标位置
    bool m_isMoving;                    ///< 是否正在移动
    bool m_isResizing;                  ///< 是否正在调整大小
    int m_resizeHandle;                 ///< 调整大小的控制点
    
    // 用于优化撤销机制的临时状态存储
    QHash<Shape *, Shape *> m_moveStartPositions;  ///< 移动开始时的图形状态
    Shape *m_resizeStartShape;                     ///< 调整大小开始时的图形状态

    // 撤销/重做相关
    QList<Operation> m_undoStack;    ///< 撤销栈
    QList<Operation> m_redoStack;    ///< 重做栈
    int m_maxUndoSteps;              ///< 最大撤销步数

    /**
     * @brief 绘制橡皮筋效果
     * @param painter 绘图工具
     */
    void drawRubberBand(QPainter *painter);
    
    /**
     * @brief 获取调整大小的控制点
     * @param pos 鼠标位置
     * @param shape 图形
     * @return 控制点索引，如果没有找到，返回-1
     */
    int getResizeHandle(const QPointF &pos, const Shape *shape) const;
    
    /**
     * @brief 更新临时图形
     * 
     * 根据当前的开始点和结束点更新临时图形。
     */
    void updateTempShape();
    
    /**
     * @brief 选择指定位置的图形
     * @param pos 位置
     */
    void selectShapeAt(const QPointF &pos);
    
    /**
     * @brief 移动选中的图形
     * @param offset 偏移量
     */
    void moveSelectedShapes(const QPointF &offset);
    
    /**
     * @brief 调整选中图形的大小
     * @param pos 鼠标位置
     */
    void resizeSelectedShape(const QPointF &pos);
    
    /**
     * @brief 更新选中图形的属性
     * 
     * 将当前的颜色、线宽、填充等属性应用到选中的图形。
     */
    void updateSelectedShapeProperties();

    /**
     * @brief 添加操作到撤销栈
     * @param op 操作
     */
    void addOperation(const Operation &op);
    
    /**
     * @brief 克隆图形
     * @param original 原始图形
     * @return 克隆的图形
     */
    Shape *cloneShape(Shape *original);
    
    /**
     * @brief 清空重做栈
     */
    void clearRedoStack();
    
    /**
     * @brief 清空撤销和重做栈
     */
    void clearUndoRedoStacks();
};

#endif // DRAWINGAREA_H