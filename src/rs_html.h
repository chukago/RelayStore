#ifndef RS_HTML_H
#define RS_HTML_H

#include <QString>
#include <QFile>
#include <QVariant>
#include <QTextCodec>
#include <QVector>
#include <QDateTime>

class rs_html
{
	private:
		QFile *_htmlFile;
		QString gen_table_header(int cols);
		QString _font;
	public:
		rs_html(QString FileName, QString font);
		void begin(void);
		void end(void);
		void write_table(QVector<QVector<QString> > *table, QVector<int> colSizes);
};

#endif /*RS_HTML_H*/
