#include "widget.h"
#include "ui_widget.h"
#include "ckb_myfileutil.h"

#include <QStringListModel>
#include <QStandardItem>
#include <QStandardItemModel>

// #include <iostream>
// #include <utility>
#include <algorithm>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::deleteBtn() {
    // ui->tableView->model();
    // QStandardItemModel *mdl = new QStrandardItemModel();
    QStandardItemModel *mdl = static_cast<QStandardItemModel *>(ui->tableView->model());

    QStringList deleteList;
    for (int i = 0; i < mdl->rowCount(); ++i) {
        QStandardItem *itm = mdl->item(i, 0);
        if (itm) {
            if (itm->checkState() == Qt::CheckState::Checked) {
                deleteList.append(itm->text());
            }
        } else {
            qDebug() << i << ", is nullptr";
        }
    }

    qDebug()<<"will delete "<<deleteList.size();
    for (const QString& s : deleteList) {
        qDebug()<<"delete: "<<s;
    }

    std::vector<std::string> vs(deleteList.size());
    std::transform(deleteList.begin(), deleteList.end(), vs.begin(), [](const QString& qs){
        return qs.toStdString();
    });

    ckb::MyFileUtil::deleteAll(vs);
}

// void Widget::deleteBtn() {
//     // QStandardItemModel
//     // QAbstractItemModel *mdl = ui->listView->model();

//     QStandardItemModel *mdl = static_cast<QStandardItemModel *>(ui->listView->model());
//     QStringList strList;
//     for (int i = 0; i < mdl->rowCount(); ++i) {
//         for (int j = 0; j < mdl->columnCount(); ++j) {

//             QStandardItem *item = mdl->item(i, j);
//             if (item) {
//                 qDebug() << "11 " << item->text()<<item->checkState();
//                 if (item->checkState() == Qt::CheckState::Checked) {
//                     strList.append(item->text());
//                 }
//             }
//         }
//     }

//     qDebug() << "get to delete " << strList.size();
//     for (const QString & s : strList) {
//         qDebug() << "delete " << s;
//     }

//     std::vector<std::string> vs(strList.size());
//     std::transform(strList.begin(), strList.end(), vs.begin(), [](const QString& qs){
//         return qs.toStdString();
//     });
//     ckb::MyFileUtil::deleteAll(vs);

// }


void Widget::searchBtn() {
    QString dir_path = ui->lineEdit->text();

    QStandardItemModel *mdl = new QStandardItemModel();
    const int COL = 2;
    QStringList headers;
    headers << "path" << "size(mb)";
    mdl->setHorizontalHeaderLabels(headers);

    std::vector<ckb::MyFileInfo> vf = ckb::MyFileUtil::getAllFile(dir_path.toStdString());

    QStandardItem *itm = nullptr;
    for (int r = 0; r < vf.size(); ++r) {
        itm = new QStandardItem(QString::fromStdString(vf[r].path));
        itm->setCheckable(true);
        mdl->setItem(r, 0, itm);
        mdl->setItem(r, 1, new QStandardItem(QString::number(vf[r].mb)));
    }

    // ui->tableView->setColumnWidth(1, 300);   // ? no change
    // ui->tableView->resizeColumnToContents(1);  // no change

    // free(ui->tableView->model());  // with this free, the 2nd click search button will crash the process. why?
    ui->tableView->setModel(mdl);
    // ui->tableView->setModel(mdl); // original model need free?  no
    // ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // ui->tableView->setColumnWidth(1, 300);  // ok, so in setModel(), it will reset?
    // ui->tableView->resizeColumnToContents(0);  // ok
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(0, 400);
}

// void Widget::searchBtn() {
//     QString dir_path = ui->lineEdit->text();
//     // ui->lineEdit->setText(dir_path + " i get");

//     QStandardItemModel *mdl;
//     mdl = new QStandardItemModel();  // (this) ?
//     // QStringList items;

//     qDebug() << "11111 "<<mdl->columnCount();
//     mdl->setColumnCount(2);
//     qDebug()<<"222 "<<mdl->columnCount();

//     // items << "111" << "222" << "aa";
//     std::vector<ckb::MyFileInfo> vs = ckb::MyFileUtil::getAllFile(dir_path.toStdString());

//     mdl->setRowCount(vs.size());
//     int idx = -1;
//     for (ckb::MyFileInfo& s : vs) {
//         ++idx;




//         // md, In generally, listView only one column...



//         // items.append(QString::fromStdString(s));
//         // QStandardItem *si = new QStandardItem(QString::fromStdString(s.path));
//         // si->appendColumn(new QStandardItem(QString::number(s.mb)));
//         // mdl->appendRow(si);
//         QList<QStandardItem*> itms;
//         qDebug() << s.path << "  ---  " << s.mb;
//         // itms << new QStandardItem(QString::fromStdString(s.path));
//         // itms << new QStandardItem(QString::number(s.mb));

//         // itms.append(new QStandardItem(QString::fromStdString(s.path)));
//         // itms.append(new QStandardItem(QString::number(s.mb)));

//         QStandardItem *itm1 = new QStandardItem(QString::fromStdString(s.path));
//         // itm1->setCheckable(true);
//         QStandardItem *itm2 = new QStandardItem(QString::number(s.mb));
//         // itms<<itm1<<itm2;

//         // mdl->setData(mdl->index(idx, 0), itm1);
//         // mdl->setData(mdl->index(idx, 1), itm2);

//         mdl->setItem(idx, 0, itm1);
//         mdl->setItem(idx, 1, itm2);

//         // itms.begin()->setCheckable(true);
//         //itms.at(0)->setCheckable(true);
//         // mdl->insertRow();

//         // qDebug()<<"itms's size "<<itms.size();
//         // mdl->appendRow(itms);
//     }


//     // for (const QString &item : items) {
//     //     QStandardItem *si = new QStandardItem(item);
//     //     si->setCheckable(true);
//     //     mdl->appendRow(si);
//     // }
//     ui->listView->setModel(mdl);


//     // ok
//     // QStringList strList;  // but this is not new!
//     // strList << "111" << "333";
//     // // ok
//     // QStringListModel *mdl = new QStringListModel(this);
//     // mdl->setStringList(strList);
//     // ui->listView->setModel(mdl);

//     // not appear    auto-destruction ? no move ? i think so, baidu's code are in main()
//     // QStringListModel mdl(strList);
//     // QModelIndex idx = mdl.index(0,0);
//     // mdl.setData(idx, QVariant("new ele"), Qt::EditRole);

//     // not appear
//     // QStringListModel mdl2(this);
//     // mdl2.setStringList(strList);
//     // ui->listView->setModel(&mdl2);


// }
