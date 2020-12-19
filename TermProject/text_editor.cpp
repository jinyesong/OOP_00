#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <cstdlib>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

vector<vector<string>> vec;
int thisPageIndex = 0;
string consoleMessage = "";
string input;

string printString(int index) { //���ڷ� ���� �ε��� �� ���� ���
	string input;
	int count = 1;
	for (int i = index; i < index+20; i++) {
		if (count < 10) {
			cout << " " << count << "| ";
		}
		else {
			cout << count << "| ";
		}
		for (int j = 0; j < vec[i].size(); j++) {
			cout << vec[i][j] + " ";
		}
		cout << "\n";
		count++;
	}
	cout << "---------------------------------------------------------------------------" << endl; //75byte
	cout << "n:����������, p:����������, i:����, d:����, c:����, s:ã��, t:����������" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	if (consoleMessage == "") {
		cout << "(�ָܼ޽���)" << endl;
	}
	else {
		cout << consoleMessage << endl;
		consoleMessage = "";
	}
	cout << "---------------------------------------------------------------------------" << endl;
	cout << "�Է�: ";
	getline(cin, input);
	cout << "---------------------------------------------------------------------------" << endl;

	thisPageIndex = index;
	return input;
}

void reConstruct() {
	vector<vector<string>> newVec;
	int byte = 0;
	vector<string> innervec;
	for (int i = 0; i < vec.size(); i++) {
		for (int j = 0; j < vec[i].size(); j++) {
			const char* word = vec[i][j].c_str();
			int length = strlen(word);
			byte += length;
			if (byte <= 71) {
				innervec.push_back(word);
				byte += 1; //����
			}
			else {
				newVec.push_back(innervec);
				innervec.clear();
				byte = length + 1;
				innervec.push_back(word);
			}
		}
	}
	newVec.push_back(innervec);
	vec = newVec;
}

class EditStrategy {
public:
	virtual void edit() = 0;
	virtual void setArg(vector<string> argvec){}
};

class NextPage : public EditStrategy {
public:
	void edit() {
		int nextPageIndex = thisPageIndex + 20; //���������� ����

		if (nextPageIndex == vec.size()) { //������ ������
			consoleMessage = "This is the last page!";
			input = printString(thisPageIndex);
		}
		else if (nextPageIndex < vec.size() && nextPageIndex + 20 > vec.size()) { //������ ������ 20�� �̸�
			int line = 0;
			while (nextPageIndex < vec.size()) { //������ �ٱ��� �� �� ���Ҵ���
				nextPageIndex++;
				line++;
			}
			input = printString(thisPageIndex + line);
		}
		else {
			input = printString(nextPageIndex);
		}
	}
};

class PrePage : public EditStrategy {
public:
	void edit() {
		if (thisPageIndex == 0) {
			consoleMessage = "This is the first page!";
			input = printString(0);
		}
		else if (0 < thisPageIndex && thisPageIndex - 20 < 0) {
			input = printString(0);
		}
		else {
			input = printString(thisPageIndex-20);
		}
	}
};

class Insert : public EditStrategy {
public:
	vector<string> argvec;

	void setArg(vector<string> input) {
		argvec = input;
	}
	void edit() {
		int line = atoi(argvec[0].c_str());
		int wordIndex = atoi(argvec[1].c_str());
		string word = argvec[2];

		if (vec[thisPageIndex + line - 1].size() < wordIndex) { //���� �˻���� n��° �ܾ� �������� ����
			consoleMessage = "�ش� ��ġ�� �ܾ �������� �ʽ��ϴ�.";
			input = printString(thisPageIndex);
		}
		else {
			vec[thisPageIndex + line - 1].insert(vec[thisPageIndex + line - 1].begin() + wordIndex, word);
			reConstruct();
			input = printString(thisPageIndex);
		}
	}
};

class Delete : public EditStrategy {
public:
	vector<string> argvec;

