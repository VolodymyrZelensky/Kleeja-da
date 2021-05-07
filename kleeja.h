#ifndef KLEEJA_H
#define KLEEJA_H

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Kleeja; }
QT_END_NAMESPACE

class Kleeja : public QMainWindow
{
		Q_OBJECT

	public:
		Kleeja(QWidget *parent = nullptr);
		~Kleeja();

	private slots:
		void on_btn_upload_clicked();
		void on_btn_copy_clicked();
		void finished(QNetworkReply *reply);
		void Progress(qint64 received, qint64 total);

	private:
		Ui::Kleeja *ui;
		bool AreYouOnline();
};
#endif // KLEEJA_H
