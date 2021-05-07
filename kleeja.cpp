#include "kleeja.h"
#include "ui_kleeja.h"

Kleeja::Kleeja(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Kleeja)
{
	ui->setupUi(this);
	ui->outpanel->hide();
}

Kleeja::~Kleeja()
{
	delete ui;
}

static bool AreYouOnline(){
	if(QProcess::execute("ping -n 1 1.1.1.1") == 0) return true;
	else return false;
}

void Kleeja::on_btn_upload_clicked()
{
	QString FilePath = QFileDialog::getOpenFileName(this,tr("Select File"), QDir::homePath());
	if(FilePath != NULL){
		if((QFileInfo(FilePath).size()/1024000)>=200){
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/assets/favicon-32x32.ico"));
			messageBox.setWindowTitle("Size Warning");
			messageBox.setText("Max file size 200 Mb");
			messageBox.exec();
			return;
		}
		if(!AreYouOnline()){
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/assets/favicon-32x32.ico"));
			messageBox.setWindowTitle("network");
			messageBox.setText("check your network connection and try again ...");
			messageBox.exec();
			return;
		}
		ui->progress->setValue(0);
		ui->btn_upload->setEnabled(false);
		QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
		QHttpPart SFile, a, b, c, d, e;
		SFile.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file_0\"; filename=\"" + QFileInfo(FilePath).fileName() + "\""));
		QFile *file = new QFile(FilePath);
		file->open(QIODevice::ReadOnly);
		SFile.setBodyDevice(file);
		file->setParent(multiPart);
		a.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"sess_id\""));
		a.setBody("");
		b.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"utype\""));
		b.setBody("anon");
		c.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"link_rcpt\""));
		c.setBody("");
		d.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"link_pass\""));
		d.setBody("");
		e.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"to_folder\""));
		e.setBody("");
		e.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"to_folder\""));
		e.setBody("");
		e.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"keepalive\""));
		e.setBody("1");
		multiPart->append(SFile);
		multiPart->append(a);
		multiPart->append(b);
		multiPart->append(c);
		multiPart->append(d);
		multiPart->append(e);
		QUrl url("https://www.gulfup.online/cgi-bin/upload.cgi?upload_type=file&utype=anon");
		QNetworkRequest request(url);
		QNetworkAccessManager *manager = new QNetworkAccessManager(this);
		QNetworkReply *reply = manager->post(request, multiPart);
		connect(manager, &QNetworkAccessManager::finished, this, &Kleeja::finished);
		connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(Progress(qint64, qint64)));
		multiPart->setParent(reply);
	}
}

void Kleeja::Progress(qint64 received, qint64 total){
	if(received != 0 && total != 0){
		qint64 percent;
		percent = received * 100 / total;
		ui->progress->setValue(percent);
	}
}

void Kleeja::finished(QNetworkReply *reply){
	ui->btn_upload->setEnabled(true);
	QByteArray response = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(response);
	QVariant JON =  doc.toVariant();
	QJsonArray JSon = JON.toJsonArray();
	if(JSon.size() == 1){
		if(JSon[0].toObject().value("file_status").toString() == "OK"){
			ui->outpanel->show();
			ui->download_link->setText("https://www.gulf-up.com/" + JSon[0].toObject().value("file_code").toString());
		}else{
			ui->progress->setValue(0);
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/assets/favicon-32x32.ico"));
			messageBox.setWindowTitle("Unknown Error");
			messageBox.setText("Your file has been rejected by Gulf-Up server");
			messageBox.exec();
		}
	}else{
		ui->progress->setValue(0);
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Information);
		messageBox.setWindowIcon(QIcon(":/images/assets/favicon-32x32.ico"));
		messageBox.setWindowTitle("Unknown Error");
		messageBox.setText("Your file has been rejected by Gulf-Up server");
		messageBox.exec();
	}
}

void Kleeja::on_btn_copy_clicked()
{
	ui->download_link->selectAll();
	ui->download_link->copy();
	ui->download_link->deselect();
}
