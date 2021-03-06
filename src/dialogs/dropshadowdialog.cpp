/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// Dropshadow dialog class for the PhotoFlare application.

#include "dropshadowdialog.h"
#include "ui_dropshadowdialog.h"
#include "../Settings.h"

#include <QColorDialog>
#include <QSettings>

dropshadowDialog::dropshadowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dropshadowDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->colorComboBox->width(),ui->colorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->colorComboBox->addItem(QString(), pixmap);
    }

    ui->colorComboBox->setOnClickHandler(this);

    if(SETTINGS->getMemParamsEnabled())
    {
        readSettings(this);
    }
}

dropshadowDialog::~dropshadowDialog()
{
    delete ui;
}

void dropshadowDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(e->x() < obj->width() - 20)
    {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this);
        if (selectedColor.isValid())
        {
            QPixmap pixmap(QSize(obj->width(),obj->height()));
            pixmap.fill(selectedColor);
            obj->insertItem(0, QString(), pixmap);
            obj->setCurrentIndex(0);
        }
        e->ignore();
    }
    else
    {
        e->accept();
    }
}

QColor dropshadowDialog::color() const
{
    QImage img = (QImage)ui->colorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

int dropshadowDialog::radius() const
{
    return ui->radiusValue->value();
}

int dropshadowDialog::padding() const
{
    return ui->paddingValue->value();
}

int dropshadowDialog::offsetx() const
{
    return ui->offsetxValue->value();
}

int dropshadowDialog::offsety() const
{
    return ui->offsetyValue->value();
}


QColor dropshadowDialog::colorName()
{
    QImage img = (QImage)ui->colorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return color.name();
}

void dropshadowDialog::setComboColor(QComboBox *obj, QColor c)
{
    QPixmap pixmap(QSize(obj->width(),obj->height()));
    pixmap.fill(c);
    obj->insertItem(0, QString(), pixmap);
    obj->setCurrentIndex(0);
}

void dropshadowDialog::on_buttonBox_accepted()
{
    if(SETTINGS->getMemParamsEnabled())
    {
        writeSettings(this);
    }
}

void dropshadowDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("color", colorName());
    settings.setValue("radius", radius());
    settings.setValue("padding", padding());
    settings.setValue("offsetx", offsetx());
    settings.setValue("offsety", offsety());
    settings.endGroup();
}

void dropshadowDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        setComboColor(ui->colorComboBox, settings.value("color").value<QColor>());
        ui->radiusValue->setValue(settings.value("radius").toInt());
        ui->paddingValue->setValue(settings.value("padding").toInt());
        ui->offsetxValue->setValue(settings.value("offsetx").toInt());
        ui->offsetyValue->setValue(settings.value("offsety").toInt());
    }
    settings.endGroup();
}

void dropshadowDialog::on_defaultsButton_clicked()
{
    setComboColor(ui->colorComboBox,Qt::black);
    ui->radiusValue->setValue(5);
    ui->paddingValue->setValue(15);
    ui->offsetxValue->setValue(5);
    ui->offsetyValue->setValue(5);
}
