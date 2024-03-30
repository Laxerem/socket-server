#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;

int main(int argc, char *argv[]) {

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7000); /* рекомендуется всегда использовать htons() 
    для преобразования порта в сетевой порядок байт, чтобы гарантировать корректную 
    работу вашего сетевого приложения на разных платформах.
    */
    addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_LOOPBACK - адрес интерфейса внутренней петли (loopback interface)
    // addr.sin_addr.s_addr = inet_addr();

    int server = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(server, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        cout << "Не удалось связать сокет сервера с адресом." << endl;
        return 1;
    }
    else 
        cout << "Порт открыт" << endl;
    
    if (listen(server, 1) == 0) {
        cout << "Сокет стал в режим прослушки" << endl;
        
        int client;

        if ((client = accept(server, nullptr, nullptr)) < 0) {
            cout << "Не получилось соединится с клиентом" << endl;
            close(server);
            return -1;
        }
        else
            cout << "Успешное соединение с клиентом" << endl;

        char buf[1024];
        int bytes_read;

        if ((bytes_read = recv(client, buf, 1024, 0)) == 0) {
            cout << "Соединение разорвано";
            return 1;
        }
        else {
            cout << "Использовано: " << bytes_read << " байт" << endl;
            cout << buf << endl;
        }

        close(client);
    }
    else {
        close(server);
        cout << "Какая то фигня";
        return 1;
    }

    close(server);


    return 0;
}
