/**
*	подключаем библиотеки
*/

#include "server.h"

/**
*	конструктор класса Server
*/
Server::Server(QObject *parent) :
    QObject(parent)
{
	// создаем экземпляр класса QTcpServer
    tcpServer = new QTcpServer(this);

	// получаем локальный IP адрес сервера
    QList< QHostAddress > addresses = QHostInfo::fromName( QHostInfo::localHostName() ).addresses();
        // для каждого адреса из листа
		foreach ( const QHostAddress & a, addresses ){
            QString protocol = "???";
				// переключаем протокол
                switch ( a.protocol() ){
					// если IPv4
                    case QAbstractSocket::IPv4Protocol:
                        protocol = "IPv4";
                    break;
					// если IPv6
                    case QAbstractSocket::IPv6Protocol:
                        protocol = "IPv6";
                    break;
					// если неизвестный протокол
                    case QAbstractSocket::UnknownNetworkLayerProtocol:
                        protocol = "err";
                    break;
                }
				// забираем первый IPv4
                if(protocol == "IPv4"){
                    address = a.toString();
                    break;
                }
        }

	// пытаемся запустить сервер
    if(startServer(QHostAddress::Any,666)){
		
		// если запустился то выводим в консоль пояснения
        std::cout << "Server has started successfully on port 666\n";
		
		// и локальный адрес сервера
        std::cout << QString("Address for connection is %1:666\n").arg(address).toStdString();
	// иначе	
    }else{
		// пишем в консоль что запуск не удачен и саму ошибку
        std::cout << tr("Unable to start the server: %1.\n").arg(tcpServer->errorString()).toStdString();
    }
}

/**
*	деструктор класса MainWindow
*/
Server::~Server() {
}

/**
*	функция запуска сервера
*/
bool Server::startServer(QHostAddress addr, qint16 port){
	// макрос для обработки нового подключения к серверу
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
	
	// пытаемя включить прослушку порта
    if (!tcpServer->listen(addr, port)) {
		
		// если ошибка то 0
        return false;
		
	// иначе
    } else {
		
		// иначе 1
        return true;
    }
	
	// в принципе незачем эта строчка тут
    return false;
}

/**
*	слот обработки нового подключения
*/
void Server::newUser(){
	// получаем указатель на сокет последнего подключения
    socket = tcpServer->nextPendingConnection();
	
	// макрос для получения данных с сокета
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	
	// выводим сообщение о новом подключении к серверу и IP адрес клиента
    std::cout << QString("New connection from host %1\n").arg(socket->peerAddress().toString()).toStdString();
	
	// отправляем клиенту приветствие
    sendString(socket, tr("Добро пожаловать на сервер"));
}

/**
*	функция отправки данных клиенту
*/
void Server::sendString(QTcpSocket *sock,QString string){
	// массив байтов
    QByteArray block;
	
	// создаем поток данных в массив байтов
    QDataStream out(&block, QIODevice::WriteOnly);

	// устанавливаем версию для совместимости
    out.setVersion(QDataStream::Qt_4_0);
	
	// отправляем данные в поток
    out << string;
	
	// поток на начало
    out.device()->seek(0);
	
	// отправляем данные клиенту
    sock->write(block);
}

/**
*	функция получения данных от клиента
*/
void Server::readData(){
	// создаем поток данных сокета
    QDataStream in(socket);
	
	// указываем версию для совместимости
    in.setVersion(QDataStream::Qt_4_0);

	// строка данных
    QString dataString;
	
	// получаем строку из потока
    in >> dataString;
	
	// выводим сообщение о полученных данных от клиента
    std::cout << QString("\"%1\" received from \"%2\" \n")
                 .arg(dataString)
                 .arg(socket->peerAddress().toString()).toStdString();
	
	// сортируем числа
    dataString = sortLine(dataString);
	
	// выводим сообщение об отправке сортированной строки клиенту
    std::cout << QString("Sending sorting line \"%1\" to client \n")
                 .arg(dataString).toStdString();
				 
	// отправляем строку
    sendString(socket, QString("%1").arg(sortLine(dataString)));
}

/**
*	функция сортировки чисел по возрастанию
*/
QString Server::sortLine(QString string){
    // обычная строка
	QString str;
	
	// лист целых чисел
    QList<int> list;
    int i = 0;
	
	// пока строка не пустая
    while(!string.isEmpty()){
	
		// разбиваем строку на секции и получаем c i-й по i-ю секцию (т.е. одну i-ю)
        str = string.section(' ',i,i);
		
		// удаляем строку str из string
        string.remove(0,str.length()+1);
		
		// если string не содержит пробел (нужно для последней секции
        if(!string.contains(' '))
			// удаляем из string str
            string.remove(str);
		// добавляем целое число в список
        list << str.toInt();
    }

	// сортируем список по возрастанию
    qSort(list.begin(), list.end());
    str.clear();

	// формируем строку для отправки клиенту
    for(int i = 0; i< list.count(); i++)
        str += QString("%1 ").arg(list.at(i));

	// удаляем последний пробел
    str.remove(str.length(),1);

	// возвращаем строку
    return str;
}
