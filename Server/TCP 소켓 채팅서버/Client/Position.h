#include <windows.h>

enum IOOperation
{
	RECV,
	SEND
};

class Position{
private:
    COORD mSendPos;
    COORD mRecvPos;
public:
    Position(){
        mSendPos.X = 0, mSendPos.Y = 20;
        mRecvPos.X = 0, mRecvPos.Y = 2;
    }

    void SetSendPos(int nicknameLength){
        mSendPos.X = nicknameLength;
    }

    void SendBox(char* nickname){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mSendPos);
        printf("%c[2K\n%c[2K\n%c[2K\n%c[2K", 27, 27, 27, 27);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mSendPos);
        printf("===============================\n[%s´Ô]: ", nickname);
    }

    void Receive(char* nickname, const char* content){
        ++mRecvPos.Y;
        if(mRecvPos.Y>=20) {
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mSendPos);
            printf("%c[2K", 27);
            ++mSendPos.Y;
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), mRecvPos);
        printf("[%s´Ô]: %s\n", nickname, content);
    }
};
