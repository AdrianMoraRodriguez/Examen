/**
 * @file Preguntas.cc
 * @brief 
 * @version 0.1
 * @date 2023-04-17
 * 
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

double exam_rest = 0;

struct Question {
    string text;
    vector<string> options;
    char answer;
    bool is_verified;
};

// Función que lee las preguntas de un fichero y las almacena en un vector, las preguntas empiezan por un número y un punto y finalizan con un punto y un espacio
void read_questions(vector<Question> &questions, const string &filename) {
    ifstream file(filename);
    bool is_true = false;
    bool is_first = true;
    if (!file) {
        cerr << "Error: no se pudo abrir el fichero " << filename << endl;
        exit(EXIT_FAILURE);
    }
    string line;
    Question question;
    string aux{};
    while (getline(file, line)) {
        //comprobamos si la linea empieza por un número y un punto, si es así metemos en una string auxiliar las líneas hasta que se encuentre una línea que empiece por letra con paréntesis
        if (line[0] >= '0' && line[0] <= '9') {
            if (is_first) {
                is_first = false;
            } else {
                if (question.answer == ' ') {
                    std::cout << "La pregunta: " << question.text << "No tiene respuesta definida\n";
                }
                questions.push_back(question);
                question.text = "";
                question.options.clear();
                question.answer = ' ';
                question.is_verified = false;
            }
            aux = line;
            while (getline(file, line)) {
                if (line[1] != ')') {
                    aux = aux + "\n" + line ;
                } else {
                    question.text = aux;
                    aux = line;
                    break;
                }
            }
        } 
        if (line[1] == ')') {
            aux = line;
                if (line[1] != ')') {
                    aux += line;
                } else {
                    //si en aux está (Verificado) o (Verificadisimo) es que la respuesta es correcta y se borra el paréntesis y el texto que hay dentro
                    if (aux.find("(Verificado)") != string::npos) {
                        aux.erase(aux.find("(Verificado)"), 13);
                        is_true = true;
                    } else if (aux.find("(Verificadisimo)") != string::npos) {
                        aux.erase(aux.find("(Verificadisimo)"), 17);
                        is_true = true;
                    }
                    if (is_true) {
                        question.answer = aux[0];
                        is_true = false;
                    }
                    question.options.push_back(aux);
                    aux = line;
                }
        }
    }
}

int bad = 0;
// Función que muestra una pregunta y sus opciones
void show_question(const Question &question) {
    cout << question.text << endl;
    for (int i = 0; i < question.options.size(); i++) {
        cout << question.options[i] << endl;
    }
}

// Función que comprueba si la respuesta es correcta
bool check_answer(const Question &question, char answer) {
    return question.answer == answer;
}

// Función que muestra el resultado de la pregunta
void show_result(const Question &question, char answer) {
    if (check_answer(question, answer)) {
        //pon correcto en verde
        cout << "\033[1;32mCorrecto\033[0m" << endl;
    } else {
        //pon incorrecto en rojo
        cout << "\033[1;31mIncorrecto\033[0m" << endl;
        cout << "La respuesta correcta es la " << question.answer << endl;
        bad++;
    }
}
double n;
double restar = 0;
bool exam;
// Función que muestra el resultado de todas las preguntas
void show_results(const vector<Question> &questions) {
    double correct = 0;
    for (int i = 0; i < questions.size(); i++) {
        if (questions[i].is_verified) {
            if (check_answer(questions[i], questions[i].answer)) {
                correct++;
            }
        }
    }
    cout << "Has acertado " << correct << " de " << n << endl;
    //si la nota es menor que 5, ponla en rojo si es igual o mayor ponla en verde
    if (exam) {
      if (exam_rest > 17) {
        cout << "\033[1;31mNo llega al mínimo de preguntas, usted ha tenido " <<  correct << " preguntas correctas, se esperaban 33" <<"\033[0m" << endl;
      } else {
        cout << "\033[1;32mNota final " << correct / n << "\033[0m" << endl;
      }
    } else if ((correct - restar) * 10 / n < 5) {
        cout << "\033[1;31mNota final " << (correct - restar) * 10 / n << "\033[0m" << endl;
    } else {
        cout << "\033[1;32mNota final " << (correct - restar) * 10 / n << "\033[0m" << endl;
    }
}

// Función que mezcla las preguntas
void shuffle_questions(vector<Question> &questions) {
    srand(time(NULL));
    random_shuffle(questions.begin(), questions.end());
}

int main() {
    bool has_bad_questions = false;
    vector<Question> questions;
    read_questions(questions, "preguntas.txt");
    shuffle_questions(questions);
    cout << "Quiere entrar en modo examen FIS? (1:Sí, 0:No): ";
    cin >> exam;
    if (exam) {
        system("clear");
        n = 50;
      for (int i = 0; i < n; i++) {
        std::cout << "Pregunta " << i +1 << " de " << n << "\n";
        show_question(questions[i]);
        std::cout << "Respuesta: ";
        char answer;
        cin >> answer;
        if (answer == questions[i].answer) {
            questions[i].is_verified = true;
        } else {
            questions[i].is_verified = false;
            exam_rest += 1;
        }
      }
      show_results(questions);
      for (int iterator= 0; iterator < n; iterator++) {
      if (questions[iterator].is_verified == false) {
        show_question(questions[iterator]);
        std::cout << "Respuesta correcta: " << questions[iterator].answer << "\n";
        has_bad_questions = true;
      }
    }
    if (!has_bad_questions) {
      std::cout << "Muy bien, has tenido 0 errores, cada vez más cerca de aprobar FIS :D\n";
    }
      return 0;
    }
    cout << "Cuantas preguntas quieres contestar? ";
    cin >> n;
    cout << "Quieres penalizar las preguntas incorrectas? (s/n) ";
    char penalize;
    cin >> penalize;
    int bad_allowed;
    if (penalize == 's') {
        cout << "Cuantas preguntas incorrectas quieres permitirte? ";
        cin >> bad_allowed;
    }
    system("clear");
    for (int i = 0; i < n; i++) {
        std::cout << "Pregunta " << i +1 << " de " << n << "\n";
        show_question(questions[i]);
        std::cout << "Respuesta: ";
        char answer;
        cin >> answer;
        if (answer == questions[i].answer) {
            questions[i].is_verified = true;
        } else {
            questions[i].is_verified = false;
        }
        show_result(questions[i], answer);
        if (penalize == 's' && bad > bad_allowed) {
          restar++;
          bad = 0;
        }
    }
    show_results(questions);
    std::cout << "Preguntas que ha tenido mal:\n";
    for (int iterator= 0; iterator < n; iterator++) {
      if (questions[iterator].is_verified == false) {
        show_question(questions[iterator]);
        std::cout << "Respuesta correcta: " << questions[iterator].answer << "\n";
        has_bad_questions = true;
      }
    }
    if (!has_bad_questions) {
      std::cout << "Muy bien, has tenido 0 errores, cada vez más cerca de aprobar FIS :D\n";
    }
}