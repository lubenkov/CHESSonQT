#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QBrush>
#include <QPalette>
#include <QMouseEvent>
#include <QIcon>
#include <QTime>
#include <QTimer>
#include <QFontDatabase>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QSound>
#include <QDir>
#include <QUrl>
#include <QMediaPlaylist>

#include <vector>

class Tile;

const int size_x = 120;
const int size_y = 120;

const int start_x = 600;
const int start_y = 100;

bool w_castle = false;
bool b_castle = false;

int av_num_w = -1; //для взятия пешки на проходе
int av_num_b = -1;

Tile* selectedPiece;
//Tile** pieces = new Tile * [64];
std::vector<Tile*> pieces(64);

QString w_square = "background-color: #A9A9A9;";
QString b_square = "background-color: #DC143C;";
QString c_square = "background-color: #4B0082;";
QString background = "background-color: #800000;";

char turn = 'w';

class SoundEffect
{
public:
    SoundEffect(QString filename, int volume)
    {
        QMediaPlayer* player = new QMediaPlayer;
        player->setMedia(QUrl::fromLocalFile("resources/" + filename));
        player->setVolume(volume);
        player->play();
       
    }
};

class Timer : public QLabel
{
public:
    Timer(QWidget* parent = 0) : QLabel(parent)
    {

        w_t = QTime(0, 5, 0);
        b_t = QTime(0, 5, 0);

        w_l = new QLabel(this);
        b_l = new QLabel(this);


        w_l->setStyleSheet("QLabel { color : white; }");
        b_l->setStyleSheet("QLabel { color : black; }");

        w_l->setText(w_t.toString());
        b_l->setText(b_t.toString());

        w_l->setGeometry(200, 600, 400, 200);
        b_l->setGeometry(200, 400, 400, 200);

        int id = QFontDatabase::addApplicationFont("resources/font1.ttf");
        QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont f(family);
        f.setPixelSize(60);
        w_l->setFont(f);
        b_l->setFont(f);

        startTimer(1000);





    }

    void timerEvent(QTimerEvent* e)
    {

        if (turn == 'w')
        {
            QTime time = w_t.addSecs(-1);
            w_t = time;
            w_l->setText(time.toString());
        }
        else if (turn == 'b')
        {
            QTime time = b_t.addSecs(-1);
            b_t = time;
            b_l->setText(time.toString());
        }

        if (w_t <= QTime(0, 0, 0, 0))
        {
            w_l->setText("WHITE LOST");
            b_l->setText("BLACK WON");
            QTime tmp = w_t.addSecs(1);
            w_t = tmp;
            turn = '0';

        }
        if (b_t <= QTime(0, 0, 0, 0))
        {
            w_l->setText("WHITE WON");
            b_l->setText("BLACK LOST");
            QTime tmp = b_t.addSecs(1);
            b_t = tmp;
            turn = '0';

        }

    }

private:
    QTime w_t, b_t;
    QLabel* w_l, * b_l;

};

class Tile : public QLabel
{
public:

    Tile(QWidget* parent = 0) : QLabel(parent)
    {

        clicked = false;
        moved = false;
        name = '0';
    }

    void setPos(const int& x, const int& y)
    {
        this->x = x + start_x;
        this->y = y + start_y;
        this->setGeometry(x + start_x, y + start_y, size_x, size_y);

    }



    void mousePressEvent(QMouseEvent* e)
    {
        if ((selectedPiece != nullptr && !clicked) || (selectedPiece == nullptr && !clicked && this->name != '0'))
        {
            clicked = true;
            this->setStyleSheet(c_square);



            movePiece();
        }
        else if (selectedPiece || (selectedPiece == nullptr && this->name != '0'))
        {
            clicked = false;
            colorBack();

            selectedPiece = nullptr;
        }
    }



    void colorBack()
    {
        if (this->colorStatus == 'w')
            this->setStyleSheet(w_square);
        else
            this->setStyleSheet(b_square);

        selectedPiece->clicked = false;
        this->clicked = false;


    }



