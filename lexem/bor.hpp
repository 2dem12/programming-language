class bor {
public:
    bor() {
        root = new node();
    }
    ~bor () {
        deleteBranch(root);
    }
    void add(const std::string& s) {
        node *cur = root;
        for (auto u : s) {
            if (cur->next[u - 'a'] == nullptr) {
                cur->next[u - 'a'] = new node();
            }
            cur = cur->next[u - 'a'];
        }
        ++cur->strings;
    }
    bool exists(const std::string& s) {
        node *cur = root;
        for (auto u: s) {
            if (cur->next[u - 'a'] != nullptr) {
                cur = cur->next[u - 'a'];
            } else return false;
        }
        if (cur->strings > 0) return 1;
        else return 0;
    }
private:
    struct node {
        node *next[26];
        int strings;
        node() {
            for (int i = 0; i < 26; i++) {
                next[i] = nullptr;
            }
            strings = 0;
        }
    };
    void deleteBranch (node * t) {
        for (int i = 0; i < 26; ++i) {
            if (t->next[i] != nullptr) {
                deleteBranch(t->next[i]);
            }
        }
        delete t;
    }
    node *root;
};

void creat_bor(bor& Bor) {
    std::string line;
    std::ifstream file("wordsList.txt");
    while (std::getline(file, line)) {
        Bor.add(line);
    }
}