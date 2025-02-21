#include "widget.h"
#include "ui_widget.h"
#include "ckb_mydbutil.h"

#include <QStandardItem>
// #include <QRegExp> // qt6: QT+=core5compat  <QtCore5Compat/QRegExp>
#include <QFile>
#include <QTextStream>
#include <QDir>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::widget)
{
    ui->setupUi(this);

    // init combo box's item
    std::vector<std::string> vs = ckb::MyDBUtil::get_all_conn_name();
    QStringList qsList;
    for (std::string& str : vs) {
        qsList<<QString::fromStdString(str);
    }
    ui->comboBox->addItems(qsList);
    // init combo box end


    // init plain text edit by local file's content
    // ...QT has file system...
    QFile qf(QDir::homePath() + QString::fromStdString(ckb::MyDBUtil::sql_file));
    if (!qf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qInfo()<<"cannot open file for read."<<qf.errorString();
    }
    QByteArray qba = qf.readAll();
    ui->plainTextEdit->setPlainText(QString::fromUtf8(qba));
    qf.close();
    // init plain text edit end

}

Widget::~Widget()
{
    delete ui;
}

void Widget::save_sql() {
    qDebug()<<ui->plainTextEdit->toPlainText();
    QFile qf(QDir::homePath() + QString::fromStdString(ckb::MyDBUtil::sql_file));
    if (!qf.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qInfo()<<"cannot open file for save."<<qf.errorString();
    }

    QTextStream out(&qf);
    out << ui->plainTextEdit->toPlainText();

    qf.close();
}

void Widget::db_query() {
    // QString sql = ui->plainTextEdit->textCursor();

    // connect selected lines, and ignore suffix space, empty line
    QString sql = ui->plainTextEdit->textCursor().selectedText();

    // trim ...
    // sql.replace(QRegExp("\\n"), " ");
    // sql.replace(QRegExp("\\r\\n"), " ");
    sql.replace("\r", " ");
    sql.replace("\n", " ");

    // \xE2\x80\xA9 ? how to remove it??

    std::string stdsql = sql.toStdString();
    // std::string stdconn = "conn1";
    std::string stdconn = ui->comboBox->currentText().toStdString();

    // qDebug()<<stdconn;
    // qDebug()<<sql<<"\n\n"<<stdsql;

    // std::pair<std::vector<std::string>, std::vector<std::vector<std::string>>>;
    ckb::MyDBResult vp = ckb::MyDBUtil::query_db(stdsql, stdconn);

    QStandardItemModel *mdl = new QStandardItemModel(vp.second.size() + 1, vp.first.size());
    QStandardItem *itm = nullptr;

    for (int i = 0; i < vp.first.size(); ++i) {
        itm = new QStandardItem(QString::fromStdString(vp.first[i]));
        mdl->setItem(0, i, itm);
    }

    for (int i = 0; i < vp.second.size(); ++i) { // row
        // qDebug()<<" --- "<<vp.second[i].size();
        for (int j = 0; j < vp.second[i].size(); ++j) { // cell
            // qDebug()<<i<<", "<<j<<", "<<vp.second[i][j];
            itm = new QStandardItem(QString::fromStdString(vp.second[i][j]));
            mdl->setItem(i + 1, j, itm);
        }
    }

    ui->tableView->setModel(mdl);
    // ui->tableView->setColumnWidth(0, 100);
    // ui->tableView->resizeColumnsToContents();  // auto resize by content's length for all column



    // QStandardItemModel *mdl = new QStandardItemModel();
    // QStandardItem *itm = nullptr;
    // itm = new QStandardItem(sql);
    // mdl->setItem(0, 0, itm);
    // ui->tableView->setModel(mdl);

    // // for (int r = 0; r < vf.size(); ++r) {
    // //     itm = new QStandardItem(QString::fromStdString(vf[r].path));
    // //     itm->setCheckable(true);
    // //     mdl->setItem(r, 0, itm);
    // //     mdl->setItem(r, 1, new QStandardItem(QString::number(vf[r].mb)));
    // // }

    // // ui->tableView->setColumnWidth(1, 300);   // ? no change
    // // ui->tableView->resizeColumnToContents(1);  // no change

    // // free(ui->tableView->model());  // with this free, the 2nd click search button will crash the process. why?
    // ui->tableView->setModel(mdl);
    // // ui->tableView->setModel(mdl); // original model need free?  no
    // // ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // // ui->tableView->setColumnWidth(1, 300);  // ok, so in setModel(), it will reset?
    // // ui->tableView->resizeColumnToContents(0);  // ok
    // ui->tableView->setColumnWidth(1, 50);
    // ui->tableView->setColumnWidth(0, 400);

}

