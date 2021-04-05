/* FAU201 GUI - Version 2.0 for Debian Linux
   Copyright (c) 2018-2019 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include "about.h"
#include "mainwindow.h"
#include "serial.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonClear->setFocus();
    filepath_ = QDir::homePath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    About about;
    about.exec();
}

void MainWindow::on_actionLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Load Settings from File"), filepath_, tr("XML files (*.xml);;All files (*)"));
    if (!filename.isEmpty())  // Note that the previous dialog will return an empty string if the user cancels it
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox errorRead;
            errorRead.critical(this, tr("Error"), tr("Could not read from %1.\n\nPlease verify that you have read access to this file.").arg(QDir::toNativeSeparators(filename)));
        }
        else
        {
            QDomDocument document;
            bool error = !document.setContent(&file);
            file.close();
            if (error)
            {
                QMessageBox errorInvalid;
                errorInvalid.critical(this, tr("Error"), tr("Invalid XML. The file might be corrupted or incomplete."));
            }
            else
            {
                QDomElement root = document.firstChildElement();
                if (root.tagName() != "settings" || root.attribute("target") != "FAU200")
                {
                    QMessageBox errorNoSettings;
                    errorNoSettings.critical(this, tr("Error"), tr("The selected file is not a FAU200 settings file."));
                }
                else
                {
                    int errorcount = implementSettings(root);
                    if (errorcount > 0)
                    {
                        QMessageBox warningDebug;
                        warningDebug.warning(this, tr("Warning"), tr("Found %1 setting(s) with invalid attribute values.\n\nPlease check your settings file for out-of-bounds or unsupported attribute values.").arg(errorcount));
                    }
                    filepath_ = filename;
                }
            }
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Settings to File"), filepath_, tr("XML files (*.xml);;All files (*)"));
    if (!filename.isEmpty())  // Note that the previous dialog will return an empty string if the user cancels it
    {
        QDomDocument document;
        QDomProcessingInstruction instruction = document.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
        document.appendChild(instruction);
        QDomElement root = document.createElement("settings");
        root.setAttribute("target", "FAU200");
        document.appendChild(root);
        QDomElement element = document.createElement("setting");
        element.setAttribute("value", QString::number(ui->doubleSpinBoxVolt->value()));
        element.setAttribute("name", "voltage");
        root.appendChild(element);
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox errorWrite;
            errorWrite.critical(this, tr("Error"), tr("Could not write to %1.\n\nPlease verify that you have write access to this file.").arg(QDir::toNativeSeparators(filename)));
        }
        else
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << document.toString();
            file.close();
            filepath_ = filename;
        }
    }
}

void MainWindow::on_actionSerial_triggered()
{
    Serial serial;
    serial.setSerialLineEditText(serialstr_);
    if (serial.exec() == QDialog::Accepted)
    {
        serialstr_ = serial.serialLineEditText();
        if (serialstr_.isEmpty())
            this->setWindowTitle("FAU201 Power Supply");
        else
            this->setWindowTitle("FAU201 Power Supply (S/N: " + serialstr_ + ")");
    }
}

void MainWindow::on_doubleSpinBoxVolt_valueChanged(const QString &arg)
{
    if (ui->actionAutoApply->isChecked())
        execute(("fau201-volt " + arg + " " + serialstr_).simplified());
}

void MainWindow::on_pushButtonApply_clicked()
{
    QString volt = QString::number(ui->doubleSpinBoxVolt->value());
    execute(("fau201-volt " + volt + " " + serialstr_).simplified());
}

void MainWindow::on_pushButtonClear_clicked()
{
    execute(("fau201-clear " + serialstr_).simplified());
}

void MainWindow::on_pushButtonReset_clicked()
{
    if (ui->actionResetClear->isChecked())
        execute(("fau201-reset " + serialstr_ + " && sleep 2 && fau201-clear " + serialstr_).simplified());
    else
        execute(("fau201-reset " + serialstr_).simplified());
}

int MainWindow::implementSettings(const QDomElement &parent)
{
    int errcnt = 0;
    QDomNodeList settings = parent.elementsByTagName("setting");
    int cnt = settings.count();
    for (int i = 0; i < cnt; ++i)
    {
        QDomNode node = settings.item(i);
        if (node.isElement())
        {
            QDomElement element = node.toElement();
            if(element.attribute("name").toLower() == "voltage")
            {
                bool ok;
                double value = element.attribute("value").toDouble(&ok);
                if (ok && value >= ui->doubleSpinBoxVolt->minimum() && value <= ui->doubleSpinBoxVolt->maximum())
                    ui->doubleSpinBoxVolt->setValue(value);
                else
                    ++errcnt;
            }
        }
    }
    return errcnt;
}

void MainWindow::execute(const QString &command)
{
    QProcess sh;
    ui->textBrowserCommand->append("$ " + command);
    sh.setProcessChannelMode(QProcess::MergedChannels);
    sh.start("sh", QStringList() << "-c" << command);
    sh.waitForFinished();
    QString result = sh.readAll();
    result.chop(1);
    ui->textBrowserCommand->append(result);
    sh.close();
}
