/**
*	���������� ����������
*/

#include "server.h"

/**
*	����������� ������ Server
*/
Server::Server(QObject *parent) :
    QObject(parent)
{
	// ������� ��������� ������ QTcpServer
    tcpServer = new QTcpServer(this);

	// �������� ��������� IP ����� �������
    QList< QHostAddress > addresses = QHostInfo::fromName( QHostInfo::localHostName() ).addresses();
        // ��� ������� ������ �� �����
		foreach ( const QHostAddress & a, addresses ){
            QString protocol = "???";
				// ����������� ��������
                switch ( a.protocol() ){
					// ���� IPv4
                    case QAbstractSocket::IPv4Protocol:
                        protocol = "IPv4";
                    break;
					// ���� IPv6
                    case QAbstractSocket::IPv6Protocol:
                        protocol = "IPv6";
                    break;
					// ���� ����������� ��������
                    case QAbstractSocket::UnknownNetworkLayerProtocol:
                        protocol = "err";
                    break;
                }
				// �������� ������ IPv4
                if(protocol == "IPv4"){
                    address = a.toString();
                    break;
                }
        }

	// �������� ��������� ������
    if(startServer(QHostAddress::Any,666)){
		
		// ���� ���������� �� ������� � ������� ���������
        std::cout << "Server has started successfully on port 666\n";
		
		// � ��������� ����� �������
        std::cout << QString("Address for connection is %1:666\n").arg(address).toStdString();
	// �����	
    }else{
		// ����� � ������� ��� ������ �� ������ � ���� ������
        std::cout << tr("Unable to start the server: %1.\n").arg(tcpServer->errorString()).toStdString();
    }
}

/**
*	���������� ������ MainWindow
*/
Server::~Server() {
}

/**
*	������� ������� �������
*/
bool Server::startServer(QHostAddress addr, qint16 port){
	// ������ ��� ��������� ������ ����������� � �������
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newUser()));
	
	// ������� �������� ��������� �����
    if (!tcpServer->listen(addr, port)) {
		
		// ���� ������ �� 0
        return false;
		
	// �����
    } else {
		
		// ����� 1
        return true;
    }
	
	// � �������� ������� ��� ������� ���
    return false;
}

/**
*	���� ��������� ������ �����������
*/
void Server::newUser(){
	// �������� ��������� �� ����� ���������� �����������
    socket = tcpServer->nextPendingConnection();
	
	// ������ ��� ��������� ������ � ������
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
	
	// ������� ��������� � ����� ����������� � ������� � IP ����� �������
    std::cout << QString("New connection from host %1\n").arg(socket->peerAddress().toString()).toStdString();
	
	// ���������� ������� �����������
    sendString(socket, tr("����� ���������� �� ������"));
}

/**
*	������� �������� ������ �������
*/
void Server::sendString(QTcpSocket *sock,QString string){
	// ������ ������
    QByteArray block;
	
	// ������� ����� ������ � ������ ������
    QDataStream out(&block, QIODevice::WriteOnly);

	// ������������� ������ ��� �������������
    out.setVersion(QDataStream::Qt_4_0);
	
	// ���������� ������ � �����
    out << string;
	
	// ����� �� ������
    out.device()->seek(0);
	
	// ���������� ������ �������
    sock->write(block);
}

/**
*	������� ��������� ������ �� �������
*/
void Server::readData(){
	// ������� ����� ������ ������
    QDataStream in(socket);
	
	// ��������� ������ ��� �������������
    in.setVersion(QDataStream::Qt_4_0);

	// ������ ������
    QString dataString;
	
	// �������� ������ �� ������
    in >> dataString;
	
	// ������� ��������� � ���������� ������ �� �������
    std::cout << QString("\"%1\" received from \"%2\" \n")
                 .arg(dataString)
                 .arg(socket->peerAddress().toString()).toStdString();
	
	// ��������� �����
    dataString = sortLine(dataString);
	
	// ������� ��������� �� �������� ������������� ������ �������
    std::cout << QString("Sending sorting line \"%1\" to client \n")
                 .arg(dataString).toStdString();
				 
	// ���������� ������
    sendString(socket, QString("%1").arg(sortLine(dataString)));
}

/**
*	������� ���������� ����� �� �����������
*/
QString Server::sortLine(QString string){
    // ������� ������
	QString str;
	
	// ���� ����� �����
    QList<int> list;
    int i = 0;
	
	// ���� ������ �� ������
    while(!string.isEmpty()){
	
		// ��������� ������ �� ������ � �������� c i-� �� i-� ������ (�.�. ���� i-�)
        str = string.section(' ',i,i);
		
		// ������� ������ str �� string
        string.remove(0,str.length()+1);
		
		// ���� string �� �������� ������ (����� ��� ��������� ������
        if(!string.contains(' '))
			// ������� �� string str
            string.remove(str);
		// ��������� ����� ����� � ������
        list << str.toInt();
    }

	// ��������� ������ �� �����������
    qSort(list.begin(), list.end());
    str.clear();

	// ��������� ������ ��� �������� �������
    for(int i = 0; i< list.count(); i++)
        str += QString("%1 ").arg(list.at(i));

	// ������� ��������� ������
    str.remove(str.length(),1);

	// ���������� ������
    return str;
}