    bool validation()
    {
        if (selectedPiece->name == 'r')
        {


            int d = 0;
            for (int i = selectedPiece->num; i > 7; i -= 8)
            {
                ++d;
            }
            int low_b = 8 * d;
            int up_b = low_b + 7;

            for (int i = selectedPiece->num - 8; i > -1; i -= 8)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num + 8; i < 64; i += 8)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num + 1; i < up_b + 1; i += 1)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num - 1; i > low_b - 1; i -= 1)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            return false;

        }
        else if (selectedPiece->name == 'h')
        {
            if (this->num == selectedPiece->num - 17 ||
                this->num == selectedPiece->num - 10 ||
                this->num == selectedPiece->num - 15 ||
                this->num == selectedPiece->num - 6 ||
                this->num == selectedPiece->num + 17 ||
                this->num == selectedPiece->num + 10 ||
                this->num == selectedPiece->num + 15 ||
                this->num == selectedPiece->num + 6)
                return true;
            else return false;
        }
        else if (selectedPiece->name == 'b')
        {
            for (int i = selectedPiece->num - 9; i > -1; i -= 9)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break; // для избежания ситуаций перепрыгивания на не подходящие клетки
            }

            for (int i = selectedPiece->num - 7; i > -1; i -= 7)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            for (int i = selectedPiece->num + 9; i < 64; i += 9)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            for (int i = selectedPiece->num + 7; i < 64; i += 7)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            return false;
        }
        else if (selectedPiece->name == 'q')
        {
            int d = 0;
            for (int i = selectedPiece->num; i > 7; i -= 8)
            {
                ++d;
            }
            int low_b = 8 * d;
            int up_b = low_b + 7;

            for (int i = selectedPiece->num - 8; i > -1; i -= 8)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num + 8; i < 64; i += 8)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num + 1; i < up_b + 1; i += 1)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num - 1; i > low_b - 1; i -= 1)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;
            }

            for (int i = selectedPiece->num - 9; i > -1; i -= 9)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            for (int i = selectedPiece->num - 7; i > -1; i -= 7)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            for (int i = selectedPiece->num + 9; i < 64; i += 9)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            for (int i = selectedPiece->num + 7; i < 64; i += 7)
            {
                if (i == this->num) return true;
                else if (pieces[i]->name != '0') break;

                if (pieces[i]->x == start_x || pieces[i]->y == start_y || pieces[i]->x == start_x + size_x * 7 || pieces[i]->y == start_y + size_y * 7) break;
            }

            return false;
        }
        else if (selectedPiece->name == 'k')
        {

            if (this->num == selectedPiece->num + 1 ||
                this->num == selectedPiece->num - 1 ||
                this->num == selectedPiece->num + 8 ||
                this->num == selectedPiece->num - 8 ||
                this->num == selectedPiece->num - 9 ||
                this->num == selectedPiece->num - 7 ||
                this->num == selectedPiece->num + 7 ||
                this->num == selectedPiece->num + 9)
                return true;
            else return false;
        }
        else if (selectedPiece->name == 'p')
        {


            if (selectedPiece->pieceColorStatus == 'w')
            {
                if (!selectedPiece->moved && this->num == selectedPiece->num - 16)
                {
                    av_num_b = selectedPiece->num - 8; // для взятия на проходе
                }
                if (this->num == av_num_w)
                {
                    pieces[av_num_w + 8]->clear(); // само взятие на проходе
                    pieces[av_num_w + 8]->setPieceColorStatus('0');
                    pieces[av_num_w + 8]->setName('0');

                    SoundEffect("capture.mp3", 50);

                    return true;
                }
                av_num_w = -1;


                if (!selectedPiece->moved && this->num == selectedPiece->num - 16 && this->name == '0' ||
                    this->num == selectedPiece->num - 8 && this->name == '0' ||
                    this->name != '0' && this->num == selectedPiece->num - 7 ||
                    this->name != '0' && this->num == selectedPiece->num - 9)
                    return true;
                else return false;
            }
            else if (selectedPiece->pieceColorStatus == 'b')
            {
                if (!selectedPiece->moved && this->num == selectedPiece->num + 16)
                {
                    av_num_w = selectedPiece->num + 8; // для взятия на проходе
                }
                if (this->num == av_num_b)
                {
                    pieces[av_num_b - 8]->clear(); // само взятие на проходе
                    pieces[av_num_b - 8]->setPieceColorStatus('0');
                    pieces[av_num_b - 8]->setName('0');

                    SoundEffect("capture.mp3", 50);

                    return true;
                }
                av_num_b = -1;

                if (!selectedPiece->moved && this->num == selectedPiece->num + 16 && this->name == '0' ||
                    this->num == selectedPiece->num + 8 && this->name == '0' ||
                    this->name != '0' && this->num == selectedPiece->num + 7 ||
                    this->name != '0' && this->num == selectedPiece->num + 9)
                    return true;
                else return false;
            }
        }

        if (selectedPiece->pieceColorStatus != 'p')
        {
            av_num_w = -1;
            av_num_b = -1;
        }
    }



    void movePiece()
    {
        if (selectedPiece)
        {
            if (selectedPiece->pieceColorStatus == turn)
            {
                if (selectedPiece->name == 'k' && this->name == 'r')
                {
                    castling();

                }
                else if (selectedPiece->name == 'p' && (this->y == start_y || this->y == start_y + size_y * 7) && (this->name == '0' || this->pieceColorStatus != selectedPiece->pieceColorStatus) && validation())
                {
                    promotion();
                }
                else if (selectedPiece->name != '0' && validation())
                {
                    standartMoving();
                }
            }

            selectedPiece->colorBack();
            this->colorBack();
            selectedPiece = nullptr;
        }
        else
        {
            selectedPiece = this;
        }
    }

    void standartMoving()
    {
        if (selectedPiece->pieceColorStatus == this->pieceColorStatus)
        {
            return;
        }
        else if (this->pieceColorStatus == 'b' || this->pieceColorStatus == 'w')
        {
            SoundEffect("capture.mp3", 50);
        }
        else
        {

            SoundEffect("move-self.mp3", 50);
        }

        copyPiece(selectedPiece, this);

        switch_turn();


    }

    void castling()
    {
        if (selectedPiece == pieces[60] && this == pieces[63] && pieces[61]->name == '0' && pieces[62]->name == '0' && !w_castle && !pieces[60]->moved && !pieces[63]->moved) //короткая рок белых
        {
            pieces[62]->setPixmap(*selectedPiece->pixmap());
            pieces[61]->setPixmap(*this->pixmap());

            pieces[62]->name = 'k';
            pieces[62]->pieceColorStatus = 'w';
            pieces[61]->name = 'r';
            pieces[61]->pieceColorStatus = 'w';

            pieces[60]->name = '0';
            pieces[60]->pieceColorStatus = '0';
            pieces[63]->name = '0';
            pieces[63]->pieceColorStatus = '0';

            w_castle = true;
            SoundEffect("castle.mp3", 50);


        }
        else if (selectedPiece == pieces[60] && this == pieces[56] && pieces[59]->name == '0' && pieces[58]->name == '0' && pieces[57]->name == '0' && !w_castle && !pieces[60]->moved && !pieces[56]->moved) //длинная рок белых
        {
            pieces[58]->setPixmap(*selectedPiece->pixmap());
            pieces[59]->setPixmap(*this->pixmap());

            pieces[58]->name = 'k';
            pieces[58]->pieceColorStatus = 'w';
            pieces[59]->name = 'r';
            pieces[59]->pieceColorStatus = 'w';

            pieces[60]->name = '0';
            pieces[60]->pieceColorStatus = '0';
            pieces[56]->name = '0';
            pieces[56]->pieceColorStatus = '0';

            w_castle = true;
            SoundEffect("castle.mp3", 50);


        }
        else if (selectedPiece == pieces[4] && this == pieces[7] && pieces[5]->name == '0' && pieces[6]->name == '0' && !b_castle && !pieces[4]->moved && !pieces[7]->moved) //короткая для черных
        {
            pieces[6]->setPixmap(*selectedPiece->pixmap());
            pieces[5]->setPixmap(*this->pixmap());

            pieces[6]->name = 'k';
            pieces[6]->pieceColorStatus = 'b';
            pieces[5]->name = 'r';
            pieces[5]->pieceColorStatus = 'b';

            pieces[4]->name = '0';
            pieces[4]->pieceColorStatus = '0';
            pieces[7]->name = '0';
            pieces[7]->pieceColorStatus = '0';

            b_castle = true;
            SoundEffect("castle.mp3", 50);

        }
        else if (selectedPiece == pieces[4] && this == pieces[0] && pieces[1]->name == '0' && pieces[2]->name == '0' && pieces[3]->name == '0' && !b_castle && !pieces[4]->moved && !pieces[0]->moved) // длинная для черных
        {
            pieces[2]->setPixmap(*selectedPiece->pixmap());
            pieces[3]->setPixmap(*this->pixmap());

            pieces[2]->name = 'k';
            pieces[2]->pieceColorStatus = 'b';
            pieces[3]->name = 'r';
            pieces[3]->pieceColorStatus = 'b';

            pieces[4]->name = '0';
            pieces[4]->pieceColorStatus = '0';
            pieces[0]->name = '0';
            pieces[0]->pieceColorStatus = '0';

            b_castle = true;
            SoundEffect("castle.mp3", 50);

        }
        else {
            standartMoving();
            return;
        }

        selectedPiece->clear();
        this->clear();


        switch_turn();



    }

    void promotion()
    {

        if (selectedPiece->pieceColorStatus == this->pieceColorStatus)
        {
            selectedPiece->colorBack();
            this->colorBack();
            selectedPiece = nullptr;
            return;
        }

        if (selectedPiece->pieceColorStatus == 'w')
        {
            this->setPixmap((QPixmap("resources/whitequeen.png").scaled(size_x, size_y)));
        }
        else
        {
            this->setPixmap((QPixmap("resources/blackqueen.png").scaled(size_x, size_y)));

        }
        selectedPiece->clear();


        this->name = 'q';
        this->pieceColorStatus = selectedPiece->pieceColorStatus;
        selectedPiece->pieceColorStatus = '0';
        selectedPiece->name = '0';


        this->moved = true;


        switch_turn();

    }

    void copyPiece(Tile* o1, Tile* o2)
    {
        o2->setPixmap(*o1->pixmap());
        o2->moved = true;
        o2->name = o1->name;
        o2->clicked = false;
        o2->pieceColorStatus = o1->pieceColorStatus;
        o1->clear();
        o1->name = '0';
        o1->pieceColorStatus = '0';



    }

    void switch_turn()
    {
        if (turn == 'w') turn = 'b';
        else turn = 'w';
    }

    void setColorStatus(char s)
    {
        colorStatus = s;
    }


    void setName(char s)
    {
        name = s;
    }

    char getName()
    {
        return name;
    }

    void setPieceColorStatus(char s)
    {
        pieceColorStatus = s;
    }

    void setNumber(int n)
    {
        this->num = n;
    }



