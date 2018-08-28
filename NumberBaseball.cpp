#include <iostream>	
#include <string>		// using String for input
#include <random>		// for getting random digit
#include <fstream>		// for saving records
#include <WinSock2.h>	// for socket networking
						// Configuration Properties => Linker => Input => Additional dependencies -> add 'ws2_32.lib'
						// 프로젝트 속성 -> 링커-> 입력 -> 추가종속성에 ws2_32.lib

#pragma warning(disable:4996) // for ignore LinkWanring 'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS 

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

checkC compare(int answ[], int guess[], int GameDigits) {
	checkC check(0, 0);

	for (int i = 0; i < GameDigits; i++) {
		for (int j = 0; j < GameDigits; j++) {
			if (answ[i] == guess[j]) {	// Find Same Digit
				if (i == j)				// If same Position strike++ else, ball++
					check.strike++;
				else
					check.ball++;
			}
		}
	}
	return check;
}

string getNumber(int GameDigits){	// Get only N-digits integer(by string) and return
	bool ok = false;				
	string result;
	while (!ok){
		cout << "중복없는 " << GameDigits << "자리 숫자를 입력해주세요 : "; // "please input (GameDigits)-digit integer without repetition"
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

bool IsRepetition(int arr[], int GameDigits) {		// check repetition of integer array 
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
	else if (ID.length() > 20) {
		cout << " System : Id too long(plz under 20)" << endl;
	}
	os.open(SAVEPATH, 'w');
	os << ID << endl;
	for (int i = 0; i < 8; i++) {
		os << 0 << endl;
	}
	os.close();
}

void showRecord() {

	ifstream is;
	is.open(SAVEPATH);
	if (!is.is_open()) {
		cout << "record file Not found" << endl;
		return;
	}

	ofstream os;
	int data[8];
	string ID;
	int length = ID.length();

	is >> ID; // ID
	for (int i = 0; i < 8; i++) {
		is >> data[i];
	}
	cout << " ----------------Record------------------" << endl;
	cout << "           ID             : " << ID << endl;
	cout << "    number of Triple      : " << data[0] << " times" << endl;
	cout << "        best score        : " << data[1] << endl;
	cout << "    number of Qudaruple   : " << data[2] << " times" << endl;
	cout << "        best score        : " << data[3] << endl;
	cout << "  Multi Play(Total/W/L/D) : " << data[4] << "/" << data[5] << "/" << data[6] << "/" << data[7] << endl;

	is.close();
}

void saveRecord(int GameDigits, int checkCount) {  // gamedigit =>  0 net win, 1 net lose, 2 net draw, 3 single trip, 4 single quad 
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
	}
	else if (GameDigits == 4) {
		data[2]++;
		if (data[3] < 101 - checkCount)
			data[3] = 101 - checkCount;
	}
	is.close();
	os.open(SAVEPATH);
	os << ID << endl;

	for (int i = 0; i < 8; i++) {
		os << data[i] << endl;
	}
	os.close();
	cout << " record saved " << endl;
}

void makeRandNumber(int * target, int GameDigits) {
	random_device rd;     // only used once to initialise (seed) engine
	mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	uniform_int_distribution<int> uni(0, 9); // guaranteed unbiased

	auto random_integer = uni(rng);

	while (IsRepetition(target, GameDigits)) { // number must be non-Repeating digits for NumberBaseballGame
		for (int i = 0; i < GameDigits; i++) {
			target[i] = uni(rng);
		}
	}
}

void runGame(int GameDigits) {

	int answ[10] = { 0, };
	makeRandNumber(answ, GameDigits);
	int checkCount = 0; 

	cout << "**************************************" << endl;
	cout << "             Play Ball                " << endl;
	cout << "**************************************" << endl;

	for (int i = 0; i < GameDigits; i++) {
		cout << answ[i];
	}cout << endl;
	while (1) { // loop until strike == (GameDigits)
		string input;
		int output[10];

		do {
			input = getNumber(GameDigits);		// Get string, If not (GameDigits)-digit integer, wait input again
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] - '0';
			}
		} while (IsRepetition(output, GameDigits));	// there should be no repetition 

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "▼: " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;
		if (compareResult.getStrike() == GameDigits) // game end
			break;
	}

	cout << "**************************************" << endl;
	cout << "*            Correct!!               *" << endl;
	cout << "*            Score : " << 101 - checkCount << "              *" << endl;
	cout << "**************************************" << endl << endl;

	saveRecord(GameDigits, checkCount);
}

