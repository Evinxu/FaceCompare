#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QString>

//compare
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

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

    QString filePath;

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

namespace Ui {
class mainwidget;
}

class mainwidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainwidget(QWidget *parent = 0);
    ~mainwidget();

    float confidence;

    QImage newimg;

    static QString IUP;

protected:
    void paintEvent(QPaintEvent *event);

private slots:

    void LoadSrcSlot();

    void StartSlot();

    void QuitSlot();

    void GetIpSlot();

    void OpenVideoSlot();

    void RotateSlot();

    void finished(QNetworkReply *reply);

signals:
    void IupSignal(QString iupstr);

private:
    QHttpPart datatoHttpPart(QByteArray body, QVariant value);

    Ui::mainwidget *ui;
    QPixmap pixmap;

    QNetworkAccessManager *m_manager;
    showPicWidget *m_left;
    showPicWidget *m_right;

};

#endif // MAINWIDGET_H
