#include <iostream>	
#include <string>	// ���ڿ��� ���� �Է¹ޱ�
#include <random>	// ���� ����
#include <fstream>	// ���� ������ ����

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

void parsing(int source, int target[], int GameDigits) {
	if (GameDigits == 4) {
		target[0] = source / 1000;
		source = source % 1000;
		target[1] = source / 100;
		source = source % 100;
		target[2] = source / 10;
		source = source % 10;
		target[3] = source;
	}
	else {
		target[0] = source / 100;
		source = source % 100;
		target[1] = source / 10;
		source = source % 10;
		target[2] = source;
	}
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
	os << 0 << endl;
	os << 0 << endl;
	os << 0 << endl;
	os << 0 << endl;
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
	int data[4];
	string ID;

	is >> ID; // ID
	is >> data[0]; // triple �� ����Ƚ��
	is >> data[1]; // triple �ְ�����
	is >> data[2]; // quadruple �� ����Ƚ��
	is >> data[3]; // quadruple �ְ�����

	cout << " ----------------------------------" << endl;
	cout << "    Record       : " << ID << endl;
	cout << "    Triple       : " << data[0]  << endl;
	cout << "      best score : " << data[1] << endl;
	cout << "    Qudaruple    : " << data[2] << endl;
	cout << "      best score : " << data[3] << endl;
	
	is.close();
}

void runGame(int GameDigits) {

	// game start : make a random 4 digits integer using each number once 

	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, 9); // guaranteed unbiased

	auto random_integer = uni(rng);
	int answ[4] = { 0,0,0,0 };
	while (IsRepetition(answ, GameDigits)) {
		for (int i = 0; i < GameDigits; i++) {
			answ[i] = uni(rng);
		}
	}
	/* ���� ���
	for (int i = 0; i < GameDigits; i++) {
	cout << answ[i];
	}
	cout << endl;
	*/

	int check = 0;  // Ȯ���� Ƚ�� ( ���� )

	cout << "**************************************" << endl;
	cout << "             Play Ball                " << endl;
	cout << "**************************************" << endl;

	while (1) { // 4��Ʈ����ũ�� ������ �ʾҴٸ� ���ϸ� �ݺ�
				// �Է¹޾Ƽ� ����   ( �̶� �Է¹��� ���� 4���� �ߺ��� ������ ��õ� �䱸)
				// �Է¹������� �� ( 
				// ��� ���			( 4strike �ϰ�� ���ڸ� ������̹Ƿ� ���� ����)

		string input;
		int int_input;
		int output[4];

		do {
			input = getNumber(GameDigits);		// ���ڿ��� �Է¹���, 4�ڸ� ������ �ƴҰ�� ������������ �Ǿ�����
			int_input = stoi(input);	// ���ڿ��� ������ ������
			parsing(int_input, output, GameDigits);	// GameDigits �ڸ����� ����(int_input)�� ���ڸ��� ���� �迭(output)�� ����
		} while (IsRepetition(output, GameDigits));	// �����迭�� �ߺ��� ������ �ȵ�

		checkC compareResult = compare(answ, output, GameDigits);
		cout << ++check << "ȸ : " << input << " (Strike :" << compareResult.getStrike() << "  Ball :" << compareResult.getBall() << ")" << endl;
		if (compareResult.getStrike() == GameDigits)// ������ ���߸� ������ ����˴ϴ�
			break;
	}

	cout << "**************************************" << endl;
	cout << "*            Corretct!!              *" << endl;
	cout << "*            Score : " << 101 - check << "              *" << endl;
	cout << "**************************************" << endl << endl;

	cout << " ������ �����մϴ�" << endl;
	// ���Ϸ� �Ѱ���Ƚ��, �ְ����� �����ؼ� �޴����� Ȯ�ΰ����ϰ� ������
	
	ifstream is;
	is.open(SAVEPATH);
	if (!is.is_open()) {
		cout << "record file Not found" << endl;
		return;
	}

	ofstream os;
	int data[4];
	string ID;

	is >> ID;
	is >> data[0]; // triple �� ����Ƚ��
	is >> data[1]; // triple �ְ�����
	is >> data[2]; // quadruple �� ����Ƚ��
	is >> data[3]; // quadruple �ְ�����

	if (GameDigits == 3) {
		data[0]++;
		if (data[1] < 101 - check)
			data[1] = 101 - check;
	}else if (GameDigits == 4) {
		data[2]++;
		if (data[3] < 101 - check)
			data[3] = 101 - check;
	}
	is.close();
	os.open(SAVEPATH);
	os << ID << endl;
	os << data[0] << endl;
	os << data[1] << endl;
	os << data[2] << endl;
	os << data[3] << endl;
	os.close();

}

int main()
{
	cout << "            Number Baseball Game  "  << endl;
	cout << "                          0.1v" << endl << endl;
	while(1){
		bool ok = false;
		string gamemodeS;
		cout << " ----------------------------------" << endl;
		cout << "    1. Single Play (Triple)      " << endl;
		cout << "    2. Single Play (Quadruple)   " << endl;
		cout << "    3. Multi Play  (Working on)  " << endl;
		cout << "    4. Make New Profile          " << endl;
		cout << "    5. View Record               " << endl;
		cout << "    0. exit game                 " << endl;
		cout << " ----------------------------------" << endl;
		while (!ok)
		{
			cout << "select menu : ";
			cin >> gamemodeS;
			if (gamemodeS.length() == 1)
			{
				bool allDigits = true;
				allDigits = allDigits && (
				(gamemodeS[0] >= '0') &&
				(gamemodeS[0] <= '5')
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
			cout << " Sorry I'm working on it "<<endl;
		}
		else if (gamemode == 4) {
			makeProfile();
		}
		else if (gamemode == 5) {
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