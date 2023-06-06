#include "emojiframe.h"
#include "ui_emojiframe.h"

EmojiFrame::EmojiFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EmojiFrame)
{
    ui->setupUi(this);
}

EmojiFrame::~EmojiFrame()
{
    delete ui;
}
