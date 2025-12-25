#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shape.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_drawingArea(nullptr),
    m_configDialog(nullptr),
    m_currentFilePath()
{
    ui->setupUi(this);

    //创建绘图区域
    m_drawingArea = new DrawingArea(this);
    setCentralWidget(m_drawingArea);

    // 创建配置对话框
    m_configDialog = new ConfigDialog(this);

    // 设置工具栏和状态栏
    setupActions();
    //setupConnections();
    updateToolButtons();
    updateStatusBar();
    updateUndoRedoActions();

    // 设置窗口标题和大小
    setWindowTitle("Qt图形编辑器");
    resize(800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_configDialog;
}

void MainWindow::setupActions()
{
    // 设置工具栏按钮样式
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    // 初始化线宽下拉框
    //ui->lineWidthComboBox->addItems({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});
    //ui->lineWidthComboBox->setCurrentIndex(1); // 默认线宽2

    // 设置颜色工具按钮的初始颜色
    QColor initialColor = Qt::black;
    QString style = QString("background-color: %1").arg(initialColor.name());
    //ui->colorToolButton->setStyleSheet(style);

    // 设置填充颜色工具按钮的初始颜色
    QColor initialFillColor = Qt::white;
    style = QString("background-color: %1").arg(initialFillColor.name());
    //ui->fillColorToolButton->setStyleSheet(style);
}

void MainWindow::updateStatusBar()
{
    QString status = "就绪";
    QList<Shape *> selectedShapes = m_drawingArea->selectedShapes();
    if (!selectedShapes.isEmpty()) {
        status = QString("已选择 %1 个图形").arg(selectedShapes.size());
    }
    statusBar()->showMessage(status);
}

void MainWindow::updateToolButtons()
{
    // 重置所有工具按钮
    //ui->ellipseToolButton->setChecked(false);
    //ui->rectangleToolButton->setChecked(false);
    //->selectToolButton->setChecked(false);
    //ui->moveToolButton->setChecked(false);
    //ui->resizeToolButton->setChecked(false);

    // 根据当前编辑模式设置相应的按钮
    DrawingArea::EditMode mode = DrawingArea::Draw; // 假设默认是Draw模式
    Shape::ShapeType shapeType = m_drawingArea->getCurrentShapeType();

    switch (mode) {
    case DrawingArea::Draw:
        switch (shapeType) {
        case Shape::Ellipse:
            //ui->ellipseToolButton->setChecked(true);
            break;
        case Shape::Rectangle:
            //ui->rectangleToolButton->setChecked(true);
            break;
        default:
            break;
        }
        break;
    case DrawingArea::Select:
        //ui->selectToolButton->setChecked(true);
        break;
    case DrawingArea::Move:
        //ui->moveToolButton->setChecked(true);
        break;
    case DrawingArea::Resize:
        //ui->resizeToolButton->setChecked(true);
        break;
    }
}

void MainWindow::applyConfiguration()
{
    m_drawingArea->setCurrentColor(m_configDialog->getColor());
    m_drawingArea->setCurrentLineWidth(m_configDialog->getLineWidth());
    m_drawingArea->setCurrentFilled(m_configDialog->isFilled());
    m_drawingArea->setCurrentFillColor(m_configDialog->getFillColor());

    // 设置最大撤销步数
    m_drawingArea->setMaxUndoSteps(m_configDialog->getMaxUndoSteps());

    // 更新工具按钮的显示
    QString style = QString("background-color: %1").arg(m_configDialog->getColor().name());
    //ui->colorToolButton->setStyleSheet(style);

    style = QString("background-color: %1").arg(m_configDialog->getFillColor().name());
    //ui->fillColorToolButton->setStyleSheet(style);

    //ui->lineWidthComboBox->setCurrentText(QString::number(m_configDialog->getLineWidth()));
    //ui->filledToolButton->setChecked(m_configDialog->isFilled());
}

// 文件操作槽函数
void MainWindow::on_actionNew_triggered()
{
    if (QMessageBox::question(this, "新建", "是否要创建新的绘图？当前未保存的内容将丢失。") == QMessageBox::Yes) {
        m_drawingArea->clearAll();
        m_currentFilePath.clear();
        setWindowTitle("Qt图形编辑器 - 未命名");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", "", "图形文件 (*.txt);;所有文件 (*.*)");
    if (!filename.isEmpty()) {
        if (m_drawingArea->loadFromFile(filename)) {
            m_currentFilePath = filename;
            setWindowTitle(QString("Qt图形编辑器 - %1").arg(filename));
            updateStatusBar();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (m_currentFilePath.isEmpty()) {
        on_actionSave_As_triggered();
    } else {
        if (m_drawingArea->saveToFile(m_currentFilePath)) {
            statusBar()->showMessage("文件已保存", 2000);
        }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", "", "图形文件 (*.txt);;所有文件 (*.*)");
    if (!filename.isEmpty()) {
        if (m_drawingArea->saveToFile(filename)) {
            m_currentFilePath = filename;
            setWindowTitle(QString("Qt图形编辑器 - %1").arg(filename));
            statusBar()->showMessage("文件已保存", 2000);
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

// 编辑操作槽函数
void MainWindow::on_actionUndo_triggered()
{
    m_drawingArea->undo();
    updateUndoRedoActions();
}

void MainWindow::on_actionRedo_triggered()
{
    m_drawingArea->redo();
    updateUndoRedoActions();
}

void MainWindow::on_actionCut_triggered()
{
    // 暂未实现剪切功能
    statusBar()->showMessage("剪切功能暂未实现", 2000);
}

void MainWindow::on_actionCopy_triggered()
{
    // 暂未实现复制功能
    statusBar()->showMessage("复制功能暂未实现", 2000);
}

void MainWindow::on_actionPaste_triggered()
{
    // 暂未实现粘贴功能
    statusBar()->showMessage("粘贴功能暂未实现", 2000);
}

void MainWindow::on_actionDelete_triggered()
{
    m_drawingArea->deleteSelectedShapes();
    updateStatusBar();
    updateUndoRedoActions();
}

void MainWindow::on_actionSelect_All_triggered()
{
    m_drawingArea->selectAll();
    updateStatusBar();
}

void MainWindow::on_actionClear_Selection_triggered()
{
    m_drawingArea->clearSelection();
    updateStatusBar();
}

// 图形绘制槽函数
void MainWindow::on_actionEllipse_triggered()
{
    m_drawingArea->setCurrentShapeType(Shape::Ellipse);
    m_drawingArea->setEditMode(DrawingArea::Draw);
    updateToolButtons();
}

void MainWindow::on_actionRectangle_triggered()
{
    m_drawingArea->setCurrentShapeType(Shape::Rectangle);
    m_drawingArea->setEditMode(DrawingArea::Draw);
    updateToolButtons();
}

// 编辑模式槽函数
void MainWindow::on_actionSelect_triggered()
{
    m_drawingArea->setEditMode(DrawingArea::Select);
    updateToolButtons();
}

void MainWindow::on_actionMove_triggered()
{
    m_drawingArea->setEditMode(DrawingArea::Move);
    updateToolButtons();
}

void MainWindow::on_actionResize_triggered()
{
    m_drawingArea->setEditMode(DrawingArea::Resize);
    updateToolButtons();
}

// 图层操作槽函数
void MainWindow::on_actionBring_Forward_triggered()
{
    m_drawingArea->moveSelectedShapesUp();
    updateUndoRedoActions();
}

void MainWindow::on_actionSend_Backward_triggered()
{
    m_drawingArea->moveSelectedShapesDown();
    updateUndoRedoActions();
}

void MainWindow::on_actionBring_to_Front_triggered()
{
    m_drawingArea->moveSelectedShapesToTop();
    updateUndoRedoActions();
}

void MainWindow::on_actionSend_to_Back_triggered()
{
    m_drawingArea->moveSelectedShapesToBottom();
    updateUndoRedoActions();
}

// 参数配置槽函数
void MainWindow::on_actionConfigure_triggered()
{
    // 设置配置对话框的当前值
    QList<Shape *> selectedShapes = m_drawingArea->selectedShapes();
    if (!selectedShapes.isEmpty()) {
        Shape *shape = selectedShapes.first();
        m_configDialog->setColor(shape->getColor());
        m_configDialog->setLineWidth(shape->getLineWidth());
        m_configDialog->setFilled(shape->isFilled());
        m_configDialog->setFillColor(shape->getFillColor());
    } else {
        m_configDialog->setColor(m_drawingArea->getCurrentColor());
        m_configDialog->setLineWidth(2); // 默认线宽
        m_configDialog->setFilled(false); // 默认不填充
        m_configDialog->setFillColor(Qt::white); // 默认填充颜色
    }

    // 设置当前的最大撤销步数
    m_configDialog->setMaxUndoSteps(m_drawingArea->getMaxUndoSteps());

    if (m_configDialog->exec() == QDialog::Accepted) {
        applyConfiguration();
    }
}

// 工具按钮槽函数
void MainWindow::on_ellipseToolButton_clicked()
{
    on_actionEllipse_triggered();
}

void MainWindow::on_rectangleToolButton_clicked()
{
    on_actionRectangle_triggered();
}

void MainWindow::on_selectToolButton_clicked()
{
    on_actionSelect_triggered();
}

void MainWindow::on_moveToolButton_clicked()
{
    on_actionMove_triggered();
}

void MainWindow::on_resizeToolButton_clicked()
{
    on_actionResize_triggered();
}

// 颜色和线宽槽函数
void MainWindow::on_colorToolButton_clicked()
{
    QColor color = QColorDialog::getColor(m_drawingArea->getCurrentColor(), this, "选择颜色");
    if (color.isValid()) {
        m_drawingArea->setCurrentColor(color);
        QString style = QString("background-color: %1").arg(color.name());
        //ui->colorToolButton->setStyleSheet(style);
    }
}

void MainWindow::on_lineWidthComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    //int width = ui->lineWidthComboBox->currentText().toInt();
    //m_drawingArea->setCurrentLineWidth(width);
}

void MainWindow::on_filledToolButton_toggled(bool checked)
{
    m_drawingArea->setCurrentFilled(checked);
}

void MainWindow::on_fillColorToolButton_clicked()
{
    QColor color = QColorDialog::getColor(m_drawingArea->getCurrentFillColor(), this, "选择填充颜色");
    if (color.isValid()) {
        m_drawingArea->setCurrentFillColor(color);
        QString style = QString("background-color: %1").arg(color.name());
        //ui->fillColorToolButton->setStyleSheet(style);
    }
}

// 绘图区域信号响应
void MainWindow::onShapeSelected(Shape *shape)
{
    Q_UNUSED(shape);
    updateStatusBar();
}

void MainWindow::onSelectionChanged()
{
    updateStatusBar();
    updateUndoRedoActions();
}



void MainWindow::updateUndoRedoActions()
{
    ui->actionUndo->setEnabled(m_drawingArea->canUndo());
    ui->actionRedo->setEnabled(m_drawingArea->canRedo());
}
