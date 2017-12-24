#include <QtCore>
#include <iostream>
#include <sstream>
#include <unistd.h>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread() : id(0) {};
    void set_id(int id)
    {
        this->id = id;
    }
    void do_work() {
        qDebug() << "Internal id: "<< id;
        emit notify_next();
        quit();
    }

private:
    int id;

    void run()
    {
        exec();
    }
signals:
    void notify_next();
public slots:
    void do_work_handler() {
            do_work();
    }
};

#include "main.moc"

int main(int argc, char *argv[])
{
    QCoreApplication loop(argc, argv);
    int thread_num = 0;
    int i;

    if (argc != 2) {
        std::cerr << "Usage: ./main THREAD_NUM\n";
        return 1;
    }

    std::stringstream conv(argv[1]);

    conv >> thread_num;

    Thread *thread = new Thread[thread_num];

    for (i = 0; i < thread_num - 1; i++) {
        thread[i].set_id(i);
        QObject::connect(&thread[i], SIGNAL(notify_next()),
                         &thread[i + 1], SLOT(do_work_handler()));
    }
    thread[thread_num - 1].set_id(thread_num - 1);

    for (i = 0; i < thread_num; i++)
        thread[i].start();

    thread[0].do_work();

    for (i = 0; i < thread_num; i++)
        thread[i].wait();

    delete [] thread;
    return 0;
}
