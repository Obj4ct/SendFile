#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void StartConnect(unsigned short,QString);
    void SendFile(QString path);
private slots:
    void on_btn_connect_clicked();

    void on_btn_selectFile_clicked();

    void on_btn_sendFile_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
