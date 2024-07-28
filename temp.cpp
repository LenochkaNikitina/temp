#include <iostream>
#include<fstream>
#include<vector>
#include<map>
#include<Windows.h>

using namespace std;

class Randomizer {
    int random;

public:
    Randomizer() {
        srand(time(0));
        random = rand() % 5 + 1;
    }

    int Get_random() {
        return random;
    }

};

class IWord_read {
public:
    virtual void Read() = 0;

    virtual string Get_word() = 0;
};

class Word_from_file :public IWord_read {
    Randomizer rand;
    string word;
public:
    Word_from_file() {}

    void Read() override {
        ifstream In;
        In.open("Words.txt");

        int size = rand.Get_random();

        for (int i = 0; i < size; i++) {
            In >> word;
        }

        In.close();
    }

    string Get_word() override {
        Read();
        return word;
    }
};

class Decoding {
public:
    string Decode(string word) {
        for (int i = 0; i < word.length()-1; i+=2)
            swap(word[i], word[i + 1]);

        reverse(word.begin(), word.end());

        return word;
    }
};

class Statistics {
    vector<char> letters;
public:
    static int tries_count;
    void Add_letter(char letter) {
        letters.push_back(letter);
    }

    void Print_letters() {
        for (int i = 0; i < letters.size(); i++)
            cout << letters[i] << ' ';
        cout << endl;
    }
};

int Statistics::tries_count;

class Ask_letter {
    char letter;
public:
    void Ask() {
        cout << "Ваша буква?" << endl;
        cin >> letter;
    }
};

class Guess_word {
    string word;
    char letter;
    int size;

public:
    static map<int, char> letter_pos;

    Guess_word() {}
    Guess_word(string word) {
        this->word = word;
        size = word.length();
    }

    int Get_size() {
        return size;
    }

    char Get_letter() {
        return letter;
    }

    map<int, char> Get_lettets_pos() {
        return letter_pos;
    }

    void Letter_position() {
        for (int i = 0; i < size; i++) {
            if (word[i] == letter)
                letter_pos[i] = letter;
        }
    }
    
    void Ask_letter() {
        cout << "Ваша буква?" << endl;
        cin >> letter;
    }
 
    string Ask_whole_word() {
        cout << "Готовы назвать всё слово?(Напечатайте слово, либо напечатайте\"нет\")" << endl;
        string answer;
        cin >> answer;

        return answer;
    }
    
    bool Check_word(string answer) {
        return answer == word;
    }
};

    map<int, char> Guess_word::letter_pos;

    class Show_word_progress  {
        Guess_word guess_word;
    public:
        Show_word_progress(Guess_word guess_word) {
            this->guess_word = guess_word;
        }
        void Show()  {
            system("cls");

            for (int i = 0; i < guess_word.Get_size(); i++) {
                if (Guess_word::letter_pos.contains(i))
                    cout << Guess_word::letter_pos[i];
                else
                    cout << '-';
            }
            cout << endl;
        }
    };

int main() {
    setlocale(0, "rus");
    SetConsoleCP(1251);

    IWord_read* word = new Word_from_file;
    Statistics statistics;
    Decoding decoding;

    Guess_word guess_word(decoding.Decode( word->Get_word()));

    Show_word_progress show(guess_word);
    show.Show();
    
    int map_size = guess_word.letter_pos.size();
    statistics.tries_count = 0;
    //IShow* hangman = new Show_hangman;

    while (guess_word.letter_pos.size() < guess_word.Get_size()) {
        guess_word.Ask_letter();

        statistics.tries_count++;
        statistics.Add_letter(guess_word.Get_letter());

        guess_word.Letter_position();
        show.Show();

        if (map_size < guess_word.letter_pos.size()) {
            if (guess_word.Check_word(guess_word.Ask_whole_word()))
                break;
            map_size = guess_word.letter_pos.size();
            show.Show();
        }

        //hangman->Show();
    }

    cout << "Вы отгадали с " << statistics.tries_count << " попыток" << endl;
    cout << "Ваши буквы:" << endl;
    statistics.Print_letters();

    return 0;
}

