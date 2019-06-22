#include "rs_html.h"

#define HTML_DESCRIPTION 	"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n"

rs_html::rs_html(QString FileName, QString font)
{
	_font = font;
	_htmlFile = new QFile(FileName);
	_htmlFile->open(QIODevice::WriteOnly);
}

void rs_html::begin(void)
{
	_htmlFile->write(HTML_DESCRIPTION);
	_htmlFile->write("<HTML>\n<HEAD>\n");
	_htmlFile->write("<META HTTP-EQUIV=\"CONTENT-TYPE\" CONTENT=\"text/html; charset=utf-8\">\n");
	_htmlFile->write("<TITLE></TITLE>\n</HEAD>\n");
	_htmlFile->write("<BODY TEXT=\"#000000\">\n");
}

void rs_html::end(void)
{
	_htmlFile->write("</BODY>\n</HTML>");
	_htmlFile->close();
}

void rs_html::write_table(QVector<QVector<QString> > *table, QVector<int> colSizes)
{
	QString _table = "<TABLE FRAME=VOID CELLSPACING=0 COLS=" + QVariant(colSizes.size()).toString() + " RULES=NONE BORDER=1>\n";
	
	_htmlFile->write(QVariant(_table).toByteArray());
	
	_htmlFile->write("<COLGROUP>");
	
	for (int i = 0; i < colSizes.size();  i++){
		_htmlFile->write("<COL WIDTH=");
		_htmlFile->write(QVariant(colSizes[i]).toByteArray());
		_htmlFile->write(">");
    }
	
	_htmlFile->write("</COLGROUP>\n<TBODY>\n");
	
	for (int row = 0; row < (*table).size(); row++){
		_htmlFile->write("<tr>");
		for (int col = 0; col <(*table)[row].size(); col++){
			_htmlFile->write("<td STYLE=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000\" ALIGN=CENTER>");
			_htmlFile->write("<FONT FACE=\"");
			_htmlFile->write(_font.toUtf8());
			_htmlFile->write("\">");
			_htmlFile->write((*table)[row][col].toUtf8());
			_htmlFile->write("</td>\n");
        }
		_htmlFile->write("</tr>\n");
    }

        _htmlFile->write("<tr colspan=" + QVariant((*table)[0].size()).toString().toUtf8() + ">");
        _htmlFile->write("<td STYLE=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000\" ALIGN=CENTER>");
        _htmlFile->write("<FONT FACE=\"");
        _htmlFile->write(_font.toUtf8());
        _htmlFile->write("\">");
        _htmlFile->write(QDateTime::currentDateTime().toString("dd.MM.yyyy-hh:mm:ss").toUtf8());
        _htmlFile->write("</td>\n");
        _htmlFile->write("</tr>\n");

        /*
        _htmlFile->write("<tr>");
                for (int col = 0; col <(*table)[0].size(); col++){
                        _htmlFile->write("<td STYLE=\"border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 1px solid #000000; border-right: 1px solid #000000\" ALIGN=CENTER>");
                        _htmlFile->write("<FONT FACE=\"");
                        _htmlFile->write(_font.toUtf8());
                        _htmlFile->write("\">");
                        _htmlFile->write(QDateTime::currentDateTime().toString("dd.MM.yyyy-hh:mm:ss").toUtf8());
                        _htmlFile->write("</td>\n");
                }
                _htmlFile->write("</tr>\n");
        */
	
	_htmlFile->write("</TBODY>\n");
}
