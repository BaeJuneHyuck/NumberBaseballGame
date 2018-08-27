#include <iostream>	
#include <string>	// ���ڿ��� ���� �Է¹ޱ�
#include <random>	// ���� ����
#include <fstream>	// ���� ������ ����
#include <WinSock2.h>// [��ó] ������ �������α׷��� ���� ���� | �ۼ��� �����ϰ� ��

using namespace std;

#define SAVEPATH "record.txt"

struct checkC {
	checkC(int b, int s) : ball(b), strike(s) {}
	int ball;
	int strike;

	int getBall() {
		return ball;
	}
	int getStrike() {
		return strike;
	}
};

checkC compare(int answ[], int guess[],int GameDigits) {
	checkC check(0, 0);

	for (int i = 0; i < GameDigits; i++) {
		for (int j = 0; j < GameDigits; j++) {
			if (answ[i] == guess[j]) {	// �������� �����Ѵ�
				if (i == j)				// ���� ��ġ�� ��Ʈ����ũ �ٸ���ġ�� ��
					check.strike++;
				else
					check.ball++;
			}
		}
	}
	return check;
}

string getNumber(int GameDigits)	// ���ڿ��� �Է¹���, n�ڸ� ������ �ƴҰ�� ������������ �Ǿ�����
{
	bool ok = false;
	string result;
	while (!ok)
	{
		cout << "�ߺ����� " << GameDigits << "�ڸ� ���ڸ� �Է����ּ��� : ";
		cin >> result;
		if (result.length() == GameDigits)
		{
			bool allDigits = true;
			for (unsigned index = 0; index < GameDigits; ++index)
			{
				allDigits = allDigits && (
					(result[index] >= '0') &&
					(result[index] <= '9')
					);
			}
			ok = allDigits;
		}
	}
	return result;
}

bool IsRepetition(int arr[],int GameDigits ) {		// ����Ҽ� ���� �������� Ȯ�����ݴϴ�
	for (int i = 0; i < GameDigits; i++) {
		for (int j = i + 1; j < GameDigits; j++) {
			if (arr[i] == arr[j]) {
				return true;
			}
		}
	}
	return false;
}

void makeProfile() {
	ofstream os;
	string ID;
	cout << " Input your new ID" << endl;
	cout << " '0' for cancel " << endl;
	
	cin >> ID;
	if (ID == "0") {
		return;
	}
	os.open(SAVEPATH,'w');
	os << ID << endl;
	for (int i = 0; i < 8; i++) {
		os << 0 << endl;
	}
	os.close();
}

void showRecord() {
	
	ifstream is;
	is.open(SAVEPATH);
	if (! is.is_open() ) {
		cout << "record file Not found" << endl;
		return;
	}

	ofstream os;
	int data[8];
	string ID;
	int length = ID.length();

	is >> ID; // ID
	is >> data[0]; // triple �� ����Ƚ��
	is >> data[1]; // triple �ְ�����
	is >> data[2]; // quadruple �� ����Ƚ��
	is >> data[3]; // quadruple �ְ�����
	is >> data[4]; // network game total
	is >> data[5]; // network win
	is >> data[6]; // network lose
	is >> data[7]; // network draw
	cout << " ----------------Record------------------" << endl;
	cout << "           ID             : " << ID <<  endl;
	cout << "    number of Triple      : " << data[0] << " times" << endl;
	cout << "        best score        : " << data[1] << endl;
	cout << "    number of Qudaruple   : " << data[2] << " times" << endl;
	cout << "        best score        : " << data[3] << endl;
	cout << "  Multi Play(Total/W/L/D) : " << data[4] <<"/"<<data[5]<<"/"<<data[6] <<"/"<< data[7] << endl;

	is.close();
}

void saveRecord(int GameDigits, int checkCount) {  // gamedigit =>  0 �ݽ¸�, 1���й�,2�ݹ��º�, 3 �̱�Ʈ����,4�̱�������� 
	ifstream is;
	is.open(SAVEPATH);
	if (!is.is_open()) {
		cout << "profile not found" << endl;
		return;
	}

	ofstream os;
	int data[8];
	string ID;

	is >> ID;
	for (int i = 0; i < 8; i++) {
		is >> data[i];
	}
	if (GameDigits == 0) {
		data[4]++;
		data[5]++;
	}
	else if (GameDigits == 1) {
		data[4]++;
		data[6]++;
	}
	else if (GameDigits == 2) {
		data[4]++;
		data[7]++;
	}
	else if (GameDigits == 3) {
		data[0]++;
		if (data[1] < 101 - checkCount)
			data[1] = 101 - checkCount;
	}else if (GameDigits == 4) {
		data[2]++;
		if (data[3] < 101 - checkCount)
			data[3] = 101 - checkCount;
	}
	is.close();
	os.open(SAVEPATH);
	os << ID << endl;

	for (int i = 0; i < 8; i++) {
		os << data[i] <<endl;
	}
	os.close();
}