	void setArg(vector<string> input) {
		argvec = input;
	}
	void edit() {
		int line = atoi(argvec[0].c_str());
		int wordIndex = atoi(argvec[1].c_str());

		if (vec[thisPageIndex + line - 1].size() < wordIndex) { //���� �˻���� n��° �ܾ� �������� ����
			consoleMessage = "�ش� ��ġ�� �ܾ �������� �ʽ��ϴ�.";
			input = printString(thisPageIndex);
		}
		else {
			vec[thisPageIndex + line - 1].erase(vec[thisPageIndex + line - 1].begin() + wordIndex - 1);
			reConstruct();
			input = printString(thisPageIndex);
		}
	}
};

class Change : public EditStrategy {
public:
	vector<string> argvec;

	void setArg(vector<string> input) {
		argvec = input;
	}
	void edit() {
		string oldWord = argvec[0];
		string newWord = argvec[1];

		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				if (oldWord == vec[i][j]) {
					vec[i].erase(vec[i].begin() + j);
					vec[i].insert(vec[i].begin() + j, newWord);
				}
			}
		}
		reConstruct();
		input = printString(thisPageIndex);
	}
};

class Search : public EditStrategy {
public:
	vector<string> argvec;

	void setArg(vector<string> input) {
		argvec = input;
	}
	void edit() {
		string searchWord = argvec[0];
		bool find = false;

		for (int i = 0; i < vec.size(); i++) {
			for (int j = 0; j < vec[i].size(); j++) {
				if (searchWord == vec[i][j]) {
					input = printString(i);
					find = true;
					break;
				}
			}
			if (find) {
				break;
			}
		}
		if (!find) {
			consoleMessage = "�ܾ ã�� ���߽��ϴ�.";
			input = printString(thisPageIndex);
		}
	}
};

class Terminate : public EditStrategy {
public:
	void edit() {
		ofstream file("test.txt");

		if (file.is_open()) {
			for (int i = 0; i < vec.size(); i++) {
				for (int j = 0; j < vec[i].size(); j++) {
					file << vec[i][j] << " ";
				}
			}
		}
	}
};

class User {
protected:
	EditStrategy* editStrategy;
	vector<string> argvec;
public:
	User(EditStrategy* strategy) {
		editStrategy = strategy;
	}
	User(EditStrategy* strategy, vector<string> input) {
		editStrategy = strategy;
		argvec = input;
	}
	void setArg() {
		editStrategy->setArg(argvec);
	}
	virtual void edit() {
		editStrategy->edit();
	}
};

