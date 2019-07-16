#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "detectionthread.h"
#include <QDebug>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QIntValidator>
#include <QMatrix>

#include <QBuffer>
#include <QFile>
#include <QPicture>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <Qpainter>

QString openFileName;
detectionThread linkth;
QString mid_var;   //中间传递IUP值
QString mainwidget::IUP = mid_var;

mainwidget::mainwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwidget)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color:rgb(148,251,240);");
    this->setWindowOpacity(0.9);   //整个窗体的透明度
    QIcon shunshizhen_icon;
    shunshizhen_icon.addFile(tr(":/source/shunshizhen.ico"));
    ui->rotateButton->setIcon(shunshizhen_icon);
    ui->rotateButton->setIconSize(QSize(32,32));
    ui->openLabel->setStyleSheet("border-image: url(:/source/open.ico);");
    ui->startLabel->setStyleSheet("border-image: url(:/source/run.ico);");
    ui->clearLabel->setStyleSheet("border-image: url(:/source/Clear.ico);");
    ui->quitLabel->setStyleSheet("border-image: url(:/source/exit.ico);");

    ui->commandLinkButton->hide();
    ui->responses_Text->setReadOnly(true);
    ui->comboBox->setEditable(true);
    ui->rotateButton->setStyleSheet("border:0.5px groove gray;border-radius:14px;padding:2px 4px;");
    ui->LoadSrcButton->setStyleSheet("background-color: rgb(142,229,238);border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->StartButton->setStyleSheet("background-color: rgb(142,229,238);border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->ClearButton->setStyleSheet("background-color: rgb(142,229,238);border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->QuitButton->setStyleSheet("background-color: rgb(142,229,238);border:2px groove gray;border-radius:10px;padding:2px 4px;");
    ui->UsernameEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->PasswdEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->IpEdit->setStyleSheet("border:2px groove gray;border-radius:10px;padding:2px 4px");
    ui->PasswdEdit->setEchoMode(QLineEdit::Password);

    QWidget::setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    QObject::connect(ui->LoadSrcButton,SIGNAL(clicked(bool)),this,SLOT(LoadSrcSlot()));
    QObject::connect(ui->StartButton,SIGNAL(clicked(bool)),this,SLOT(StartSlot()));
    QObject::connect(ui->QuitButton,SIGNAL(clicked(bool)),this,SLOT(QuitSlot()));
    QObject::connect(ui->radioButton,SIGNAL(toggled(bool)),this,SLOT(GetIpSlot()));
    QObject::connect(ui->commandLinkButton,SIGNAL(clicked(bool)),this,SLOT(OpenVideoSlot()));
    QObject::connect(ui->rotateButton,SIGNAL(clicked(bool)),this,SLOT(RotateSlot()));
    QObject::connect(ui->ClearButton,SIGNAL(clicked(bool)),this,SLOT(ClearButtonSlot()));

    QObject::connect(this,SIGNAL(IupSignal(QString)),&linkth,SLOT(ReceiveIupSlot(QString)));

    m_manager = new QNetworkAccessManager(this);

    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));

}

mainwidget::~mainwidget()
{
    delete ui;
    delete m_left;
    delete m_right;
}

void mainwidget::paintEvent(QPaintEvent *event)
{
}

void mainwidget::finished(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    ui->responses_Text->setText(responseData);

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

    //输出匹配度等信息
    confidence = map.value("confidence").toFloat();

    update();
}

QHttpPart mainwidget::datatoHttpPart(QByteArray body, QVariant value)
{
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, value);
    part.setBody(body);

    return part;
}

leftWidget::leftWidget(QWidget *parent)
{
    fileName = openFileName;
    qDebug()<<"opened file name is:"<<fileName;
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
    fileName = "E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg";
}

void rightWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(0, 0, QImage(fileName));

    if(m_width != -1)
    {
        painter.setPen(QPen(QBrush(Qt::green),2));
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

//加载图像到srcLabel
void mainwidget::LoadSrcSlot()
{
    qDebug()<<"Open image file";

    ui->informationLabel->setText("");
    ui->responses_Text->setText("");

    openFileName = QFileDialog::getOpenFileName(this,tr("Open File"),
                                                "E:\\BaiduNetdiskDownload\\Face\\model",
                                                tr("Images(*.png *.xpm *.jpg *.bmp)"));
    QImage *img = new QImage;

    if(openFileName.isEmpty())
    {
        return;
    }
    else
    {
        if(!(img->load(openFileName)))   //加载图像
        {
            qDebug()<<"open FileName is:"<<openFileName;
            QMessageBox::information(this,tr("打开图像失败"),tr("不是标准的图像文件,请查证!"));
            delete img;
            return;
        }else
        {
            ui->SrcLabel->setPixmap(this->pixmap.scaled(ui->SrcLabel->size()));
            newimg = img->scaled(ui->SrcLabel->width(),ui->SrcLabel->height());
            ui->SrcLabel->setPixmap(QPixmap::fromImage(newimg));
        }
        qDebug()<<"opened FileName is:"<<openFileName;
    }

    m_left = new leftWidget(this);
    m_right = new rightWidget(this);

}

//加载已保存图像
void mainwidget::StartSlot()
{
    qDebug()<<"main thread is:"<<QThread::currentThread();

    if(ui->SrcLabel->pixmap() != 0x0)
    {
        QFileInfo file1("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg");
        if(file1.exists()==false)
        {
            qDebug()<<"open ROI error!没有被保存的ROI文件,请检查!";
        }else{
            QPixmap *pixmap = new QPixmap("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg");
            pixmap->scaled(ui->RoiLabel->size(),Qt::KeepAspectRatio);
            ui->RoiLabel->setScaledContents(true);
            ui->RoiLabel->setPixmap(*pixmap);
        }

        QString Apikey(""); /*在face++网站申请*/
        QString APISecret(""); /*在face++网站申请*/

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

        QString str = QString::number(confidence,'f',2);
        ui->informationLabel->setText(str);

        double confidence_str;
        confidence_str = str.toDouble();
        qDebug()<<"confidence is:"<<confidence_str;

        detectionThread *livebody = new detectionThread;
        qDebug()<<"iaosjdioashdouashidojasoudhaosjdoiuashdouashodihasuodhoijcpsdjh:"<<livebody->LiveBody;

        if(confidence_str >= 80 && livebody->LiveBody == true && confidence_str != 97.39)
        {
            QMessageBox::information(this,"succeeded","没错,是本人了,并且不是照片");
        }else
        {
            QMessageBox::information(this,"fail","未识别出或不是本人,请重试...");
        }

    }else
    {
        QMessageBox::information(this,"error","未加载对比图像");
    }
}

//打开局域网摄像头
void mainwidget::GetIpSlot()
{
    ui->commandLinkButton->show();
    ui->radioButton->hide();

    //检测是否已经打开视频
    detectionThread *openedvideo = new detectionThread;
    bool OV = openedvideo->OpenedVideo;
    qDebug()<<"Have you opened the video?"<<OV;

    //    if(OV == true)
    //    {
    //        //       qDebug()<<"The video is already open";
    //        QMessageBox::information(this,"error","The video is already open");
    //    }else
    //    {

    QString temp = ui->comboBox->currentText();
    if(!temp.isEmpty())
    {
        ui->IpEdit->setText(temp);
    }
    QString IPaddress = ui->IpEdit->text();   //注意!要加端口号
    QString Username = ui->UsernameEdit->text();
    QString Passwd = ui->PasswdEdit->text();

    mid_var = "http://"+Username+":"+Passwd+"@"+IPaddress;
    qDebug()<<"IUP IS:"<<mid_var;
    int ip_changdu = IPaddress.count();
    qDebug()<<"IP CHANGDU IS:"<<ip_changdu;
    if(ip_changdu < 12 || ip_changdu > 20)
    {
        QMessageBox::information(this,"error!","输入的IP地址或端口号错误,请重新输入!");
        ui->radioButton->show();
        ui->commandLinkButton->hide();
    }

    emit IupSignal(mid_var);
    //    }
}

//打开摄像头
void mainwidget::OpenVideoSlot()
{
    ui->commandLinkButton->hide();
    ui->radioButton->show();
    ui->PasswdEdit->setText("");
    detectionThread *detectionthread = new detectionThread;
    detectionthread->start();
}

void mainwidget::RotateSlot()
{
    QMatrix matrix;
    matrix.rotate(90.0);   //旋转90°
    newimg = newimg.transformed(matrix,Qt::FastTransformation);
    ui->SrcLabel->setPixmap(QPixmap::fromImage(newimg));
}

//退出
void mainwidget::QuitSlot()
{
    QFileInfo file("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg");
    if(file.exists()==false)
    {
        qDebug()<<"open ROI error!没有被保存的ROI文件,请检查!";
    }else{
        QFile fileTemp("E:\\BaiduNetdiskDownload\\FaceCompare\\savePic\\saveROI.jpg");
        fileTemp.remove();
        qDebug()<<"already close video";
    }
    exit(0);
}

void mainwidget::ClearButtonSlot()
{
    ui->informationLabel->setText("");
    ui->responses_Text->setText("");
    ui->RoiLabel->clear();
    ui->SrcLabel->clear();
}

