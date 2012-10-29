/**
*	���������� ����������
*/

#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include "server.h"

int main(int argc, char *argv[])								// ����� ����� � ����������
{
	// ������� ����� ��� ���������� ���������
    QTextCodec *codec = QTextCodec::codecForName("CP-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QCoreApplication a(argc, argv);
    Server *m_server = new Server();							// ������� ��������� ������ Server
    
    return a.exec();											// ��������� ����������
}
