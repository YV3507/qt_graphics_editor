#include "drawingarea.h"
#include "shapefactory.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QPainterPath>
#include <algorithm>

DrawingArea::DrawingArea(QWidget *parent)
    : QWidget(parent),
      m_currentShapeType(Shape::Ellipse),
      m_editMode(Draw),
      m_currentColor(Qt::black),
      m_currentLineWidth(2),
      m_currentFilled(false),
      m_currentFillColor(Qt::white),
      m_tempShape(nullptr),
      m_isDrawing(false),
      m_isMoving(false),
      m_isResizing(false),
      m_resizeHandle(-1),
      m_maxUndoSteps(50)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setMouseTracking(true);
}

DrawingArea::~DrawingArea()
{
    clearAll();
}

void DrawingArea::setCurrentShapeType(Shape::ShapeType type)
{
    m_currentShapeType = type;
    m_editMode = Draw;
}

Shape::ShapeType DrawingArea::getCurrentShapeType() const
{
    return m_currentShapeType;
}

void DrawingArea::setEditMode(EditMode mode)
{
    m_editMode = mode;
}

DrawingArea::EditMode DrawingArea::getEditMode() const
{
    return m_editMode;
}

void DrawingArea::setCurrentColor(const QColor &color)
{
    m_currentColor = color;
    updateSelectedShapeProperties();
}

QColor DrawingArea::getCurrentColor() const
{
    return m_currentColor;
}

void DrawingArea::setCurrentLineWidth(int width)
{
    m_currentLineWidth = width;
    updateSelectedShapeProperties();
}

int DrawingArea::getCurrentLineWidth() const
{
    return m_currentLineWidth;
}

void DrawingArea::setCurrentFilled(bool filled)
{
    m_currentFilled = filled;
    updateSelectedShapeProperties();
}

bool DrawingArea::getCurrentFilled() const
{
    return m_currentFilled;
}

void DrawingArea::setCurrentFillColor(const QColor &color)
{
    m_currentFillColor = color;
    updateSelectedShapeProperties();
}

QColor DrawingArea::getCurrentFillColor() const
{
    return m_currentFillColor;
}

void DrawingArea::clearAll()
{
    // 先清空撤销/重做栈并清理内存
    clearUndoRedoStacks();
    
    // 然后清理当前图形
    qDeleteAll(m_shapes);
    m_shapes.clear();
    m_selectedShapes.clear();
    delete m_tempShape;
    m_tempShape = nullptr;
    update();
    emit selectionChanged();
}

void DrawingArea::clearUndoRedoStacks()
{
    // 清理撤销栈
    for (const Operation &op : m_undoStack) {
        if (op.type == DeleteShape && op.shape && !m_shapes.contains(op.shape)) {
            delete op.shape;
        }
        if (op.oldShape) {
            delete op.oldShape;
        }
    }
    m_undoStack.clear();
    
    // 清理重做栈
    clearRedoStack();
}

bool DrawingArea::saveToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件进行保存");
        return false;
    }

    QTextStream out(&file);
    for (Shape *shape : m_shapes) {
        out << shape->save() << "\n";
    }

    file.close();
    return true;
}

bool DrawingArea::loadFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件进行读取");
        return false;
    }

    // 清空现有图形
    clearAll();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            Shape *shape = ShapeFactory::createShape(line);
            if (shape) {
                m_shapes.append(shape);
            }
        }
    }

    file.close();
    update();
    return true;
}

QList<Shape *> DrawingArea::selectedShapes() const
{
    return m_selectedShapes;
}

void DrawingArea::selectAll()
{
    clearSelection();
    m_selectedShapes = m_shapes;
    for (Shape *shape : m_selectedShapes) {
        shape->setSelected(true);
    }
    update();
    emit selectionChanged();
}

void DrawingArea::clearSelection()
{
    for (Shape *shape : m_selectedShapes) {
        shape->setSelected(false);
    }
    m_selectedShapes.clear();
    update();
    emit selectionChanged();
}

void DrawingArea::deleteSelectedShapes()
{
    for (Shape *shape : m_selectedShapes) {
        // 记录删除操作用于撤销
        Operation op;
        op.type = DeleteShape;
        op.shape = shape; // 存储原始指针用于撤销
        op.oldIndex = m_shapes.indexOf(shape);
        addOperation(op);
        
        m_shapes.removeOne(shape);
        // 注意：这里不删除shape，留待撤销操作处理
    }
    m_selectedShapes.clear(); // 确保选择列表被清空
    update();
    emit selectionChanged();
}