int netWorkServer() {
	SOCKET sock, clientsock;
	WSADATA	wsa;
	struct sockaddr_in sockinfo, clientinfo;
	int clientsize;
	int strlen;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
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

	if (bind(sock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) {
		cout << "bind fail " << endl;
		return 1;
	}

	if (listen(sock, 5) == SOCKET_ERROR) {
		cout << "listen fail" << endl;
		return 1;
	}

	clientsize = sizeof(clientinfo);
	cout << "waiting client... " << endl;

	clientsock = accept(sock, (SOCKADDR*)&clientinfo, &clientsize);
	if (clientsock == INVALID_SOCKET) {
		cout << "Invalid client socket " << endl;
		return 1;
	}

	// get ID
	ifstream is;
	char myID[20];
	char enemyID[20];

	is.open(SAVEPATH);
	if (!is.is_open()) {
		cout << "profile not found" << endl;
		strcpy(myID, "Guest");
	}
	else {
		is >> myID;
		is.close();
	}
	send(clientsock, myID, sizeof(myID), 0);
	strlen = recv(clientsock, enemyID, sizeof(enemyID), 0);

	// Start Game //

	int GameDigits = 4;		// network game is Quadruple
	string input;			// input from std input
	int output[10];			// output parsed from input
	char gameData[8];		// data for socket send message
							// index : [0~3] player's guess, [4]th try, [5] strike, [6] ball, [7]gameStatus
	int checkCount = 0;		// try
	int answ[10] = { 0, };
	makeRandNumber(answ, GameDigits);
	
	cout << "******************************************l***********************************" << endl;
	cout << "             " << myID << "                                   " << enemyID << "                 " << endl;
	cout << "******************************************l***********************************" << endl;

	while (1) {
		if (clientsock == INVALID_SOCKET) { // check network status
			cout << "client network failed " << endl;
			closesocket(sock);
			closesocket(clientsock);
			WSACleanup();
			break;
		}

		do {
			input = getNumber(GameDigits);
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] - '0';
			}
		} while (IsRepetition(output, GameDigits));

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "▲: " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;
		if (compareResult.getStrike() == GameDigits)
			gameData[7] = '1';	// gameStatus '1' for "Client win"

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
			closesocket(sock);
			closesocket(clientsock);
			WSACleanup();
			return 1;
		}
		cout << "                                           "
			<< int(gameData[4] - '0') << "▼: " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;
		if (gameData[7] == '1') {
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
	}
	closesocket(sock);
	closesocket(clientsock);
	WSACleanup();
	return 0;
}