void runGame(int GameDigits) {

	// game start : make a random 4 digits integer using each number once 
	random_device rd;     // only used once to initialise (seed) engine
	mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	uniform_int_distribution<int> uni(0, 9); // guaranteed unbiased

	auto random_integer = uni(rng);
	int answ[10] = { 0,};
	while (IsRepetition(answ, GameDigits)) {
		for (int i = 0; i < GameDigits; i++) {
			answ[i] = uni(rng);
		}
	}

	int checkCount = 0;  // Ȯ���� Ƚ�� ( ���� )

	cout << "**************************************" << endl;
	cout << "             Play Ball                " << endl;
	cout << "**************************************" << endl;

	while (1) { // 4��Ʈ����ũ�� ������ �ʾҴٸ� ���ϸ� �ݺ�
				// �Է¹޾Ƽ� ����   ( �̶� �Է¹��� ���� 4���� �ߺ��� ������ ��õ� �䱸)
				// �Է¹������� �� ( 
				// ��� ���			( 4strike �ϰ�� ���ڸ� ������̹Ƿ� ���� ����)

		string input;
		int output[10];

		do {
			input = getNumber(GameDigits);		// ���ڿ��� �Է¹���, 4�ڸ� ������ �ƴҰ�� ������������ �Ǿ�����
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] -'0';
			}
		} while (IsRepetition(output, GameDigits));	// �����迭�� �ߺ��� ������ �ȵ�

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "ȸ : " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;
		if (compareResult.getStrike() == GameDigits)// ������ ���߸� ������ ����˴ϴ�
			break;
	}

	cout << "**************************************" << endl;
	cout << "*            Correct!!               *" << endl;
	cout << "*            Score : " << 101 - checkCount << "              *" << endl;
	cout << "**************************************" << endl << endl;

	cout << " ������ �����մϴ�" << endl;
	saveRecord(GameDigits, checkCount);
}

int netWorkServer() {
	SOCKET sock, clientsock;
	WSADATA	wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	int strlen;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { // �ʱ�ȭ ����
		cout << "network init fail " << endl; 
		return 1;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		cout << "socket init fail " << endl;
		return 1;
	}

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(1234);
	sockinfo.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) { // bind
		cout << "bind fail " << endl;
		return 1;
	}

	if (listen(sock, 5) == SOCKET_ERROR) { // ���(backlog) 5���� ������ ���
		cout << "��⿭ ����" << endl;
		return 1;
	}

	clientsize = sizeof(clientinfo);
	cout << "waiting client... " << endl;

	//				(��������, Ŭ���̾�Ʈ �ּҰ�, Ŭ���̾�Ʈ �ּҰ� ������)
	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize); // ��⿭�� ��û�� �޾Ƽ� ������� ����
	if (clientsock == INVALID_SOCKET) {
		cout << "client network failed " << endl;
		return 1;
	}
	// ������ ���� ��ũ����, ������ ������������, ������ ����, �Լ��÷���
	//  send(clientsock, message, sizeof(message), 0);

	// Start Game //
	random_device rd;    
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(0, 9);
	int GameDigits = 4;

	auto random_integer = uni(rng);
	int answ[10] = {0,};
	while (IsRepetition(answ, GameDigits)) {
		for (int i = 0; i < GameDigits; i++) {
			answ[i] = uni(rng);
		}
	}

	int checkCount = 0;  // Ȯ���� Ƚ�� ( ���� )
	bool myTurn = true;
	cout << "******************************************l***********************************" << endl;
	cout << "             Play Ball                    l              Away                 " << endl;
	cout << "******************************************l***********************************" << endl;

	while (1) {
		if (clientsock == INVALID_SOCKET) {
			cout << "client network failed " << endl;

			closesocket(sock);
			closesocket(clientsock);

			WSACleanup();
			break;
		}
		string input;
		int output[10];
		char gameData[8];

		do {
			input = getNumber(GameDigits);		// ���ڿ��� �Է¹���, 4�ڸ� ������ �ƴҰ�� ������������ �Ǿ�����
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] - '0';
			}
		} while (IsRepetition(output, GameDigits));	// �����迭�� �ߺ��� ������ �ȵ�

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "ȸ �� : " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;
		if (compareResult.getStrike() == GameDigits)
			checkCount = 0;

		for (int i = 0; i < 4; i++) {
			gameData[i] = output[i] + '0';
		}
		gameData[4] = checkCount + '0';
		gameData[5] = compareResult.getStrike() + '0';
		gameData[6] = compareResult.getBall() + '0';

		myTurn = false;
		send(clientsock, gameData, sizeof(gameData), 0);

		strlen = recv(clientsock, gameData, sizeof(gameData), 0);
		if (strlen == -1) {
			cout << " recieve failed" << endl; 

			closesocket(sock);
			closesocket(clientsock);

			WSACleanup();
			return 1;
		}		
		cout <<"                                           "
			<< gameData[4] << "ȸ �� : " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;
		if (gameData[7] == '1') {
			// ������ �¸� �����ߴ�
			cout << "**************************************" << endl;
			cout << "*             Win                    *" << endl;
			cout << "**************************************" << endl << endl;
			saveRecord(0, 0);
			break;
		}
		else if (gameData[7] == '2') {
			cout << "**************************************" << endl;
			cout << "*             Lose                   *" << endl;
			cout << "**************************************" << endl << endl;
			saveRecord(1, 0);
			break;
		}
		else if (gameData[7] == '3') {
			cout << "**************************************" << endl;
			cout << "*             Draw                   *" << endl;
			cout << "**************************************" << endl << endl;
			saveRecord(2, 0);
			break;
		}
		bool myTurn = true;
	}
	// ���ӳ�
	closesocket(sock);
	closesocket(clientsock);

	WSACleanup();

	return 0;
}

