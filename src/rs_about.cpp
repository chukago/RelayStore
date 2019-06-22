#include "rs_about.h"

rs_about::rs_about(QWidget *parent)
	:QDialog(parent)
{
    setWindowTitle("О программе");
    resize(320, 120);

    QLabel *_info = new QLabel("Клиент базы данных РТУ", this);
    _info->setAlignment(Qt::AlignCenter);

    QPushButton *_ok = new QPushButton(tr("Ok"), this);
	
    QGridLayout *_gl = new QGridLayout(this);
	_gl->addWidget(_info, 0, 0);
	_gl->addWidget(_ok, 1, 0);
	
	setLayout(_gl);

    QObject::connect(_ok, &QPushButton::clicked, this, &QDialog::close);
}
