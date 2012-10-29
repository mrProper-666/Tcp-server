/**
*	подключаем библиотеки
*/

#include <QtCore/QCoreApplication>
#include <QTextCodec>
#include "server.h"

int main(int argc, char *argv[])								// точка входа в приложение
{
	// создаем кодек дл€ нормальной кодировки
    QTextCodec *codec = QTextCodec::codecForName("CP-1251");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);

    QCoreApplication a(argc, argv);
    Server *m_server = new Server();							// создаем экземпл€р класса Server
    
    return a.exec();											// запускаем приложение
}
