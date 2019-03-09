#include "comparethread.h"
#include <QDebug>
#include <QTime>
#include <QApplication>
#include <QString>
#include "mainwidget.h"
#include <QMessageBox>

#include <QBuffer>
#include <QFile>
#include <QPicture>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <Qpainter>

compareThread::compareThread(QObject *parent)
{}

void compareThread::runCompare()
{
    qDebug()<<"compareThread is:"<<QThread::currentThread();

    m_manager = new QNetworkAccessManager(this);

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));

    QString Apikey("JcY_4jqgtDjof78Z3WPBGytvGfqsO2Xh"); /*face++网站申请*/
    QString APISecret("0A_2so6KV1mgnAp5_-FMrl6SpFVXz_gH"); /*face++网站申请*/

    //初始化消息体
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    //添加key
    QHttpPart keyPart = datatoHttpPart(Apikey.toLatin1(),
                                       QVariant("form-data; name=\"api_key\""));

    multiPart->append(keyPart);

    //添加密钥
    QHttpPart secretPart = datatoHttpPart(APISecret.toLatin1(),
                                          QVariant("form-data; name=\"api_secret\""));

    multiPart->append(secretPart);

    //添加图片1
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image_base64_1\""));

    QByteArray fileflow;
    QFile *file = new QFile(m_left->fileName);
    if(file->open(QIODevice::ReadOnly))
    {
        fileflow = file->readAll();
        imagePart.setBody(fileflow.toBase64());
    }

    file->setParent(multiPart);
    multiPart->append(imagePart);

    //添加图片2
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image_base64_2\""));

    file = new QFile(m_right->fileName);
    if(file->open(QIODevice::ReadOnly))
    {
        fileflow = file->readAll();
        imagePart.setBody(fileflow.toBase64());
    }else
    {
        qDebug()<<"open error!";
    }

    file->setParent(multiPart);
    multiPart->append(imagePart);

    //初始化请求对象
    QUrl url("https://api-cn.faceplusplus.com/facepp/v3/compare");
    QNetworkRequest request;
    request.setUrl(url);

    QNetworkReply *reply = m_manager->post(request, multiPart);
    multiPart->setParent(reply);
}

void compareThread::finished(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();

    QMap<QString,QVariant>map;
    QJsonDocument d;
    map = d.fromJson(responseData).toVariant().toMap();

    QMap<QString, QVariant>rectangleMap1;
    rectangleMap1 = map.value("faces1").toJsonArray().at(0).toObject().toVariantMap().value("face_rectangle").toMap();

    QMap<QString, QVariant>rectangleMap2;
    rectangleMap2 = map.value("faces2").toJsonArray().at(0).toObject().toVariantMap().value("face_rectangle").toMap();

    //取face1的坐标
    m_left->setm_width(rectangleMap1.value("width").toInt());
    m_left->setm_top(rectangleMap1.value("top").toInt());
    m_left->setm_left(rectangleMap1.value("left").toInt());
    m_left->setm_height(rectangleMap1.value("height").toInt());

    //取face2的坐标
    m_right->setm_width(rectangleMap2.value("width").toInt());
    m_right->setm_top(rectangleMap2.value("top").toInt());
    m_right->setm_left(rectangleMap2.value("left").toInt());
    m_right->setm_height(rectangleMap2.value("height").toInt());

    qDebug()<<"aojsdoipajsduhbasuodhoaisjdoasdoiassadadfdouhjoajda pipei";

    update();
}

QHttpPart compareThread::datatoHttpPart(QByteArray body, QVariant value)
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, value);
    part.setBody(body);

    return part;
}

leftWidget::leftWidget(QWidget *parent)
{
    fileName = "E:\\BaiduNetdiskDownload\\Face\\model\\test2.jpg";
}

void leftWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, QImage(fileName));

    if(m_width != -1)
    {
        painter.setPen(QPen(QBrush(Qt::green), 2));
        QPolygonF polygon;
        polygon << QPointF(m_left, m_top)
                << QPointF(m_left + m_width, m_top)
                << QPointF(m_left + m_width, m_top + m_height)
                << QPointF(m_left, m_top + m_height);
        painter.drawPolygon(polygon);
    }
}

rightWidget::rightWidget(QWidget *parent)
{
    fileName = "E:\\BaiduNetdiskDownload\\Face\\face_test\\source\\save\\saveROI.jpg";
}

void rightWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, QImage(fileName));

    if(m_width != -1)
    {
        painter.setPen(QPen(QBrush(Qt::green), 2));
        QPolygonF polygon;
        polygon << QPointF(m_left, m_top)
                << QPointF(m_left + m_width, m_top)
                << QPointF(m_left + m_width, m_top + m_height)
                << QPointF(m_left, m_top + m_height);
        painter.drawPolygon(polygon);
    }
}

showPicWidget::showPicWidget(QWidget *parent):
    QWidget(parent)
{
    m_width = -1;
    m_top = -1;
    m_left = -1;
    m_height = -1;
}