int main() {
	//���� �о�ͼ� ������ ���� ����
	ifstream file;
	file.open("test.txt");
	char word[20];
	int byte = 0;
	vector<string> innervec;
	while (!file.eof()) {
		file >> word;
		int length = strlen(word);
		byte += length;
		if (byte <= 71) {
			innervec.push_back(word);
			byte += 1; //����
		}
		else {
			vec.push_back(innervec);
			innervec.clear();
			byte = length + 1;
			innervec.push_back(word);
		}
	}
	vec.push_back(innervec);
	input = printString(0);

	while (1) {
		string check = input.substr(0, 1);
		string arg;
		vector<string> argvec;
		if (input.length() > 3) {
			arg = input.substr(2, input.length() - 3); //��ȣ ���� ���ڵ�
			char ch[100];
			strcpy(ch, arg.c_str());
			char* token = strtok(ch, ",");
			while (token != NULL) {
				argvec.push_back(token);
				token = strtok(NULL, ",");
			}
		}
		bool isError = false;

		if (check == "n") {
			EditStrategy* nextPage = new NextPage();
			User* user = new User(nextPage);
			user->edit();
		}
		else if (check == "p") {
			EditStrategy* prePage = new PrePage();
			User* user = new User(prePage);
			user->edit();
		}
		else if (check == "i") {
			if (input.substr(1,1)!="(" && input.substr(input.length()-2, 1)!=")") { //���������� �Է�
				consoleMessage = "�߸��� �Է��Դϴ�.";
				isError = true;
			}
			for (char const& c : argvec[0]) { //Ư������ �Է�
				if (!isdigit(c) && c != char(" ")) {
					consoleMessage = "Ư�����ڸ� �����ϰ� �ֽ��ϴ�.";
					isError = true;
					break;
				}
			}
			for (char const& c : argvec[1]) { //Ư������ �Է�
				if (!isdigit(c)) {
					consoleMessage = "Ư�����ڸ� �����ϰ� �ֽ��ϴ�.";
					isError = true;
					break;
				}
			}
			if (argvec[2].length() >= 71) { //75����Ʈ �̻� �Է�
				consoleMessage = "���ڼ��� �ʹ� �����ϴ�.";
				isError = true;
			}
			if (arg.find(" ") != string::npos) { //��ȣ�ȿ� ������ ���Ե� ���
				consoleMessage = "��ȣ �ȿ� ������ ���ԵǾ��ֽ��ϴ�.";
				isError = true;
			}
			int line = atoi(argvec[0].c_str());
			if (line < 1 || line > 20) {
				consoleMessage = "������ �������� �ʽ��ϴ�.";
				isError = true;
			}

			if (isError) {
				input = printString(thisPageIndex);
			}
			else {
				EditStrategy* insert = new Insert();
				User* user = new User(insert, argvec);
				user->setArg();
				user->edit();
			}
		}
		else if (check == "d") {
			if (input.substr(1, 1) != "(" && input.substr(input.length() - 2, 1) != ")") { //���������� �Է�
				consoleMessage = "�߸��� �Է��Դϴ�.";
				isError = true;
			}
			if (arg.find(" ") != string::npos) { //��ȣ�ȿ� ������ ���Ե� ���
				consoleMessage = "��ȣ �ȿ� ������ ���ԵǾ��ֽ��ϴ�.";
				isError = true;
			}
			int line = atoi(argvec[0].c_str());
			if (line < 1 || line > 20) {
				consoleMessage = "������ �������� �ʽ��ϴ�.";
				isError = true;
			}

			if (isError) {
				input = printString(thisPageIndex);
			}
			else {
				EditStrategy* deleteStrategy = new Delete();
				User* user = new User(deleteStrategy, argvec);
				user->setArg();
				user->edit();
			}
		}
		else if (check == "c") {
			if (input.substr(1, 1) != "(" && input.substr(input.length() - 2, 1) != ")") { //���������� �Է�
				consoleMessage = "�߸��� �Է��Դϴ�.";
				isError = true;
			}
			if (arg.find(" ") != string::npos) { //��ȣ�ȿ� ������ ���Ե� ���
				consoleMessage = "��ȣ �ȿ� ������ ���ԵǾ��ֽ��ϴ�.";
				isError = true;
			}

			if (isError) {
				input = printString(thisPageIndex);
			}
			else {
				EditStrategy* change = new Change();
				User* user = new User(change, argvec);
				user->setArg();
				user->edit();
			}
		}
		else if (check == "s") {
			if (input.substr(1, 1) != "(" && input.substr(input.length() - 2, 1) != ")") { //���������� �Է�
				consoleMessage = "�߸��� �Է��Դϴ�.";
				isError = true;
			}
			if (arg.find(" ") != string::npos) { //��ȣ�ȿ� ������ ���Ե� ���
				consoleMessage = "��ȣ �ȿ� ������ ���ԵǾ��ֽ��ϴ�.";
				isError = true;
			}

			if (isError) {
				input = printString(thisPageIndex);
			}
			else {
				EditStrategy* search = new Search();
				User* user = new User(search, argvec);
				user->setArg();
				user->edit();
			}
		}
		else if (check == "t") {
			EditStrategy* terminate = new Terminate();
			User* user = new User(terminate);
			user->edit();
			file.close();
			break;
		}
		else {
			consoleMessage = "�߸��� �Է��Դϴ�.";
			input = printString(thisPageIndex);
		}
	}
	return 0;
}