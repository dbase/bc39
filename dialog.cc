/**************************************************************************
 Copyright (c) 2014, Sergey Kulishenko <serkul(at)ukr(dot)net>

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**************************************************************************/

#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>
#include <QFileDialog>

#include <bc39/bc39.h>

Dialog::Dialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);

  connect(ui->draw, SIGNAL(clicked()), SLOT(draw()));
  connect(ui->save, SIGNAL(clicked()), SLOT(save()));
}

Dialog::~Dialog()
{
  delete ui;
}

void Dialog::draw()
{
  using bc39::Generator;

  Generator::setWideToNarrowRatio(ui->ratio->value());

  Generator g;
  g.setHeight(ui->height->value());
  g.setNarrowWidth(ui->narrowWidth->value());
  int rv = g.generate(ui->input->text().toUtf8().data());

  if (rv == 0) {
    QImage i;
    i.loadFromData(g.bitmap().toXpm());
    ui->view->setPixmap(QPixmap::fromImage(i));
  } else {
    ui->view->clear();
    switch (rv) {
    case Generator::kInvalidCharacter:
      ui->view->setText("Invalid character detected in the input!");
      break;
    case Generator::kOutOfMemory:
      ui->view->setText("Ran out of memory!");
      break;
    }
  }
}

void Dialog::save()
{
  QString fileName = QFileDialog::getSaveFileName(
                       this,
                       "Save As PNG...",
                       ".",
                       QString("PNG files (*.png);;All files (*)"));
  if (!fileName.isEmpty()) {
    if (!fileName.endsWith(".png"))
      fileName.append(".png");
    ui->view->pixmap()->save(fileName, "PNG");
  }
}
