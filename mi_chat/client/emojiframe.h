#ifndef EMOJIFRAME_H
#define EMOJIFRAME_H

#include <QWidget>

namespace Ui {
class EmojiFrame;
}

class EmojiFrame : public QWidget
{
    Q_OBJECT

public:
    explicit EmojiFrame(QWidget *parent = nullptr);
    ~EmojiFrame();

private:
    Ui::EmojiFrame *ui;
};

#endif // EMOJIFRAME_H
