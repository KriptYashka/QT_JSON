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

void MainWindow::on_btnShowJson_clicked(){
    ui->colView->setModel(viewModel);
}

QStandardItem* convert(GroupItem item){
    QStandardItem* standardItem = new QStandardItem(QString::fromStdString(item.getName()));
    for (int childIndex = 0; childIndex < item.getChildrenCount(); ++childIndex){
        QStandardItem* standardChild = convert(item.getChild(childIndex));
        standardItem->appendRow(standardChild);
    }
    return standardItem;
}

void MainWindow::on_btnReadJson_clicked(){
    //QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"));
    string path = "E:/Git/KriptYashka/build-QT_JSON-Desktop_Qt_6_0_1_MinGW_64_bit-Debug/test.json";
    //JsonData jsondata = JsonData(filePath.toStdString());
    JsonData jsondata = JsonData(path);
    vector<GroupItem> rootGroupItems = jsondata.getData();
    viewModel->clear();
    for (int groupnum = 0; groupnum < jsondata.rootSize() ; ++groupnum){
        GroupItem rootItem = jsondata.at(groupnum);
        QStandardItem *rootGroup = convert(rootItem);
        viewModel->appendRow(rootGroup);
    }
}