int netWorkClient() {
	// 127.0.0.1 : �ڱ�ȣ�� IP���
	// ��Ʈ�� 1234

	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;

	int strlen; // recv ���ϰ� ����

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		cout << "init failed" << endl;
		return 1;
	}
	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientsock == INVALID_SOCKET) {
		cout << "client socket failed" << endl;
		return 1;
	}

	memset(&sockinfo, 0, sizeof(sockinfo)); // �ּ� ����ü �ʱ�ȭ

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(1234); // ��Ʈ�� 1234 ���
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP �Է¹ޱ�

	if (connect(clientsock,(SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR){
		cout << "server link failed" << endl;
		return 1;
	}

	// ���� ����//
	// Start Game //
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 9);
	int GameDigits = 4;

	auto random_integer = uni(rng);
	int answ[4] = { 0,0,0,0 };
	while (IsRepetition(answ, GameDigits)) {
		for (int i = 0; i < GameDigits; i++) {
			answ[i] = uni(rng);
		}
	}

	int checkCount = 0;  // Ȯ���� Ƚ�� ( ���� )
	bool myTurn = false;
	cout << "******************************************l***********************************" << endl;
	cout << "             Play Ball                    l              Away                 " << endl;
	cout << "******************************************l***********************************" << endl;

	string input;
	int output[10];
	char gameData[8];

	strlen = recv(clientsock, gameData, sizeof(gameData), 0);
	if (strlen == -1) {
		cout << " recieve failed" << endl; 
		return 0;
	}
	cout << "                                           " 
		<< gameData[4] << "ȸ �� : " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;
	myTurn = true;

	while (1) {

		if (clientsock == INVALID_SOCKET) {
			cout << "client network failed " << endl;

			closesocket(clientsock);
			WSACleanup();
			return 1;
		}

		do {
			input = getNumber(GameDigits);		// ���ڿ��� �Է¹���, 4�ڸ� ������ �ƴҰ�� ������������ �Ǿ�����
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] - '0';
			}
		} while (IsRepetition(output, GameDigits));	// �����迭�� �ߺ��� ������ �ȵ�

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "ȸ �� : " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;

		if (compareResult.getStrike() == GameDigits) {
			if (gameData[5] == '4') {  // ������ ���� ȸ���� ������ �������� -> ���º�
				gameData[7] = '3';


				for (int i = 0; i < 4; i++) {
					gameData[i] = output[i] + '0';
				}
				gameData[4] = checkCount + '0';
				gameData[5] = compareResult.getStrike() + '0';
				gameData[6] = compareResult.getBall() + '0';

				send(clientsock, gameData, sizeof(gameData), 0);
				cout << "**************************************" << endl;
				cout << "*             Draw                   *" << endl;
				cout << "**************************************" << endl << endl;
				saveRecord(2, 0);
				break;
			}
			else {  // ���� 4���ε� ������4���� �ƴ� -> �¸�
				gameData[7] = '2';


				for (int i = 0; i < 4; i++) {
					gameData[i] = output[i] + '0';
				}
				gameData[4] = checkCount + '0';
				gameData[5] = compareResult.getStrike() + '0';
				gameData[6] = compareResult.getBall() + '0';

				send(clientsock, gameData, sizeof(gameData), 0);
				cout << "**************************************" << endl;
				cout << "*             Win                    *" << endl;
				cout << "**************************************" << endl << endl;
				saveRecord(0, 0);
				break;
			}
		}
		else if (gameData[5] == '4') { // ���� 4��Ʈ����ũ�� �ƴѵ� ��밡 4������ -> ����
			gameData[7] ='1';

			for (int i = 0; i < 4; i++) {
				gameData[i] = output[i] + '0';
			}
			gameData[4] = checkCount + '0';
			gameData[5] = compareResult.getStrike() + '0';
			gameData[6] = compareResult.getBall() + '0';

			send(clientsock, gameData, sizeof(gameData), 0);

			cout << "**************************************" << endl;
			cout << "*             Lose                   *" << endl;
			cout << "**************************************" << endl << endl;
			saveRecord(1, 0);
			break;
		}
		else { // �ƹ���Ȳ�� �ƴϴ� - >���� ���
			gameData[7] = '0';

			for (int i = 0; i < 4; i++) {
				gameData[i] = output[i] + '0';
			}
			gameData[4] = checkCount + '0';
			gameData[5] = compareResult.getStrike() + '0';
			gameData[6] = compareResult.getBall() + '0';

			send(clientsock, gameData, sizeof(gameData), 0);
			strlen = recv(clientsock, gameData, sizeof(gameData), 0);
			if (strlen == -1) {
				cout << " recieve failed" << endl; 
				closesocket(clientsock);
				WSACleanup();
				return 1;
			}
			cout << "                                           " 
				<< gameData[4] << "ȸ �� : " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;

	  	}
	}
	closesocket(clientsock);
	WSACleanup();

	return 0;
}

