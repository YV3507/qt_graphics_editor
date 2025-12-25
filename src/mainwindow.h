#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingarea.h"
#include "configdialog.h"



/**
 * @file mainwindow.h
 * @brief 主窗口类的头文件
 *
 * 这个文件定义了应用程序的主窗口类MainWindow，负责用户界面和交互逻辑。
 * 主窗口包含菜单栏、工具栏、状态栏和中央的绘图区域，是用户与应用程序交互的主要界面。
 */

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE



/**
 * @class MainWindow
 * @brief 应用程序的主窗口类
 *
 * 负责创建和管理应用程序的用户界面，处理用户的菜单操作、工具栏操作等。
 * 主窗口协调各个组件之间的交互，如绘图区域、配置对话框等。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow类的构造函数
     * @param parent 父窗口
     *
     * 初始化主窗口，创建绘图区域和配置对话框，设置UI和信号槽连接。
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * @brief MainWindow类的析构函数
     *
     * 清理资源，释放UI对象、绘图区域和配置对话框。
     */
    ~MainWindow();

private slots:
    // 文件操作
    /**
     * @brief 新建文件槽函数
     *
     * 创建一个新的绘图文件，清空当前的绘图区域。
     */
    void on_actionNew_triggered();

    /**
     * @brief 打开文件槽函数
     *
     * 打开一个已有的绘图文件，加载其中的图形。
     */
    void on_actionOpen_triggered();

    /**
     * @brief 保存文件槽函数
     *
     * 保存当前的绘图到文件。
     */
    void on_actionSave_triggered();

    /**
     * @brief 另存为文件槽函数
     *
     * 将当前的绘图保存为新文件。
     */
    void on_actionSave_As_triggered();

    /**
     * @brief 退出应用程序槽函数
     *
     * 关闭应用程序。
     */
    void on_actionExit_triggered();

    // 编辑操作
    /**
     * @brief 撤销操作槽函数
     *
     * 撤销上一步操作。
     */
    void on_actionUndo_triggered();

    /**
     * @brief 重做操作槽函数
     *
     * 重做下一步操作。
     */
    void on_actionRedo_triggered();

    /**
     * @brief 更新撤销/重做操作按钮状态
     *
     * 根据当前的撤销/重做栈状态，更新撤销和重做按钮的启用状态。
     */
    void updateUndoRedoActions();

    /**
     * @brief 剪切操作槽函数
     *
     * 剪切选中的图形。
     */
    void on_actionCut_triggered();

    /**
     * @brief 复制操作槽函数
     *
     * 复制选中的图形。
     */
    void on_actionCopy_triggered();

    /**
     * @brief 粘贴操作槽函数
     *
     * 粘贴剪贴板中的图形。
     */
    void on_actionPaste_triggered();

    /**
     * @brief 删除操作槽函数
     *
     * 删除选中的图形。
     */
    void on_actionDelete_triggered();

    /**
     * @brief 全选操作槽函数
     *
     * 选择所有图形。
     */
    void on_actionSelect_All_triggered();

    /**
     * @brief 清除选择槽函数
     *
     * 取消所有图形的选择状态。
     */
    void on_actionClear_Selection_triggered();

    // 图形绘制
    /**
     * @brief 椭圆工具槽函数
     *
     * 选择椭圆绘制工具。
     */
    void on_actionEllipse_triggered();

    /**
     * @brief 矩形工具槽函数
     *
     * 选择矩形绘制工具。
     */
    void on_actionRectangle_triggered();

    // 编辑模式
    /**
     * @brief 选择模式槽函数
     *
     * 切换到选择模式。
     */
    void on_actionSelect_triggered();

    /**
     * @brief 移动模式槽函数
     *
     * 切换到移动模式。
     */
    void on_actionMove_triggered();

    /**
     * @brief 调整大小模式槽函数
     *
     * 切换到调整大小模式。
     */
    void on_actionResize_triggered();

    // 图层操作
    /**
     * @brief 将选中图形上移一层槽函数
     *
     * 将选中的图形在图层顺序中上移一层。
     */
    void on_actionBring_Forward_triggered();

    /**
     * @brief 将选中图形下移一层槽函数
     *
     * 将选中的图形在图层顺序中下移一层。
     */
    void on_actionSend_Backward_triggered();

    /**
     * @brief 将选中图形移到顶层槽函数
     *
     * 将选中的图形移到图层顺序的最顶层。
     */
    void on_actionBring_to_Front_triggered();

    /**
     * @brief 将选中图形移到底层槽函数
     *
     * 将选中的图形移到图层顺序的最底层。
     */
    void on_actionSend_to_Back_triggered();

    // 参数配置
    /**
     * @brief 配置参数槽函数
     *
     * 打开配置对话框，让用户设置图形属性和编辑器参数。
     */
    void on_actionConfigure_triggered();

    // 工具按钮
    /**
     * @brief 椭圆工具按钮点击槽函数
     *
     * 选择椭圆绘制工具。
     */
    void on_ellipseToolButton_clicked();

    /**
     * @brief 矩形工具按钮点击槽函数
     *
     * 选择矩形绘制工具。
     */
    void on_rectangleToolButton_clicked();

    /**
     * @brief 选择工具按钮点击槽函数
     *
     * 切换到选择模式。
     */
    void on_selectToolButton_clicked();

    /**
     * @brief 移动工具按钮点击槽函数
     *
     * 切换到移动模式。
     */
    void on_moveToolButton_clicked();

    /**
     * @brief 调整大小工具按钮点击槽函数
     *
     * 切换到调整大小模式。
     */
    void on_resizeToolButton_clicked();

    // 颜色和线宽
    /**
     * @brief 颜色工具按钮点击槽函数
     *
     * 打开颜色选择对话框，让用户选择线条颜色。
     */
    void on_colorToolButton_clicked();

    /**
     * @brief 线宽下拉框当前索引改变槽函数
     * @param index 当前索引
     *
     * 当线宽下拉框的当前索引改变时，更新当前线宽。
     */
    void on_lineWidthComboBox_currentIndexChanged(int index);

    /**
     * @brief 填充工具按钮状态改变槽函数
     * @param checked 是否选中
     *
     * 当填充工具按钮的状态改变时，更新当前填充状态。
     */
    void on_filledToolButton_toggled(bool checked);

    /**
     * @brief 填充颜色工具按钮点击槽函数
     *
     * 打开颜色选择对话框，让用户选择填充颜色。
     */
    void on_fillColorToolButton_clicked();

    // 绘图区域信号响应
    /**
     * @brief 图形选中信号响应槽函数
     * @param shape 被选中的图形
     *
     * 当图形被选中时，更新状态栏和UI状态。
     */
    void onShapeSelected(Shape *shape);

    /**
     * @brief 选择改变信号响应槽函数
     *
     * 当选择的图形改变时，更新状态栏、UI状态和撤销/重做按钮状态。
     */
    void onSelectionChanged();

private:
    Ui::MainWindow *ui;              ///< UI对象，由Qt Designer生成
    DrawingArea *m_drawingArea;      ///< 绘图区域
    ConfigDialog *m_configDialog;    ///< 配置对话框
    QString m_currentFilePath;       ///< 当前文件路径

    /**
     * @brief 设置动作
     *
     * 初始化和配置各种动作（Action）的属性。
     */
    void setupActions();

    /**
     * @brief 设置信号槽连接
     *
     * 连接各种信号和槽，建立组件之间的通信。
     */
    void setupConnections();

    /**
     * @brief 更新状态栏
     *
     * 根据当前的选择状态和编辑模式，更新状态栏的显示内容。
     */
    void updateStatusBar();

    /**
     * @brief 更新工具按钮
     *
     * 根据当前的编辑模式和图形类型，更新工具按钮的选中状态。
     */
    void updateToolButtons();

    /**
     * @brief 应用配置
     *
     * 将配置对话框中的设置应用到绘图区域和当前选中的图形。
     */
    void applyConfiguration();
};
#endif // MAINWINDOW_H