int netWorkClient() {

	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	int strlen;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		cout << "init failed" << endl;
		return 1;
	}
	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientsock == INVALID_SOCKET) {
		cout << "Invalid client socket" << endl;
		return 1;
	}

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(1234); // using port "1234"
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1"); // set Server IP (127.0.0.1 for test)
 
	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR) {
		cout << "connect failed" << endl;
		return 1;
	}

	// get ID
	ifstream is;
	char myID[20];
	char enemyID[20];

	is.open(SAVEPATH);
	if (!is.is_open()) {
		cout << "profile not found" << endl;
		strcpy(myID, "Guest");
	}
	else {
		is >> myID;
		is.close();
	}

	strlen = recv(clientsock, enemyID, sizeof(enemyID), 0);
	send(clientsock, myID, sizeof(myID), 0);

	// Start Game //
	int GameDigits = 4;
	int checkCount = 0;
	string input;
	int output[10];
	char gameData[8];
	int answ[4] = { 0,0,0,0 };
	makeRandNumber(answ, GameDigits);
	
	cout << "******************************************l***********************************" << endl;
	cout << "             " << myID << "                                   " << enemyID << "                 " << endl;
	cout << "******************************************l***********************************" << endl;

	strlen = recv(clientsock, gameData, sizeof(gameData), 0);
	if (strlen == -1) {
		cout << " recieve failed" << endl;
		return 1;
	}
	cout << "                                           "
		<< int(gameData[4] - '0') << "▲: " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;

	while (1) {

		if (clientsock == INVALID_SOCKET) {
			cout << "client network failed " << endl;

			closesocket(clientsock);
			WSACleanup();
			return 1;
		}

		do {
			input = getNumber(GameDigits);
			for (int i = 0; i < GameDigits; i++) {
				output[i] = input[i] - '0';
			}
		} while (IsRepetition(output, GameDigits));

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++checkCount << "▼: " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;

		if (compareResult.getStrike() == GameDigits) {	// Client end the Game
			if (gameData[5] == '4') {  // Server also ended the game -> Draw
				for (int i = 0; i < 4; i++) {
					gameData[i] = output[i] + '0';
				}
				gameData[4] = checkCount + '0';
				gameData[5] = compareResult.getStrike() + '0';
				gameData[6] = compareResult.getBall() + '0';
				gameData[7] = '3'; // GameData[7] for transfer "draw" to Server

				send(clientsock, gameData, sizeof(gameData), 0);
				cout << "**************************************" << endl;
				cout << "*             Draw                   *" << endl;
				cout << "**************************************" << endl << endl;
				saveRecord(2, 0);
				break;
			}
			else {  // Server did not finished -> Client Win
				for (int i = 0; i < 4; i++) {
					gameData[i] = output[i] + '0';
				}
				gameData[4] = checkCount + '0';
				gameData[5] = compareResult.getStrike() + '0';
				gameData[6] = compareResult.getBall() + '0';
				gameData[7] = '2'; // GameData[7] for transfer "Client Win" to Server

				send(clientsock, gameData, sizeof(gameData), 0);
				cout << "**************************************" << endl;
				cout << "*             Win                    *" << endl;
				cout << "**************************************" << endl << endl;
				saveRecord(0, 0);
				break;
			}
		}
		else if (gameData[5] == '4') { // Server ended Game but client yet -> "Client Lose"
			for (int i = 0; i < 4; i++) {
				gameData[i] = output[i] + '0';
			}
			gameData[4] = checkCount + '0';
			gameData[5] = compareResult.getStrike() + '0';
			gameData[6] = compareResult.getBall() + '0';
			gameData[7] = '1'; // GameData[7] for transfer "Client Lose" to Server

			send(clientsock, gameData, sizeof(gameData), 0);
			cout << "**************************************" << endl;
			cout << "*             Lose                   *" << endl;
			cout << "**************************************" << endl << endl;
			saveRecord(1, 0);
			break;
		}
		else { // no Situation 
			for (int i = 0; i < 4; i++) {
				gameData[i] = output[i] + '0';
			}
			gameData[4] = checkCount + '0';
			gameData[5] = compareResult.getStrike() + '0';
			gameData[6] = compareResult.getBall() + '0';
			gameData[7] = '0'; // GameData[7] for transfer "keep going on" to Server

			send(clientsock, gameData, sizeof(gameData), 0);
			strlen = recv(clientsock, gameData, sizeof(gameData), 0);
			if (strlen == -1) {
				cout << " recieve failed" << endl;
				closesocket(clientsock);
				WSACleanup();
				return 1;
			}
			cout << "                                           "
				<< int(gameData[4]-'0') << "▲: " << gameData[0] << gameData[1] << gameData[2] << gameData[3] << " (Strike :" << gameData[5] << "  Ball :" << gameData[6] << ")" << endl;
		}
	}
	closesocket(clientsock);
	WSACleanup();

	return 0;
}

int main()
{
	cout << "            Number Baseball Game  " << endl;
	cout << "                          0.2v" << endl << endl;
	while (1) {
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