private:

    //QPixmap pixmap;
    char name;

    int num;

    bool clicked;
    char colorStatus;
    char pieceColorStatus;

    bool moved;

    


    int x;
    int y;
};



class Board : public QWidget
{
public:

    Board(QWidget* parent = 0) : QWidget(parent)
    {


        this->setStyleSheet(background);

        QMediaPlaylist* playlist = new QMediaPlaylist();
        playlist->addMedia(QUrl("resources/lofi.mp3"));
        playlist->setPlaybackMode(QMediaPlaylist::Loop);

        QMediaPlayer* music = new QMediaPlayer();
        music->setPlaylist(playlist);
        music->setVolume(10);
        music->play();

        Timer* timer = new Timer(this);
        timer->setGeometry(0, 0, 2000, 2000);

        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                pieces[i * 8 + j] = new Tile(this);
                pieces[i * 8 + j]->setPos(size_x * j, size_y * i);
                pieces[i * 8 + j]->setNumber(i * 8 + j);

                if (i % 2 == 0)
                {
                    if (j % 2 == 0) {
                        pieces[i * 8 + j]->setColorStatus('w');
                        pieces[i * 8 + j]->setStyleSheet(w_square);
                    }

                    else {
                        pieces[i * 8 + j]->setColorStatus('b');
                        pieces[i * 8 + j]->setStyleSheet(b_square);
                    }
                }
                else
                {
                    if (j % 2 == 0) {
                        pieces[i * 8 + j]->setColorStatus('b');
                        pieces[i * 8 + j]->setStyleSheet(b_square);
                    }
                    else {
                        pieces[i * 8 + j]->setColorStatus('w');
                        pieces[i * 8 + j]->setStyleSheet(w_square);
                    }
                }

            }
        }



        pieces[0]->setPixmap(QPixmap("resources/blackrook.png").scaled(size_x, size_y));
        pieces[0]->setName('r');
        pieces[0]->setPieceColorStatus('b');


        pieces[1]->setPixmap(QPixmap("resources/blackknight.png").scaled(size_x, size_y));
        pieces[1]->setName('h');
        pieces[1]->setPieceColorStatus('b');


        pieces[2]->setPixmap(QPixmap("resources/blackbishop.png").scaled(size_x, size_y));
        pieces[2]->setName('b');
        pieces[2]->setPieceColorStatus('b');

        pieces[3]->setPixmap(QPixmap("resources/blackqueen.png").scaled(size_x, size_y));
        pieces[3]->setName('q');
        pieces[3]->setPieceColorStatus('b');


        pieces[4]->setPixmap(QPixmap("resources/blackking.png").scaled(size_x, size_y));
        pieces[4]->setName('k');
        pieces[4]->setPieceColorStatus('b');


        pieces[5]->setPixmap(QPixmap("resources/blackbishop.png").scaled(size_x, size_y));
        pieces[5]->setName('b');
        pieces[5]->setPieceColorStatus('b');


        pieces[6]->setPixmap(QPixmap("resources/blackknight.png").scaled(size_x, size_y));
        pieces[6]->setName('h');
        pieces[6]->setPieceColorStatus('b');


        pieces[7]->setPixmap(QPixmap("resources/blackrook.png").scaled(size_x, size_y));
        pieces[7]->setName('r');
        pieces[7]->setPieceColorStatus('b');


        for (size_t i = 8; i < 16; ++i)
        {
            pieces[i]->setPixmap(QPixmap("resources/blackpawn.png").scaled(size_x, size_y));
            pieces[i]->setName('p');
            pieces[i]->setPieceColorStatus('b');

        }

        for (size_t i = 48; i < 56; ++i)
        {
            pieces[i]->setPixmap(QPixmap("resources/whitepawn.png").scaled(size_x, size_y));
            pieces[i]->setName('p');
            pieces[i]->setPieceColorStatus('w');

        }

        pieces[56]->setPixmap(QPixmap("resources/whiterook.png").scaled(size_x, size_y));
        pieces[56]->setName('r');
        pieces[56]->setPieceColorStatus('w');


        pieces[57]->setPixmap(QPixmap("resources/whiteknight.png").scaled(size_x, size_y));
        pieces[57]->setName('h');
        pieces[57]->setPieceColorStatus('w');


        pieces[58]->setPixmap(QPixmap("resources/whitebishop.png").scaled(size_x, size_y));
        pieces[58]->setName('b');
        pieces[58]->setPieceColorStatus('w');


        pieces[59]->setPixmap(QPixmap("resources/whitequeen.png").scaled(size_x, size_y));
        pieces[59]->setName('q');
        pieces[59]->setPieceColorStatus('w');


        pieces[60]->setPixmap(QPixmap("resources/whiteking.png").scaled(size_x, size_y));
        pieces[60]->setName('k');
        pieces[60]->setPieceColorStatus('w');


        pieces[61]->setPixmap(QPixmap("resources/whitebishop.png").scaled(size_x, size_y));
        pieces[61]->setName('b');
        pieces[61]->setPieceColorStatus('w');


        pieces[62]->setPixmap(QPixmap("resources/whiteknight.png").scaled(size_x, size_y));
        pieces[62]->setName('h');
        pieces[62]->setPieceColorStatus('w');


        pieces[63]->setPixmap(QPixmap("resources/whiterook.png").scaled(size_x, size_y));
        pieces[63]->setName('r');
        pieces[63]->setPieceColorStatus('w');







    }




private:

};


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Board w;
    w.setWindowTitle("Chess");
    w.setWindowIcon(QIcon("resources/icon.png"));
    //w.resize(size_x * 8, size_y * 8);
    w.showMaximized();

    return a.exec();
}
