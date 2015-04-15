#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QList>
#include <QAction>
#include <QSettings>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QDebug>


#define NODELISTMAX 20

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initWidget();
    configQSettings = new QSettings("KTVServer.conf", QSettings::IniFormat);
    ///读取配置文件
    getInitConfig();
    ///初始化窗口
    initWindows();

    ui->tableView_NodeList->setContextMenuPolicy(Qt::CustomContextMenu);
    popMenu = new QMenu(ui->tableView_NodeList);
    deleteAction = new QAction("删除", this);
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(pushButtonDeleteAction()));

    ui->listWidget->setBackgroundRole( QPalette::NoRole);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)),
            ui->stackedWidget, SLOT(setCurrentIndex(int)));

    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWidget()
{
    QRegExp regx("[0-9]+$");
    QValidator *validate = new QRegExpValidator(regx, this);
    ui->lineEdit_ListenPort->setValidator(validate);
    ui->lineEdit_ERPPort->setValidator(validate);
    ui->lineEdit_AppPort->setValidator(validate);
    ui->lineEdit_IniPort->setValidator(validate);
    ui->lineEdit_TelNetPort->setValidator(validate);
    ui->lineEdit_UploadListenPort->setValidator(validate);
    ui->lineEdit_SSLAppPort->setValidator(validate);
    ui->lineEdit_SSLBoxPort->setValidator(validate);
    ui->lineEdit_SSLErpPort->setValidator(validate);
    ui->lineEdit_SSLIniPort->setValidator(validate);

    ui->lineEdit_Port->setValidator(validate);
    ui->lineEdit_ERPPort->setValidator(validate);

    //QRegExp regDou("^(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
    //QRegExp regDou("^(-?\d+)(\.\d+)?$");
    //QValidator *valiDou = new QRegExpValidator(regDou, this);
    //ui->lineEdit_JingDu->setValidator(valiDou);
    //ui->lineEdit_WeiDU->setValidator(valiDou);

    ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_ERPPassword->setEchoMode(QLineEdit::Password);

    ui->pushButton_NodeListUrl->setFocus();
    ui->pushButton_NodeListUrl->setShortcut(QKeySequence::InsertParagraphSeparator);

}

void MainWindow::getInitConfig()
{
    /*控制台*/
    hide = configQSettings->value("/console/hide", 0).toBool();

    /*服务器配置*/
    log_request = configQSettings->value("/server/log_request", 0).toBool();
    num_io_threads = configQSettings->value("/server/num_io_threads", 10).toInt();
    num_min_worker_threads = configQSettings->value("/server/num_min_worker_threads", 20).toInt();
    num_max_worker_threads = configQSettings->value("/server/num_max_worker_threads", 20).toInt();
    max_cache_num = configQSettings->value("/server/max_cache_num", 100).toInt();
    back_queue_size = configQSettings->value("/server/back_queue_size", 5).toInt();

    box_listen_port = configQSettings->value("/server/box_listen_port", 58849).toInt();
    erp_listen_port = configQSettings->value("/server/erp_listen_port", 25377).toInt();
    app_listen_port = configQSettings->value("/server/app_listen_port", 3050).toInt();
    ini_listen_port = configQSettings->value("/server/ini_listen_port", 11235).toInt();
    tel_listen_port = configQSettings->value("/server/tel_listen_port", 2500).toInt();
    upload_listen_port = configQSettings->value("/server/upload_listen_port", 45623).toInt();
    app_safe_port = configQSettings->value("/server/app_safe_port", 3051).toInt();
    box_safe_port = configQSettings->value("/server/box_safe_port", 58850).toInt();
    erp_safe_port = configQSettings->value("/server/erp_safe_port", 25378).toInt();
    ini_safe_port = configQSettings->value("/server/ini_safe_port", 11236).toInt();

    use_safe_app = configQSettings->value("/server/use_safe_app", "NONE").toString();
    use_safe_erp = configQSettings->value("/server/use_safe_erp", "NONE").toString();
    use_safe_ini = configQSettings->value("/server/use_safe_ini", "NONE").toString();
    use_safe_box = configQSettings->value("/server/use_safe_box", "NONE").toString();
    log_file = configQSettings->value("/server/log_file", "ktvserver.log").toString();

    /*数据库信息*/
    port = configQSettings->value("/database/port", 3306).toInt();
    hostname = configQSettings->value("/database/hostname", "127.0.0.1").toString();
    username = configQSettings->value("/database/username", "yiqiding").toString();
    password = configQSettings->value("/database/password", "ktv_dev").toString();
    database = configQSettings->value("/database/database", "yiqiding_ktv").toString();

    erp_port = configQSettings->value("/database/erp_port", 3306).toInt();
    erp_hostname = configQSettings->value("/database/erp_hostname", "127.0.0.1").toString();
    erp_username = configQSettings->value("/database/erp_username", "yqc").toString();
    erp_password = configQSettings->value("/database/erp_password", "yqc2014").toString();
    erp_database = configQSettings->value("/database/erp_database", "yqcdb").toString();

    /*机顶盒信息配置项*/
    apk_path = configQSettings->value("/info/apk_path", "install/KTVBox.apk").toString();
    version = configQSettings->value("/info/version", "0").toString();
    video_url = configQSettings->value("/info/video_url", "http://192.168.1.242:8081").toString();
    ip = configQSettings->value("/info/ip", "192.168.1.242").toString();
    romVersion = configQSettings->value("/info/romVersion", "100.100.100").toString();
    romUrl = configQSettings->value("/info/romUrl", "install/update.zip").toString();
    tvPlayUrl = configQSettings->value("/info/tvPlayUrl", "rtsp://192.168.1.254/h264").toString();
    wineServiceUrl = configQSettings->value("/info/wineServiceUrl", "http://192.168.1.242:8888/").toString();
    validate = configQSettings->value("/info/validate", 0).toBool();

    /*本地设置，经纬度可用于打车服务*/
    jingdu = configQSettings->value("/local/jingdu", "121.431996").toString();
    weidu = configQSettings->value("/local/weidu", "31.194322").toString();
    address = configQSettings->value("/local/address", "XuHong Center Road No 21 , 3th").toString();
    sid = configQSettings->value("/local/sid", 0).toInt();

    /*node_list为资源服务器地址必须配置正确*/
    for(int i=0; i<NODELISTMAX; i++)
    {
        QString str("/node_list/");
        str += QString::number(i+1);
        QString val = configQSettings->value(str, "").toString();
        if(!val.isEmpty())
            tempMap.insert(i+1, val);
    }
    if (tempMap.isEmpty())
        tempMap.insert(1, "http://192.168.1.243");

}

