
#include "RosettaStone.h"

using namespace std;

filesystem::path exe_path = filesystem::current_path();
filesystem::path data_dir = exe_path / "Data";
filesystem::path vocab_path = data_dir / "vocab.rsv";
filesystem::path model_path = data_dir / "model_0.rsm";

unordered_map<string, int> vocab_lookup;

vector<float> parse_neuron(const std::string& line) {
    vector<float> weights;
    stringstream ss(line);
    string token;

    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            weights.push_back(std::stof(token));
        }
    }

    return weights;
}

void load_model() {
    cout << "Loading Model..." << endl;

    

    if (!filesystem::exists(model_path))
        cerr << "ERR_FILE_DOES_NOT_EXIST: " << model_path << "\n";

    std::ifstream f(model_path);

    if (!f.is_open())
        cerr << "ERR_OPENING_FILE: " << model_path << "\n";

    string line;
    int line_count = 0;
    uint8_t hidden_layers;
    uint8_t neuron_per_layer;
    vector<float> weights;

    getline(f, line);
    hidden_layers = stoi(line);
    getline(f, line);
    neuron_per_layer = stoi(line);

    while (getline(f, line)) {
       weights = parse_neuron(line);

    }
    cout << "Weights:";
    for (const float& t : weights)
        cout << " \"" << t << "\"";
    cout << endl;


    


    cout << hidden_layers << endl;
}

void load_vocab() {

    cout << "Loading Vocabulary..." << endl;

    unordered_map<string, int> vocab;

    if (!filesystem::exists(vocab_path))
        cerr << "ERR_FILE_DOES_NOT_EXIST: " << vocab_path << "\n";

    std::ifstream f(vocab_path);

    if (!f.is_open())
        cerr << "ERR_OPENING_FILE: " << vocab_path << "\n";


    string line;

    while (getline(f, line)) { //Reads each line of data
        string id; // path id

        while (line.size() && line[0] != ':') { // gets path id by reading each character up until ':'
            id += line[0];
            line.erase(0, 1);
        }

        line.erase(0, 1); // erases ':' leaving the remainder as the path

        vocab.insert({ id, stoi(line) });
    }
    
    f.close(); // Close the file

    cout << "Loaded " << vocab.size() << " words" << endl;

    vocab_lookup = vocab;
}

void add_vocab(const string& word) {
    if (!filesystem::exists(vocab_path))
        cerr << "ERR_FILE_DOES_NOT_EXIST: " << vocab_path << "\n";

    std::ofstream f(vocab_path, ios::app);
    
    if (!f.is_open())
        cerr << "ERR_OPENING_FILE: " << vocab_path << "\n";
    
    f << word << ':' << vocab_lookup.size() + 1 << "\n";

    cout << "Added: " << word << ":" << vocab_lookup.size() + 1 << endl;

    f.flush();

    load_vocab();
}

void to_lowercase(std::string& s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::tolower(c); });
}

bool yes_no_prompt() {
    string response;
    getline(cin, response);
    to_lowercase(response);
    if (response[0] == 'y')
        return true;
    return false;
}

vector<string> parse(string& s) {

    to_lowercase(s);

    cout << s << endl;
    vector<string> tokens;
    stringstream ss(s);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    cout << "Words:";
    for (const string& t : tokens)
        cout << " \"" << t << "\"";
    cout << endl;

    return tokens;
}

vector<int> tokenize(string input) { //converts user input into a integer set
    vector<int> tokens;
    vector<string> words = parse(input);

    string line;

    for (const string& word : words) {

        if (!vocab_lookup.contains(word)) {
            cout << "Word: \"" << word << "\" is not found in the vocabulary, add it?" << endl;
            if (yes_no_prompt()) {
                add_vocab(word);
            }
            else {
                tokens.push_back(0);
                continue;
            }
        }
        tokens.push_back(vocab_lookup.at(word));
    }

    cout << "Tokens:";
    for (const int& t : tokens)
        cout << " " << t;
    cout << endl;

    return tokens;
}

int main()
{
    load_vocab();

    load_model();

    string user_input = "";
    int output = 0;

    while (true)
    {
        cout << "==========================" << endl;
        cout << "Enter user input: ";
        getline(cin, user_input);

        vector<int> tokens = tokenize(user_input);

        if (find(tokens.begin(), tokens.end(), 9) != tokens.end()) {
            cout << "Has lights" << endl;
        }



        cout << endl;
        cout << "Output:  " << output << endl;
    }
	return 0;
}