// 图层操作函数
void DrawingArea::moveSelectedShapesUp()
{
    if (m_selectedShapes.isEmpty()) return;

    // 对选中的图形按当前顺序进行排序（从大到小）
    QList<int> indices;
    for (Shape *shape : m_selectedShapes) {
        indices.append(m_shapes.indexOf(shape));
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());

    // 逐个移动图形
    for (int index : indices) {
        if (index < m_shapes.size() - 1) {
            // 记录图层操作用于撤销
            Operation op;
            op.type = LayerChange;
            op.shape = m_shapes[index];
            op.oldIndex = index;
            op.newIndex = index + 1;
            addOperation(op);
            
            m_shapes.swapItemsAt(index, index + 1);
        }
    }

    update();
}

void DrawingArea::moveSelectedShapesDown()
{
    if (m_selectedShapes.isEmpty()) return;

    // 对选中的图形按当前顺序进行排序（从小到大）
    QList<int> indices;
    for (Shape *shape : m_selectedShapes) {
        indices.append(m_shapes.indexOf(shape));
    }
    std::sort(indices.begin(), indices.end());

    // 逐个移动图形
    for (int index : indices) {
        if (index > 0) {
            // 记录图层操作用于撤销
            Operation op;
            op.type = LayerChange;
            op.shape = m_shapes[index];
            op.oldIndex = index;
            op.newIndex = index - 1;
            addOperation(op);
            
            m_shapes.swapItemsAt(index, index - 1);
        }
    }

    update();
}

void DrawingArea::moveSelectedShapesToTop()
{
    if (m_selectedShapes.isEmpty()) return;

    // 对选中的图形按当前顺序进行排序（从大到小）
    QList<int> indices;
    for (Shape *shape : m_selectedShapes) {
        indices.append(m_shapes.indexOf(shape));
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());

    // 逐个移动图形到顶部
    int topIndex = m_shapes.size() - 1;
    for (int index : indices) {
        if (index < topIndex) {
            // 记录图层操作用于撤销
            Operation op;
            op.type = LayerChange;
            op.shape = m_shapes[index];
            op.oldIndex = index;
            op.newIndex = topIndex;
            addOperation(op);
            
            Shape *shape = m_shapes.takeAt(index);
            m_shapes.append(shape);
            topIndex--;
        }
    }

    update();
}

void DrawingArea::moveSelectedShapesToBottom()
{
    if (m_selectedShapes.isEmpty()) return;

    // 对选中的图形按当前顺序进行排序（从小到大）
    QList<int> indices;
    for (Shape *shape : m_selectedShapes) {
        indices.append(m_shapes.indexOf(shape));
    }
    std::sort(indices.begin(), indices.end());

    // 逐个移动图形到底部
    int bottomIndex = 0;
    for (int index : indices) {
        if (index > bottomIndex) {
            // 记录图层操作用于撤销
            Operation op;
            op.type = LayerChange;
            op.shape = m_shapes[index];
            op.oldIndex = index;
            op.newIndex = bottomIndex;
            addOperation(op);
            
            Shape *shape = m_shapes.takeAt(index);
            m_shapes.insert(bottomIndex, shape);
            bottomIndex++;
        }
    }

    update();
}

void DrawingArea::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制所有图形
    for (Shape *shape : m_shapes) {
        shape->draw(&painter);
    }

    // 绘制选中图形的边框
    for (Shape *shape : m_selectedShapes) {
        if (m_shapes.contains(shape)) { // 确保图形仍然存在
            shape->drawSelected(&painter);
        }
    }

    // 绘制橡皮筋效果
    if (m_isDrawing && m_tempShape) {
        drawRubberBand(&painter);
    }
}