void MainWindow::setInitConfig()
{
    QString tempStr;
    configQSettings->setValue("/console/hide", ui->checkBox_hide->isChecked());

    /*服务器配置*/
    configQSettings->setValue("/server/log_request", ui->checkBox_LogRequest->isChecked());
    configQSettings->setValue("/server/num_io_threads", ui->spinBox_IOCP->text().toInt());
    configQSettings->setValue("/server/num_min_worker_threads", ui->spinBox_Min->text().toInt());
    configQSettings->setValue("/server/num_max_worker_threads", ui->spinBox_Max->text().toInt());
    configQSettings->setValue("/server/max_cache_num", ui->spinBox_CacheNum->text().toInt());
    configQSettings->setValue("/server/back_queue_size", ui->spinBox_QueueSize->text().toInt());

    configQSettings->setValue("/server/box_listen_port", ui->lineEdit_ListenPort->text().toInt());
    configQSettings->setValue("/server/erp_listen_port", ui->lineEdit_ERPListenPort->text().toInt());
    configQSettings->setValue("/server/app_listen_port", ui->lineEdit_AppPort->text().toInt());
    configQSettings->setValue("/server/ini_listen_port", ui->lineEdit_IniPort->text().toInt());
    configQSettings->setValue("/server/tel_listen_port", ui->lineEdit_TelNetPort->text().toInt());
    configQSettings->setValue("/server/upload_listen_port", ui->lineEdit_UploadListenPort->text().toInt());

    configQSettings->setValue("/server/app_safe_port", ui->lineEdit_SSLAppPort->text().toInt());
    configQSettings->setValue("/server/box_safe_port", ui->lineEdit_SSLBoxPort->text().toInt());
    configQSettings->setValue("/server/erp_safe_port", ui->lineEdit_SSLErpPort->text().toInt());
    configQSettings->setValue("/server/ini_safe_port", ui->lineEdit_SSLIniPort->text().toInt());

    configQSettings->setValue("/server/use_safe_app", ui->comboBox_SSLAPP->currentText());
    configQSettings->setValue("/server/use_safe_erp", ui->comboBox_SSLErp->currentText());
    configQSettings->setValue("/server/use_safe_ini", ui->comboBox_SSLIni->currentText());
    configQSettings->setValue("/server/use_safe_box", ui->comboBox_SSLBox->currentText());
    configQSettings->setValue("/server/log_file", ui->lineEdit_Log->text());


    /*数据库信息*/
    configQSettings->setValue("/database/port", ui->lineEdit_Port->text().toInt());
    configQSettings->setValue("/database/hostname", ui->lineEdit_HostName->text());
    configQSettings->setValue("/database/username", ui->lineEdit_UserName->text());
    configQSettings->setValue("/database/password", ui->lineEdit_Password->text());
    configQSettings->setValue("/database/database", ui->lineEdit_Database->text());

    configQSettings->setValue("/database/erp_port", ui->lineEdit_ERPPort->text().toInt());
    configQSettings->setValue("/database/erp_hostname", ui->lineEdit_ERPHostname->text());
    configQSettings->setValue("/database/erp_username", ui->lineEdit_ERPUsername->text());
    configQSettings->setValue("/database/erp_password", ui->lineEdit_ERPPassword->text());
    configQSettings->setValue("/database/erp_database", ui->lineEdit_ERPDatabase->text());

    /*机顶盒信息配置项*/
    configQSettings->setValue("/info/apk_path", ui->lineEdit_ApkPath->text());
    configQSettings->setValue("/info/version", ui->lineEdit_ApkVersion->text());
    configQSettings->setValue("/info/video_url", ui->lineEdit_VideoUrl->text());
    configQSettings->setValue("/info/ip", ui->lineEdit_InformationIp->text());
    configQSettings->setValue("/info/romVersion", ui->lineEdit_RomVersion->text());
    configQSettings->setValue("/info/romUrl", ui->lineEdit_RomUrl->text());
    configQSettings->setValue("/info/tvPlayUrl", ui->lineEdit_TVPlayUrl->text());
    configQSettings->setValue("/info/wineServiceUrl", ui->lineEdit_WineServiceUrl->text());
    configQSettings->setValue("/info/validate", ui->checkBox_Validate->isChecked());

    /*本地设置，经纬度可用于打车服务*/
    configQSettings->setValue("/local/jingdu", ui->lineEdit_JingDu->text());
    configQSettings->setValue("/local/weidu", ui->lineEdit_WeiDU->text());
    //tempStr = ui->lineEdit_Address->text();
    configQSettings->setValue("/local/address", ui->lineEdit_Address->text());
    configQSettings->setValue("/local/sid", ui->lineEdit_KTVSid->text().toInt());

    /*node_list为资源服务器地址*/
    for(int i=0; i<model->rowCount(); i++)
    {
        QAbstractItemModel *modeSsl = ui->tableView_NodeList->model();
        QModelIndex indexTemp = modeSsl->index(i, 0);
        int index = modeSsl->data(indexTemp).toInt();

        indexTemp = modeSsl->index(i, 1);
        QString http = modeSsl->data(indexTemp).toString();

        QString str("/node_list/");
        str += QString::number(index);

        configQSettings->setValue(str, http);
    }
}