int main()
{
	system("mode 80,25");   //Set mode to ensure window does not exceed buffer size
	//SMALL_RECT WinRect = { 0, 0, 80, 25 };   //New dimensions for window in 8x12 pixel chars
	//SMALL_RECT* WinSize = &WinRect;
	//SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, WinSize);   //Set new size for window

	cout << "            Number Baseball Game  "  << endl;
	cout << "                          0.1v" << endl << endl;
	while(1){
		bool ok = false;
		string gamemodeS;
		cout << " ---------------------------------------" << endl;
		cout << "      1. Single Play (Triple)      " << endl;
		cout << "      2. Single Play (Quadruple)   " << endl;
		cout << "      3. Make Multi Play Room (Server)" << endl;
		cout << "      4. Find Multi Play Room (Client)" << endl;
		cout << "      5. Make New Profile          " << endl;
		cout << "      6. View Record               " << endl;
		cout << "      0. exit game                 " << endl;
		cout << " ---------------------------------------" << endl;
		while (!ok)
		{
			cout << "select menu : ";
			cin >> gamemodeS;
			if (gamemodeS.length() == 1)
			{
				bool allDigits = true;
				allDigits = allDigits && (
				(gamemodeS[0] >= '0') &&
				(gamemodeS[0] <= '6')
				);
				ok = allDigits;
			}
		}
		int gamemode = stoi(gamemodeS);
		if (gamemode == 1) {
			cout << endl;
			runGame(3);
		}
		if (gamemode == 2) {
			cout << endl;
			runGame(4);
		}
		else if (gamemode == 3) {
			netWorkServer();
		}
		else if (gamemode == 4) {
			netWorkClient();
		}
		else if (gamemode == 5) {
			makeProfile();
		}
		else if (gamemode == 6) {
			showRecord();
		}
		else if (gamemode == 0) {

			cout << "**************************************" << endl;
			cout << "                Good Bye              " << endl;
			cout << "**************************************" << endl;
			break;
		}
	}
	return 0;
}