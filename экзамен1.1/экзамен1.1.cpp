#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
//Программа работает, но есть проблема с определением буквы(угаданая или нет)
using namespace std;

const int MAX_WRONG = 7;  // максимальное количество ошибок

vector<string> read_words(string filename) {
    vector<string> words;
    ifstream file(filename);
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    }
    return words;
}

string random_word(const vector<string>& words) {
    srand(time(nullptr));
    int index = rand() % words.size();
    return words[index];
}

char ask_guess(const string& missed_letters, const string& correct_letters) {
    char guess;
    bool valid_guess = false;
    while (!valid_guess) {
        cout << "Пропущенные буквы: " << missed_letters << endl;
        cout << "Угаданные буквы: ";
        for (char letter : correct_letters) {
            cout << letter << " ";
        }
        cout << endl;
        cout << "Введите букву: ";
        cin >> guess;
        guess = toupper(guess);  // преобразование в верхний регистр
        if (!isalpha(guess)) {
            cout << "Неверный ввод. Попробуйте еще раз." << endl;
        }
        else {
            valid_guess = true;
        }
    }
    return guess;
}

void show_word(const string& word, const string& guessed_letters) {
    string display_word;
    for (char letter : word) {
        if (guessed_letters.find(letter) != string::npos) {
            display_word += letter;
        }
        else {
            display_word += "-";
        }
    }
    cout << "Загаданное слово: " << display_word << endl;
}

void show_statistic(const string& word, const string& missed_letters, const string& correct_letters, int tries, time_t start_time) {
    time_t end_time = time(nullptr);
    double elapsed_time = difftime(end_time, start_time);
    cout << "Время игры: " << elapsed_time << " секунд" << endl;
    cout << "Количество попыток: " << tries << endl;
    cout << "Загаданное слово: " << word << endl;
    cout << "Пропущенные буквы: " << missed_letters << endl;
    cout << "Угаданные буквы: " << correct_letters << endl;
}

void main() {
    setlocale(LC_ALL, "rus");
    const string FILENAME = "words.txt";
    const vector<string> WORDS = read_words(FILENAME);

    string word = random_word(WORDS);
    string missed_letters;
    string correct_letters;
    int wrong_guesses = 0;
    int tries = 0;
    time_t start_time = time(nullptr);

    cout << "Добро пожаловать в игру \"Виселица\"!" << endl;

    while (wrong_guesses < MAX_WRONG && correct_letters.length() != word.length()) {
        char guess = ask_guess(missed_letters, correct_letters);
        tries++;

        if (missed_letters.find(guess) != string::npos || correct_letters.find(guess) != string::npos) {
            cout << "Вы уже вводили эту букву. Попробуйте другую." << endl;
            continue;
        }
        if (word.find(guess) != string::npos) {
            cout << "Вы угадали букву!" << endl;
            correct_letters += guess;
        }
        else if (word.find(guess) == string::npos) {
            cout << "Эта буква не входит в загаданное слово." << endl;
            missed_letters += guess;
            wrong_guesses++;
        }

        show_word(word, correct_letters);
    }

    if (wrong_guesses == MAX_WRONG) {
        cout << "К сожалению, вы проиграли. Было загадано слово \"" << word << "\"." << endl;
    }
    else {
        cout << "Поздравляем, вы выиграли!" << endl;
    }

    show_statistic(word, missed_letters, correct_letters, tries, start_time);
}
