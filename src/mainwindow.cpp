/* FAU201 GUI - Version 1.0 for Debian Linux
   Copyright (c) 2018 Samuel Lourenço

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

void MainWindow::on_actionSerial_triggered()
{
    Serial serial;
    serial.setSerialLineEditText(serialstr_);
    if (serial.exec() == QDialog::Accepted)
    {
        serialstr_ = serial.serialLineEditText();
        if (serialstr_ == "")
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
