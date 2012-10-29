/**
*	подключаем библиотеки
*/

#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <iostream>
#include <QHostInfo>
#include <QtAlgorithms>

// создаем класс главного окна
class Server : public QObject
{
// макрос(нужен для сигнал-слотов)
    Q_OBJECT

// публичные объявления (доступны вне класса)
public:
    explicit Server(QObject *parent = 0);			// конструктор (explicit - запрещает автоматическое создание конвертирующего конструrтора
    ~Server();										// деструктор

// приватные объявления (доступны только в классе)
private:
    QTcpServer *tcpServer;							// указатель на экземпляр класса QTcpServer
    QTcpSocket *socket;								// указатель на экземпляр класса QTcpSocket
    bool startServer(QHostAddress, qint16);			// метод запускающий сервер (аргументы - адрес и порт, возвращаем true если соединился и false если нет)
    QString address;								// IP адрес сервера
    void sendString(QTcpSocket *, QString);			// метод отправки данных клиенту (аргументы - сокет, в который отправляем и сами данные)
    QString sortLine(QString);						// метод сортировки строки, возвращает строку

// приватные слоты (доступны только из класса)
private slots:
    void newUser();									// слот обработки нового подключения
    void readData();								// слот чтения данных от клиента
};

#endif // SERVER_H
