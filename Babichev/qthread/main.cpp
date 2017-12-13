#include <QtCore>
#include <iostream>
#include <sstream>
#include <unistd.h>

/*class Worker : public QObject
{
    Q_OBJECT

public:
    void pass_id(int id, int id2)
    {
        q_id = id;
        l_id = id2;
    }

private:
    int q_id;
    int l_id;
signals:
    void wake_up();
private slots:
};
*/
class Thread : public QThread
{
    Q_OBJECT

public:
    void pass_id(int id, int id2)
    {
        q_id = id;
        l_id = id2;
    }

private:
    int q_id;
    int l_id;

    void run()
    {
        if (q_id == 0) {
            emit notify_next();
            return;
        }
        exec();
    }
signals:
    void notify_next();
public slots:
    void do_work() {
        qDebug() << "Internal id: "<< q_id;
        emit notify_next();
        quit();
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication loop(argc, argv);

    if (argc != 2) {
        std::cerr << "Usage: ./main THREAD_NUM\n";
        return 1;
    }

    std::stringstream conv(argv[1]);

    int thread_num = 0;
    conv >> thread_num;

    Thread *thread = new Thread[thread_num];
    int i;

    qDebug() << "Internal id: "<< 0;
    for (i = 0; i < thread_num; i++) {
        thread[i].pass_id(i, thread_num);
        if (i != thread_num - 1) {
            QObject::connect(&thread[i], SIGNAL(notify_next()),
                             &thread[i+1], SLOT(do_work()));
            thread[i+1].moveToThread(&thread[i]);
        }
    }
    sleep(1);
    for (i = 0; i < thread_num; i++)
        thread[i].start();
    for (i = 0; i < thread_num; i++) {
        thread[i].wait();
    }

    delete [] thread;
    return 0;
}
