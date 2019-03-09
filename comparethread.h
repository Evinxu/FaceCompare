#ifndef COMPARETHREAD_H
#define COMPARETHREAD_H

#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include <QTimerEvent>
#include <QBasicTimer>
#include <QTimer>

#include <QWidget>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

using namespace std;

class showPicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit showPicWidget(QWidget *parent = 0);

    int getm_width(){return m_width;}
    int getm_top(){return m_top;}
    int getm_left(){return m_left;}
    int getm_height(){return m_height;}


    void setm_width(int width){m_width = width;}
    void setm_top(int top){m_top = top;}
    void setm_left(int left){m_left = left;}
    void setm_height(int height){m_height = height;}

    QString fileName;
protected:
    int m_width;
    int m_top;
    int m_left;
    int m_height;
};

class leftWidget : public showPicWidget
{
    Q_OBJECT

public:
    explicit leftWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
};

class rightWidget : public showPicWidget
{
    Q_OBJECT
public:
    explicit rightWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
};

class compareThread : public QWidget
{
    Q_OBJECT
public:
     explicit compareThread(QObject *parent = 0);
     void runCompare();

     float confidence;

private slots:
    void finished(QNetworkReply *reply);

private:
    QHttpPart datatoHttpPart(QByteArray body, QVariant value);

    QNetworkAccessManager *m_manager;
    showPicWidget *m_left;
    showPicWidget *m_right;

signals:
     void comparechange(QString);
};

#endif // COMPARETHREAD_H
