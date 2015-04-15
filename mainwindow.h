#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QMap>
class QAction;
class QSettings;
class QStandardItemModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_NodeListUrl_clicked();

    void on_pushButton_Ok_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Default_clicked();


private slots:
    void pushButtonDeleteAction();

    void on_tableView_NodeList_customContextMenuRequested(const QPoint &pos);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_ERPPassword_stateChanged(int arg1);

private:
    void initWidget();
    void getInitConfig();
    void setInitConfig();
    void initWindows();
    int getIndex();
    //void contextMenuEvent(QContextMenuEvent *e);

private:
    QSettings *configQSettings;
    QStandardItemModel *model;

    //控制台
    bool hide;

    //服务器配置
    bool log_request;
    int num_io_threads;
    int num_min_worker_threads;
    int num_max_worker_threads;
    int max_cache_num;
    int back_queue_size;

    int box_listen_port;
    int erp_listen_port;
    int app_listen_port;
    int ini_listen_port;
    int tel_listen_port;
    int upload_listen_port;
    int app_safe_port;
    int box_safe_port;
    int erp_safe_port;
    int ini_safe_port;

    QString use_safe_app;
    QString use_safe_erp;
    QString use_safe_ini;
    QString use_safe_box;
    QString log_file;

    //数据库信息
    int port;
    QString hostname;
    QString username;
    QString password;
    QString database;

    int erp_port;
    QString erp_hostname;
    QString erp_username;
    QString erp_password;
    QString erp_database;

    //机顶盒信息配置项
    QString apk_path;
    QString version;
    QString video_url;
    QString ip;
    QString romVersion;
    QString romUrl;
    QString tvPlayUrl;
    QString wineServiceUrl;
    bool validate;

    //本地设置，经纬度可用于打车服务
    QString jingdu;
    QString weidu;
    QString address;
    int sid;

    //
    QStringList nodelist;
    QMap<int, QString> tempMap;
    QAction  *deleteAction;
    QMenu    *popMenu;

private:
    Ui::MainWindow *ui;

    QListWidget *leftList;
    QStackedWidget *stack;
};

#endif // MAINWINDOW_H