void MainWindow::initWindows()
{
    //控制台
    ui->checkBox_hide->setChecked(hide);

    /*服务器配置*/
    ui->checkBox_LogRequest->setChecked(log_request);
    ui->spinBox_IOCP->setValue(num_io_threads);
    ui->spinBox_Min->setValue(num_min_worker_threads);
    ui->spinBox_Max->setValue(num_max_worker_threads);
    ui->spinBox_CacheNum->setValue(max_cache_num);
    ui->spinBox_QueueSize->setValue(back_queue_size);

    ui->lineEdit_ListenPort->setText(QString::number(box_listen_port));
    ui->lineEdit_ERPListenPort->setText(QString::number(erp_listen_port));
    ui->lineEdit_AppPort->setText(QString::number(app_listen_port));
    ui->lineEdit_IniPort->setText(QString::number(ini_listen_port));
    ui->lineEdit_TelNetPort->setText(QString::number(tel_listen_port));
    ui->lineEdit_UploadListenPort->setText(QString::number(upload_listen_port));
    ui->lineEdit_SSLAppPort->setText(QString::number(app_safe_port));
    ui->lineEdit_SSLBoxPort->setText(QString::number(box_safe_port));
    ui->lineEdit_SSLErpPort->setText(QString::number(erp_safe_port));
    ui->lineEdit_SSLIniPort->setText(QString::number(ini_safe_port));

    ui->comboBox_SSLAPP->setCurrentText(use_safe_app);
    ui->comboBox_SSLBox->setCurrentText(use_safe_box);
    ui->comboBox_SSLErp->setCurrentText(use_safe_erp);
    ui->comboBox_SSLIni->setCurrentText(use_safe_ini);

    ui->lineEdit_Log->setText(log_file);

     /*数据库信息*/
    ui->lineEdit_Port->setText(QString::number(port));
    ui->lineEdit_HostName->setText(hostname);
    ui->lineEdit_UserName->setText(username);
    ui->lineEdit_Password->setText(password);
    ui->lineEdit_Database->setText(database);

    ui->lineEdit_ERPPort->setText(QString::number(erp_port));
    ui->lineEdit_ERPHostname->setText(erp_hostname);
    ui->lineEdit_ERPUsername->setText(erp_username);
    ui->lineEdit_ERPPassword->setText(erp_password);
    ui->lineEdit_ERPDatabase->setText(erp_database);

    /*机顶盒信息配置项*/
    ui->lineEdit_ApkPath->setText(apk_path);
    ui->lineEdit_ApkVersion->setText(version);
    ui->lineEdit_VideoUrl->setText(video_url);
    ui->lineEdit_InformationIp->setText(ip);
    ui->lineEdit_RomVersion->setText(romVersion);
    ui->lineEdit_RomUrl->setText(romUrl);
    ui->lineEdit_TVPlayUrl->setText(tvPlayUrl);
    ui->lineEdit_WineServiceUrl->setText(wineServiceUrl);

    ui->checkBox_Validate->setChecked(validate);

    /*本地设置，经纬度可用于打车服务*/
    ui->lineEdit_JingDu->setText(jingdu);
    ui->lineEdit_WeiDU->setText(weidu);
    ui->lineEdit_Address->setText(address);
    ui->lineEdit_KTVSid->setText(QString::number(sid));

    /*node_list为资源服务器地址*/
    model = new QStandardItemModel();
    model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("序号")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("地址")));
    ui->tableView_NodeList->setModel(model);   
    ui->tableView_NodeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_NodeList->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableView_NodeList->setColumnWidth(0, 90);
    ui->tableView_NodeList->setColumnWidth(1, 314);
    ui->tableView_NodeList->verticalHeader()->hide();

    //ui->tableView_NodeList->sortByColumn(0);
    model->sort(0);
    QList<int> listKey = tempMap.keys();
    for(int i=0; i<listKey.size(); i++)
    {
        int index = listKey.at(i);
        model->setItem(i, 0, new QStandardItem(QString::number(index)));
        model->setItem(i, 1, new QStandardItem(tempMap.value(index)));
    }
}