void DrawingArea::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();

    switch (m_editMode) {
    case Draw:
        if (event->button() == Qt::LeftButton) {
            m_isDrawing = true;
            m_startPoint = event->pos();
            m_endPoint = event->pos();
            updateTempShape();
        }
        break;

    case Select:
        if (event->button() == Qt::LeftButton) {
            if (event->modifiers() != Qt::ControlModifier) {
                clearSelection();
            }
            selectShapeAt(event->pos());
        }
        break;

    case Move:
        if (event->button() == Qt::LeftButton) {
            m_isMoving = false;
            m_moveStartPositions.clear();
            
            if (!m_selectedShapes.isEmpty()) {
                // 记录移动开始时的状态，用于撤销
                for (Shape *shape : m_selectedShapes) {
                    m_moveStartPositions[shape] = cloneShape(shape);
                }
                m_isMoving = true;
            } else {
                clearSelection();
                selectShapeAt(event->pos());
                if (!m_selectedShapes.isEmpty()) {
                    // 记录移动开始时的状态，用于撤销
                    for (Shape *shape : m_selectedShapes) {
                        m_moveStartPositions[shape] = cloneShape(shape);
                    }
                    m_isMoving = true;
                }
            }
        }
        break;

    case Resize:
        if (event->button() == Qt::LeftButton) {
            m_isResizing = false;
            m_resizeHandle = -1;
            m_resizeStartShape = nullptr;
            
            if (!m_selectedShapes.isEmpty() && m_selectedShapes.size() == 1) {
                Shape *shape = m_selectedShapes.first();
                if (shape) {
                    m_resizeHandle = getResizeHandle(event->pos(), shape);
                    if (m_resizeHandle != -1) {
                        m_isResizing = true;
                        // 记录调整大小开始时的状态，用于撤销
                        m_resizeStartShape = cloneShape(shape);
                    }
                }
            } else {
                clearSelection();
                selectShapeAt(event->pos());
                if (m_selectedShapes.size() == 1) {
                    Shape *shape = m_selectedShapes.first();
                    if (shape) {
                        m_resizeHandle = getResizeHandle(event->pos(), shape);
                        if (m_resizeHandle != -1) {
                            m_isResizing = true;
                            // 记录调整大小开始时的状态，用于撤销
                            m_resizeStartShape = cloneShape(shape);
                        }
                    }
                }
            }
        }
        break;
    }

    update();
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event)
{
    QPointF delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();

    switch (m_editMode) {
    case Draw:
        if (m_isDrawing) {
            m_endPoint = event->pos();
            updateTempShape();
            update();
        }
        break;
    case Select:
        // 选择模式下的鼠标移动不需要特殊处理
        break;
    case Move:
        if (m_isMoving && !m_selectedShapes.isEmpty()) {
            moveSelectedShapes(delta);
            update();
        }
        break;

    case Resize:
        if (m_isResizing && m_selectedShapes.size() == 1 && m_resizeHandle != -1) {
            Shape *shape = m_selectedShapes.first();
            if (shape) {
                resizeSelectedShape(event->pos());
                update();
            }
        }
        break;
    }

    // 更新鼠标光标
    if (m_editMode == Resize && !m_selectedShapes.isEmpty() && m_selectedShapes.size() == 1) {
        Shape *shape = m_selectedShapes.first();
        if (shape) {
            int handle = getResizeHandle(event->pos(), shape);
            switch (handle) {
            case 0: // 左上
            case 3: // 右下
                setCursor(Qt::SizeFDiagCursor);
                break;
            case 1: // 右上
            case 2: // 左下
                setCursor(Qt::SizeBDiagCursor);
                break;
            default:
                setCursor(Qt::ArrowCursor);
                break;
            }
        }
    } else if (m_editMode == Move && !m_selectedShapes.isEmpty()) {
        setCursor(Qt::SizeAllCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void DrawingArea::mouseReleaseEvent(QMouseEvent *event)
{
    // 处理移动操作的撤销记录
    if (m_isMoving && !m_moveStartPositions.isEmpty()) {
        for (auto it = m_moveStartPositions.constBegin(); it != m_moveStartPositions.constEnd(); ++it) {
            Shape *shape = it.key();
            Shape *oldShape = it.value();
            if (shape && oldShape) {
                Operation op;
                op.type = MoveShape;
                op.shape = shape;
                op.oldShape = oldShape;
                op.oldIndex = m_shapes.indexOf(shape);
                addOperation(op);
            }
        }
        m_moveStartPositions.clear();
    }

    // 处理调整大小操作的撤销记录
    if (m_isResizing && m_resizeStartShape && m_selectedShapes.size() == 1) {
        Shape *shape = m_selectedShapes.first();
        if (shape) {
            Operation op;
            op.type = ResizeShape;
            op.shape = shape;
            op.oldShape = m_resizeStartShape;
            addOperation(op);
        }
        m_resizeStartShape = nullptr;
    }

    // 重置移动和调整大小状态
    m_isMoving = false;
    m_isResizing = false;
    m_resizeHandle = -1;

    switch (m_editMode) {
    case Draw:
        if (m_isDrawing && event->button() == Qt::LeftButton) {
            m_isDrawing = false;
            if (m_tempShape) {
                // 确保图形有有效大小
                QRectF rect = QRectF(m_startPoint, m_endPoint).normalized();
                if (rect.width() > 1 && rect.height() > 1) {
                    // 设置图形属性
                    m_tempShape->setColor(m_currentColor);
                    m_tempShape->setLineWidth(m_currentLineWidth);
                    m_tempShape->setFilled(m_currentFilled);
                    m_tempShape->setFillColor(m_currentFillColor);

                    m_shapes.append(m_tempShape);
                    
                    // 记录添加操作用于撤销
                    Operation op;
                    op.type = AddShape;
                    op.shape = m_tempShape;
                    op.oldIndex = m_shapes.size() - 1;
                    addOperation(op);
                    
                    // 自动选择新创建的图形
                    clearSelection();
                    m_selectedShapes.append(m_tempShape);
                } else {
                    // 如果图形太小，删除它
                    delete m_tempShape;
                }
                m_tempShape = nullptr;
                update();
                emit selectionChanged();
            }
        }
        break;
    case Select:
        // 选择模式下的鼠标释放不需要特殊处理
        break;
    case Move:
        m_isMoving = false;
        break;

    case Resize:
        m_isResizing = false;
        m_resizeHandle = -1;
        break;
    }
}

void DrawingArea::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete && !m_selectedShapes.isEmpty()) {
        deleteSelectedShapes();
    } else if (event->key() == Qt::Key_Escape) {
        if (m_isDrawing) {
            m_isDrawing = false;
            delete m_tempShape;
            m_tempShape = nullptr;
            update();
        } else {
            clearSelection();
        }
    }
}

void DrawingArea::drawRubberBand(QPainter *painter)
{
    painter->save();

    QPen pen(m_currentColor, m_currentLineWidth, Qt::DashLine);
    painter->setPen(pen);

    if (m_currentFilled) {
        QBrush brush(m_currentFillColor, Qt::DiagCrossPattern);
        painter->setBrush(brush);
    } else {
        painter->setBrush(Qt::NoBrush);
    }

    switch (m_currentShapeType) {
    case Shape::Ellipse:
        painter->drawEllipse(QRectF(m_startPoint, m_endPoint).normalized());
        break;
    case Shape::Rectangle:
        painter->drawRect(QRectF(m_startPoint, m_endPoint).normalized());
        break;
    default:
        break;
    }

    painter->restore();
}

int DrawingArea::getResizeHandle(const QPointF &pos, const Shape *shape) const
{
    if (!shape) return -1;

    const QRectF &rect = shape->getBoundingRect();
    QRectF handle = QRectF(-4, -4, 8, 8);

    // 检查四个角的控制点
    handle.moveCenter(rect.topLeft());
    if (handle.contains(pos)) return 0;

    handle.moveCenter(rect.topRight());
    if (handle.contains(pos)) return 1;

    handle.moveCenter(rect.bottomLeft());
    if (handle.contains(pos)) return 2;

    handle.moveCenter(rect.bottomRight());
    if (handle.contains(pos)) return 3;

    return -1;
}

void DrawingArea::updateTempShape()
{
    delete m_tempShape;
    m_tempShape = nullptr;

    QRectF rect = QRectF(m_startPoint, m_endPoint).normalized();
    m_tempShape = ShapeFactory::createShape(m_currentShapeType);
    if (m_tempShape) {
        m_tempShape->setBoundingRect(rect);
    }
}

void DrawingArea::selectShapeAt(const QPointF &pos)
{
    // 从后往前查找，优先选择上层图形
    for (int i = m_shapes.size() - 1; i >= 0; --i) {
        Shape *shape = m_shapes[i];
        if (shape && shape->contains(pos)) {
            m_selectedShapes.append(shape);
            update();
            emit selectionChanged();
            return;
        }
    }
}

void DrawingArea::moveSelectedShapes(const QPointF &offset)
{
    if (m_selectedShapes.isEmpty()) return;
    
    for (Shape *shape : m_selectedShapes) {
        shape->move(offset);
    }
    update(); // 确保界面及时更新
}

void DrawingArea::resizeSelectedShape(const QPointF &pos)
{
    if (m_selectedShapes.size() != 1 || m_resizeHandle == -1) {
        return;
    }

    Shape *shape = m_selectedShapes.first();
    if (!shape) return;

    QRectF rect = shape->getBoundingRect();

    switch (m_resizeHandle) {
    case 0: // 左上
        rect.setTopLeft(pos);
        break;
    case 1: // 右上
        rect.setTopRight(pos);
        break;
    case 2: // 左下
        rect.setBottomLeft(pos);
        break;
    case 3: // 右下
        rect.setBottomRight(pos);
        break;
    default:
        return;
    }

    // 确保图形不会变得太小
    QRectF normalizedRect = rect.normalized();
    if (normalizedRect.width() < 10 || normalizedRect.height() < 10) {
        return; // 不允许调整到太小的尺寸
    }

    shape->resize(normalizedRect);
}

void DrawingArea::updateSelectedShapeProperties()
{
    for (Shape *shape : m_selectedShapes) {
        // 记录修改前的状态用于撤销
        Operation op;
        op.type = ModifyShape;
        op.shape = shape;
        op.oldShape = cloneShape(shape);
        op.oldIndex = m_shapes.indexOf(shape);
        addOperation(op);

        shape->setColor(m_currentColor);
        shape->setLineWidth(m_currentLineWidth);
        shape->setFilled(m_currentFilled);
        shape->setFillColor(m_currentFillColor);
    }
    update();
}

// 设置最大撤销步数
void DrawingArea::setMaxUndoSteps(int steps)
{
    if (steps > 0) {
        m_maxUndoSteps = steps;
        // 如果当前撤销栈超过新的限制，移除多余的操作
        while (m_undoStack.size() > m_maxUndoSteps) {
            Operation oldOp = m_undoStack.takeFirst();
            if (oldOp.type == DeleteShape && oldOp.shape && !m_shapes.contains(oldOp.shape)) {
                delete oldOp.shape;
            }
            if (oldOp.oldShape) {
                delete oldOp.oldShape;
            }
        }
    }
}

int DrawingArea::getMaxUndoSteps() const
{
    return m_maxUndoSteps;
}

// 撤销/重做相关方法
bool DrawingArea::canUndo() const
{
    return !m_undoStack.isEmpty();
}

bool DrawingArea::canRedo() const
{
    return !m_redoStack.isEmpty();
}

void DrawingArea::undo()
{
    if (m_undoStack.isEmpty()) return;

    Operation op = m_undoStack.takeLast();
    m_redoStack.append(op);

    // 在撤销操作前清除当前选择，避免选择状态混乱
    m_selectedShapes.clear();

    switch (op.type) {
    case AddShape:
        // 撤销添加操作：删除图形
        if (op.shape && m_shapes.contains(op.shape)) {
            m_shapes.removeOne(op.shape);
            // 延迟删除，让redo可以恢复
        }
        break;

    case DeleteShape:
        // 撤销删除操作：重新添加图形
        if (op.shape) {
            if (op.oldIndex >= 0 && op.oldIndex < m_shapes.size()) {
                m_shapes.insert(op.oldIndex, op.shape);
            } else {
                m_shapes.append(op.shape);
            }
            // 注意：撤销删除时不自动选择恢复的图形
        }
        break;

    case ModifyShape:
        // 撤销修改操作：恢复旧的属性
        if (op.shape && op.oldShape) {
            // 交换新旧图形的属性
            QColor tempColor = op.shape->getColor();
            int tempLineWidth = op.shape->getLineWidth();
            bool tempFilled = op.shape->isFilled();
            QColor tempFillColor = op.shape->getFillColor();
            QRectF tempRect = op.shape->getBoundingRect();

            op.shape->setColor(op.oldShape->getColor());
            op.shape->setLineWidth(op.oldShape->getLineWidth());
            op.shape->setFilled(op.oldShape->isFilled());
            op.shape->setFillColor(op.oldShape->getFillColor());
            op.shape->setBoundingRect(op.oldShape->getBoundingRect());

            op.oldShape->setColor(tempColor);
            op.oldShape->setLineWidth(tempLineWidth);
            op.oldShape->setFilled(tempFilled);
            op.oldShape->setFillColor(tempFillColor);
            op.oldShape->setBoundingRect(tempRect);
        }
        break;

    case MoveShape:
    case ResizeShape:
    case LayerChange:
        // 撤销移动、调整大小和图层操作：恢复到原来的位置
        if (op.shape) {
            int currentIndex = m_shapes.indexOf(op.shape);
            if (currentIndex != -1 && op.oldIndex != -1) {
                // 确保目标索引在有效范围内
                int targetIndex = qBound(0, op.oldIndex, m_shapes.size());
                if (currentIndex != targetIndex) {
                    m_shapes.move(currentIndex, targetIndex);
                }
            }
        }
        break;
    }

    update();
    emit selectionChanged();
}

void DrawingArea::redo()
{
    if (m_redoStack.isEmpty()) return;

    Operation op = m_redoStack.takeLast();
    m_undoStack.append(op);

    // 在重做操作前清除当前选择，避免选择状态混乱
    m_selectedShapes.clear();

    switch (op.type) {
    case AddShape:
        // 重做添加操作：重新添加图形
        if (op.shape) {
            if (op.oldIndex >= 0 && op.oldIndex < m_shapes.size()) {
                m_shapes.insert(op.oldIndex, op.shape);
            } else {
                m_shapes.append(op.shape);
            }
        }
        break;

    case DeleteShape:
        // 重做删除操作：从列表中移除但不删除（延迟删除）
        if (op.shape && m_shapes.contains(op.shape)) {
            m_shapes.removeOne(op.shape);
        }
        break;

    case ModifyShape:
        // 重做修改操作：恢复新的属性
        if (op.shape && op.oldShape) {
            // 再次交换新旧图形的属性
            QColor tempColor = op.shape->getColor();
            int tempLineWidth = op.shape->getLineWidth();
            bool tempFilled = op.shape->isFilled();
            QColor tempFillColor = op.shape->getFillColor();
            QRectF tempRect = op.shape->getBoundingRect();

            op.shape->setColor(op.oldShape->getColor());
            op.shape->setLineWidth(op.oldShape->getLineWidth());
            op.shape->setFilled(op.oldShape->isFilled());
            op.shape->setFillColor(op.oldShape->getFillColor());
            op.shape->setBoundingRect(op.oldShape->getBoundingRect());

            op.oldShape->setColor(tempColor);
            op.oldShape->setLineWidth(tempLineWidth);
            op.oldShape->setFilled(tempFilled);
            op.oldShape->setFillColor(tempFillColor);
            op.oldShape->setBoundingRect(tempRect);
        }
        break;

    case MoveShape:
    case ResizeShape:
    case LayerChange:
        // 重做移动、调整大小和图层操作：恢复到新的位置
        if (op.shape) {
            int currentIndex = m_shapes.indexOf(op.shape);
            if (currentIndex != -1 && op.newIndex != -1) {
                // 确保目标索引在有效范围内
                int targetIndex = qBound(0, op.newIndex, m_shapes.size());
                if (currentIndex != targetIndex) {
                    m_shapes.move(currentIndex, targetIndex);
                }
            }
        }
        break;
    }

    update();
    emit selectionChanged();
}

void DrawingArea::addOperation(const Operation &op)
{
    m_undoStack.append(op);
    if (m_undoStack.size() > m_maxUndoSteps) {
        // 移除最早的操作
        Operation oldOp = m_undoStack.takeFirst();
        if (oldOp.type == DeleteShape && oldOp.shape && !m_shapes.contains(oldOp.shape)) {
            delete oldOp.shape;
        }
        if (oldOp.oldShape) {
            delete oldOp.oldShape;
        }
    }
    clearRedoStack();
}

Shape *DrawingArea::cloneShape(Shape *original)
{
    if (!original) return nullptr;

    Shape *clone = ShapeFactory::createShape(original->getType());
    if (clone) {
        clone->setColor(original->getColor());
        clone->setLineWidth(original->getLineWidth());
        clone->setFilled(original->isFilled());
        clone->setFillColor(original->getFillColor());
        clone->setBoundingRect(original->getBoundingRect());
    }
    return clone;
}

void DrawingArea::clearRedoStack()
{
    for (const Operation &op : m_redoStack) {
        if (op.type == DeleteShape && op.shape && !m_shapes.contains(op.shape)) {
            delete op.shape;
        }
        if (op.oldShape) {
            delete op.oldShape;
        }
    }
    m_redoStack.clear();
}