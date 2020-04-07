#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <string>
using namespace std;

int ProcessClientRequest (int clSocket,sockaddr* ClientAddress){
    char buf[1024];
    int bytes_read;
    ofstream fout;
    ifstream t("501.txt");
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);     
    fout.open("t.txt", ios::app);
    while(1){
        bytes_read = recv(clSocket, buf, 1024, 0);
        if(bytes_read < 0) {
            break;
        }
        if (bytes_read == 0)
            break;
        send(clSocket, &buffer[0], size, 0);
        fout << buf;
    }
    cout << buffer << endl;
    fout.close();
    shutdown(clSocket, 2);
    close(clSocket);
    return bytes_read;
}


int main(){

    int serverSocket = socket ( AF_INET, SOCK_STREAM, 0 );
    if (serverSocket < 0){
        perror("socket");
        exit(1);
    }
    struct sockaddr_in ServerAddress;
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(3333);
    ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
 
    if ( bind ( serverSocket, (sockaddr*)&ServerAddress, sizeof ( ServerAddress ) ) < 0 ){
        perror("bind");
        exit(2);
    }
    if ( listen ( serverSocket, 1) < 0 ) {
        perror("listen");
        exit(3);
    }

    int flag_error = 0;
    for (;;) {
        struct sockaddr_in ClientAddress;
        socklen_t ClAddrLen = sizeof(ClientAddress);                                    
        // ждем очередного клиента 
        int clSocket = accept (serverSocket,(sockaddr*)&ClientAddress, &ClAddrLen);
        if ( clSocket < 0 ){
            flag_error++;
            if (flag_error > 5) {
                perror("fatal");
                exit(4);
            }
        } else {
            flag_error = 0;
        }
    ProcessClientRequest(clSocket,(sockaddr*)&ClientAddress);
}
    return 0;
}