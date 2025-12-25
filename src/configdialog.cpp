#include "configdialog.h"
#include "ui_configdialog.h"
#include <QColorDialog>

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog),
    m_color(Qt::black),
    m_fillColor(Qt::white),
    m_maxUndoSteps(50)
{
    ui->setupUi(this);

    // 设置默认值
    ui->lineWidthSpinBox->setRange(1, 20);
    ui->lineWidthSpinBox->setValue(2);
    
    // 设置最大撤销步数的默认值
    ui->maxUndoStepsSpinBox->setRange(1, 1000);
    ui->maxUndoStepsSpinBox->setValue(50);

    // 更新颜色按钮显示
    on_colorButton_clicked();
    on_fillColorButton_clicked();
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::setColor(const QColor &color)
{
    m_color = color;
    QString style = QString("background-color: %1").arg(color.name());
    ui->colorButton->setStyleSheet(style);
}

QColor ConfigDialog::getColor() const
{
    return m_color;
}

void ConfigDialog::setLineWidth(int width)
{
    ui->lineWidthSpinBox->setValue(width);
}

int ConfigDialog::getLineWidth() const
{
    return ui->lineWidthSpinBox->value();
}

// 最大撤销步数设置
void ConfigDialog::setMaxUndoSteps(int steps)
{
    m_maxUndoSteps = steps;
    ui->maxUndoStepsSpinBox->setValue(steps);
}

int ConfigDialog::getMaxUndoSteps() const
{
    return ui->maxUndoStepsSpinBox->value();
}

void ConfigDialog::setFilled(bool filled)
{
    ui->filledCheckBox->setChecked(filled);
}

bool ConfigDialog::isFilled() const
{
    return ui->filledCheckBox->isChecked();
}

void ConfigDialog::setFillColor(const QColor &color)
{
    m_fillColor = color;
    QString style = QString("background-color: %1").arg(color.name());
    ui->fillColorButton->setStyleSheet(style);
}

QColor ConfigDialog::getFillColor() const
{
    return m_fillColor;
}

void ConfigDialog::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(m_color, this, "选择颜色");
    if (color.isValid()) {
        m_color = color;
        QString style = QString("background-color: %1").arg(color.name());
        ui->colorButton->setStyleSheet(style);
    }
}

void ConfigDialog::on_fillColorButton_clicked()
{
    QColor color = QColorDialog::getColor(m_fillColor, this, "选择填充颜色");
    if (color.isValid()) {
        m_fillColor = color;
        QString style = QString("background-color: %1").arg(color.name());
        ui->fillColorButton->setStyleSheet(style);
    }
}