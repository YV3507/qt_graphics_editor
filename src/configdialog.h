#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QColor>

namespace Ui {
class ConfigDialog;
}

/**
 * @file configdialog.h
 * @brief 配置对话框类的头文件
 * 
 * 这个文件定义了ConfigDialog类，用于设置图形的属性，如颜色、线宽、填充等。
 * 配置对话框提供了一个用户友好的界面，允许用户自定义图形的外观和编辑器的行为。
 */

/**
 * @class ConfigDialog
 * @brief 配置对话框类
 * 
 * 用于设置图形的属性，如颜色、线宽、填充等。
 * 这个对话框是模态的，用户可以在其中修改各种参数，然后应用到当前选中的图形或新创建的图形上。
 */
class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief ConfigDialog类的构造函数
     * @param parent 父窗口
     * 
     * 初始化配置对话框，设置默认值和信号槽连接。
     */
    explicit ConfigDialog(QWidget *parent = nullptr);
    
    /**
     * @brief ConfigDialog类的析构函数
     * 
     * 清理资源，释放UI对象。
     */
    ~ConfigDialog() override;

    /**
     * @brief 设置线条颜色
     * @param color 线条颜色
     */
    void setColor(const QColor &color);
    
    /**
     * @brief 获取线条颜色
     * @return 当前线条颜色
     */
    QColor getColor() const;

    /**
     * @brief 设置线条宽度
     * @param width 线条宽度
     */
    void setLineWidth(int width);
    
    /**
     * @brief 获取线条宽度
     * @return 当前线条宽度
     */
    int getLineWidth() const;

    /**
     * @brief 设置最大撤销步数
     * @param steps 最大撤销步数
     */
    void setMaxUndoSteps(int steps);
    
    /**
     * @brief 获取最大撤销步数
     * @return 当前最大撤销步数
     */
    int getMaxUndoSteps() const;

    /**
     * @brief 设置是否填充
     * @param filled 是否填充
     */
    void setFilled(bool filled);
    
    /**
     * @brief 获取是否填充
     * @return 当前是否填充
     */
    bool isFilled() const;

    /**
     * @brief 设置填充颜色
     * @param color 填充颜色
     */
    void setFillColor(const QColor &color);
    
    /**
     * @brief 获取填充颜色
     * @return 当前填充颜色
     */
    QColor getFillColor() const;

private slots:
    /**
     * @brief 线条颜色按钮点击槽函数
     * 
     * 打开颜色选择对话框，让用户选择线条颜色。
     */
    void on_colorButton_clicked();
    
    /**
     * @brief 填充颜色按钮点击槽函数
     * 
     * 打开颜色选择对话框，让用户选择填充颜色。
     */
    void on_fillColorButton_clicked();

private:
    Ui::ConfigDialog *ui;        ///< UI对象，由Qt Designer生成
    QColor m_color;              ///< 当前线条颜色
    QColor m_fillColor;          ///< 当前填充颜色
    int m_maxUndoSteps;          ///< 最大撤销步数
};

#endif // CONFIGDIALOG_H