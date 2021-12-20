#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
const int MAXSYMB = 256;

struct Node {
    int left = -1;
    int right = -1;
    int par = -2;
    unsigned char let = 0;
};

void compress() {
    cout << "Write path to the file that we need to compress\n";
    string inp_name;
    cin >> inp_name;
    if (inp_name == "a") {
        inp_name = "input.txt";
    }
    cout << "Write path to the directory where we need to save the file\n";
    string out_name;
    cin >> out_name;
    if (out_name == "a") {
        out_name = "";
    }
    out_name = out_name + "compressed_" + inp_name;
    FILE *inp = fopen(inp_name.c_str(), "rb");
    FILE *out = fopen(out_name.c_str(), "w");
    int cnts[MAXSYMB];
    for (int i = 0; i < MAXSYMB; i++) {
        cnts[i] = 0;
    }
    unsigned char cur;
    ll all = 0;
    while (fscanf(inp, "%c", &cur) != -1) {
        assert(cur < MAXSYMB);
        cnts[cur]++;
        all++;
    }

    int uniq_let = 0;
    for (int i = 0; i < MAXSYMB; i++) {
        if (cnts[i]) {
            uniq_let++;
        }
    }

    Node tree[uniq_let * 2 - 1];
    int ind = 0;
    for (int i = 0; i < MAXSYMB; i++) {
        if (cnts[i]) {
            tree[ind].let = i;
            tree[ind].par = -1;
            ind++;
        }
    }

    int fir_min = -1, sec_min = -1;
    for (int i = uniq_let; i < uniq_let * 2 - 1; i++) {
        fir_min = -1;
        for (int j = 0; j < uniq_let * 2 - 1; j++) {
            if (tree[j].par == -1) {
                if (fir_min == -1 || cnts[tree[fir_min].let] > cnts[tree[j].let]) {
                    fir_min = j;
                }
            }
        }
        sec_min = -1;
        for (int j = 0; j < uniq_let * 2 - 1; j++) {
            if (tree[j].par == -1 && j != fir_min) {
                if (sec_min == -1 || cnts[tree[sec_min].let] > cnts[tree[j].let]) {
                    sec_min = j;
                }
            }
        }
        tree[i].let = tree[sec_min].let;
        cnts[tree[sec_min].let] += cnts[tree[fir_min].let];
        tree[i].par = -1;
        tree[i].left = fir_min;
        tree[i].right = sec_min;
        tree[fir_min].par = i;
        tree[sec_min].par = i;
    }
    fprintf(out, "%d\n", uniq_let * 2 - 1);
    for (int i = 0; i < uniq_let * 2 - 1; i++) {
        fprintf(out, "%d %d %d %d\n", tree[i].par, tree[i].left, tree[i].right, tree[i].let);
    }
    fprintf(out, "%lld\n", all);
    vector <unsigned char> byt_symb[MAXSYMB];
    vector <unsigned char> tmp;
    int passed[uniq_let * 2 - 1];
    for (int i = 0; i < uniq_let * 2 - 1; i++) {
        passed[i] = 0;
    }
    ind = uniq_let * 2 - 2;
    while (true) {
        if (ind == -1) {
            break;
        }
        if (passed[ind] == 2) {
            if (tree[ind].par != -1) {
                tmp.pop_back();
            }
            ind = tree[ind].par;
            continue;
        }
        if (tree[ind].left == -1 && tree[ind].right == -1) {
            passed[ind] = 2;
            if (tree[ind].par == -1) {
                byt_symb[tree[ind].let].push_back('1');
                break;
            }
            //cout << "! " << tree[ind].let << '\n';
            for (unsigned char i: tmp) {
                byt_symb[tree[ind].let].push_back(i);
                //cout << i << ' ';
            }
            //cout << '\n';
            tmp.pop_back();
            ind = tree[ind].par;
            continue;
        }
        if (passed[ind] == 0) {
            passed[ind]++;
            ind = tree[ind].left;
            tmp.push_back('0');
            continue;
        }
        if (passed[ind] == 1) {
            passed[ind]++;
            ind = tree[ind].right;
            tmp.push_back('1');
            continue;
        }
    }
    unsigned char post = 0;
    ind = 0;
    fclose(inp);
    inp = fopen(inp_name.c_str(), "rb");
    while (fscanf(inp, "%c", &cur) != -1) {
        assert(cur < MAXSYMB);
        //cout << int(cur) << '\n';
        for (char i: byt_symb[cur]) {
            //cout << i << ' ';
            //cout << (i - '0') << ' ';
            post += ((i - '0') << ind);
            ind++;
            if (ind == 8) {
                ind = 0;
                fprintf(out, "%c", post);
                //cout << int(post) << '\n';
                post = 0;
            }
        }
        //cout << '\n';
    }
    //cout << ind;
    if (ind != 0) {
        //cout << int(post);
        fprintf(out, "%c", post);
    }
    fclose(inp);
    fclose(out);
}

void decompress() {
    cout << "Write path to the file that we need to decompress\n";
    string inp_name;
    cin >> inp_name;
    if (inp_name == "a") {
        inp_name = "input.txt";
    }
    cout << "Write path to the directory where we need to save the file\n";
    string out_name;
    cin >> out_name;
    if (out_name == "a") {
        out_name = "";
    }
    out_name = out_name + "decompressed_" + inp_name;
    FILE *inp = fopen(inp_name.c_str(), "r");
    FILE *out = fopen(out_name.c_str(), "wb");
    int tree_size;
    fscanf(inp, "%d\n", &tree_size);
    Node tree[tree_size];
    for (int i = 0; i < tree_size; i++) {
        fscanf(inp, "%d %d %d %d\n", &tree[i].par, &tree[i].left, &tree[i].right, &tree[i].let);
    }
    ll n;
    fscanf(inp, "%lld\n", &n);
    int indt = tree_size - 1;
    int ind = 8;
    char cur;
    while (n) {
        while (tree[indt].left != -1 && tree[indt].right != -1) {
            if (ind == 8) {
                ind = 0;
                fscanf(inp, "%c", &cur);
                //cout << int(cur) << '\n';
            }
            //cout << ((1 << ind) & cur) << ' ';
            if (((1 << ind) & cur) == 0) {
                indt = tree[indt].left;
            }
            else {
                indt = tree[indt].right;
            }
            ind++;
        }
        //cout << '\n';
        fprintf(out, "%c", tree[indt].let);
        indt = tree_size - 1;
        n--;
    }
    fclose(inp);
    fclose(out);
}

int main() {
    cout << "What do you want? <compress> or <decompress>?\n";
    string com;
    cin >> com;
    if (com == "compress") {
        compress();
    }
    else {
        decompress();
    }
}
