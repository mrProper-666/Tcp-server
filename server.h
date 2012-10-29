/**
*	���������� ����������
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

// ������� ����� �������� ����
class Server : public QObject
{
// ������(����� ��� ������-������)
    Q_OBJECT

// ��������� ���������� (�������� ��� ������)
public:
    explicit Server(QObject *parent = 0);			// ����������� (explicit - ��������� �������������� �������� ��������������� �������r����
    ~Server();										// ����������

// ��������� ���������� (�������� ������ � ������)
private:
    QTcpServer *tcpServer;							// ��������� �� ��������� ������ QTcpServer
    QTcpSocket *socket;								// ��������� �� ��������� ������ QTcpSocket
    bool startServer(QHostAddress, qint16);			// ����� ����������� ������ (��������� - ����� � ����, ���������� true ���� ���������� � false ���� ���)
    QString address;								// IP ����� �������
    void sendString(QTcpSocket *, QString);			// ����� �������� ������ ������� (��������� - �����, � ������� ���������� � ���� ������)
    QString sortLine(QString);						// ����� ���������� ������, ���������� ������

// ��������� ����� (�������� ������ �� ������)
private slots:
    void newUser();									// ���� ��������� ������ �����������
    void readData();								// ���� ������ ������ �� �������
};

#endif // SERVER_H
