#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete viewModel;
}

void MainWindow::setCView(){

}

void MainWindow::on_btnShowJson_clicked(){
    for (int groupnum = 0; groupnum < 3 ; ++groupnum) {
            /* Create the phone groups as QStandardItems */
            QStandardItem *group = new QStandardItem(QString("Group %1").arg(groupnum));
            /* Append to each group 5 person as children */
            for (int personnum = 0; personnum < 5 ; ++personnum) {
                QStandardItem *child = new QStandardItem(QString("Person %1 (group %2)").arg(personnum).arg(groupnum));
                /* the appendRow function appends the child as new row */
                group->appendRow(child);
            }
            /* append group as new row to the model. model takes the ownership of the item */
            viewModel->appendRow(group);
        }
    ui->colView->setModel(viewModel);
}