int MainWindow::getIndex()
{
    int newIndex = -1;
    int rowCount = model->rowCount();
    for(int i=0; i<NODELISTMAX; i++)
    {
        bool falg = false;
        for(int j=0; j<rowCount; j++)
        {
            QAbstractItemModel *modeSsl = ui->tableView_NodeList->model();
            QModelIndex indexTemp = modeSsl->index(j, 0);
            int index = modeSsl->data(indexTemp).toInt();

            if (i+1 == index){
                falg = true;
                break;
            }
        }

        if (!falg){
            newIndex = i+1;
            break;
        }
    }

    return newIndex;
}

void MainWindow::on_pushButton_NodeListUrl_clicked()
{
    if(ui->lineEdit_NodeListUrl->text().isEmpty())
        return;

    int newIndex = getIndex();
    if(newIndex == -1)
        return;

    int rowCount = model->rowCount();
    model->setItem(rowCount, 0, new QStandardItem(QString::number(newIndex)));
    model->setItem(rowCount, 1, new QStandardItem(ui->lineEdit_NodeListUrl->text()));
}

void MainWindow::on_pushButton_Ok_clicked()
{
    setInitConfig();

    QMessageBox::information(this, "提示", "保存成功！");
}

void MainWindow::on_pushButton_Cancel_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_Default_clicked()
{
    QFile file;
    file.remove("KTVServer.conf");

    tempMap.clear();
    getInitConfig();
    initWindows();
    setInitConfig();
}

void MainWindow::pushButtonDeleteAction()
{
    ///删除操作    
    int row = ui->tableView_NodeList->currentIndex().row();    
    model->removeRow(row);
}

void MainWindow::on_tableView_NodeList_customContextMenuRequested(const QPoint &pos)
{
    popMenu->addAction(deleteAction);
    popMenu->exec(QCursor::pos());
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1)
        ui->lineEdit_Password->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_Password->setEchoMode(QLineEdit::Password);
}

void MainWindow::on_checkBox_ERPPassword_stateChanged(int arg1)
{
    if(arg1)
        ui->lineEdit_ERPPassword->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit_ERPPassword->setEchoMode(QLineEdit::Password);
}
