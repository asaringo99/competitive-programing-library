#include<bits/stdc++.h>
using namespace std;
typedef long long ll ;
typedef long double ld ;
typedef pair<ll,ll> P ;
typedef tuple<ll,ll,ll> TP ;
#define chmin(a,b) a = min(a,b)
#define chmax(a,b) a = max(a,b)
#define bit_count(x) __builtin_popcountll(x)
#define gcd(a,b) __gcd(a,b)
#define lcm(a,b) a / gcd(a,b) * b
#define rep(i,n) for(int i = 0 ; i < n ; i++)
#define rrep(i,a,b) for(int i = a ; i < b ; i++)
#define endl "\n"
//#define USE_GRAPHVIZ

#ifdef USE_GRAPHVIZ
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#endif

enum {
    NOTFOUND = 0xFFFFFFFFFFFFFFFFLLU
};

uint64_t NODE_NO = 0;

class Node {
public:
    uint64_t no;    // node番号

    // internal nodeのときに使用
    uint64_t num;       // 左の子の部分木のもつbitの数
    uint64_t ones;      // 左の子の部分木のもつ1の数
    Node *left;
    Node *right;
    int64_t balance;    // 右の子の高さ - 左の子の高さ．+なら右の子の方が高い，-なら左の子の方が高い

    // leafのときに使用
    uint64_t bits;       // bit
    uint64_t bits_size;  // bitのサイズ(debug用)

    bool is_leaf;

    Node(uint64_t bits, uint64_t bits_size, bool is_leaf) : no(NODE_NO++), num(0), ones(0), bits(bits), bits_size(bits_size), is_leaf(is_leaf), left(nullptr), right(nullptr), balance(0) {}

    bool is_valid_node() {
        if (is_leaf) {
            if (num != 0) { return false; }
            if (ones != 0) { return false; }
            if (left != nullptr) { return false; }
            if (right != nullptr) { return false; }
        }
        else {
            if (num == 0) { return false; }
            if (left == nullptr) { return false; }
            if (right == nullptr) { return false; }
            if (bits != 0) { return false; }
            if (bits_size != 0) { return false; }
            if (ones > num) {return false; }
        }
        return true;
    }

    string info() {
        string str = "No:" + to_string(this->no) + "\n";
        if (is_leaf) {
            str += "size:" + to_string(this->bits_size) + "\n";
            for (int i = 0; i < bits_size; ++i) {
                str += to_string((bits >> (uint64_t)i) & (uint64_t)1);
            }
        }
        else {
            str += "num:" + to_string(this->num) + " ones:" + to_string(this->ones) + "\n";
        }

        return str;
    }
};

class DynamicBitVector {
public:
    Node *root;
    uint64_t size;                         // 全体のbitの数
    uint64_t num_one;                      // 全体の1の数
    const uint64_t bits_size_limit = 32;   // 各ノードが管理するbitの長さ制限．2 * bits_size_limit以上になったらノードを分割し， 1/2*bits_size_limit以下になったらノードを結合する

    DynamicBitVector(): root(nullptr), size(0), num_one(0) {}

    DynamicBitVector(vector<uint64_t> &v): root(nullptr), size(0), num_one(0) {
        if (v.size() == 0) {
            return;
        }

        deque<pair<Node*, uint64_t>> leaves;
        for (int i = 0; i < v.size(); i += this->bits_size_limit) {
            uint64_t bits = 0;
            const uint64_t bits_size = min(this->bits_size_limit, (uint64_t)v.size() - i);
            for (int j = 0; j < bits_size; ++j) {
                assert(v[i + j] == 0 or v[i + j] == 1);
                if (v[i + j] == 1) {
                    bits |= (uint64_t)1 << j;
                }
            }

            leaves.emplace_back(make_pair(new Node(bits, bits_size, true), bits_size));
        }


        deque<tuple<Node*, uint64_t, uint64_t, uint64_t>> nodes;   // (node, 全体のbit数, 全体の1の数, 高さ)
        while (not leaves.empty()) {
            const auto node = leaves.front().first;
            const auto bits_size = leaves.front().second;
            leaves.pop_front();
            nodes.emplace_back(make_tuple(node, bits_size, popCount(node->bits), 0));
        }

        while (nodes.size() > 1) {

            deque<tuple<Node*, uint64_t, uint64_t, uint64_t>> next_nodes;
            while (not nodes.empty()) {
                // あまりがでたときは，最後に作った中間ノードと結合させるためにnodesに戻す
                if (nodes.size() == 1) {
                    nodes.push_front(next_nodes.back());
                    next_nodes.pop_back();
                }

                Node *left_node;
                uint64_t left_num, left_ones, left_height;
                tie(left_node, left_num, left_ones, left_height) = nodes.front(); nodes.pop_front();

                Node *right_node;
                uint64_t right_num, right_ones, right_height;
                tie(right_node, right_num, right_ones, right_height) = nodes.front(); nodes.pop_front();

                const auto internal_node = new Node(0, 0, false);
                internal_node->num = left_num;
                internal_node->ones = left_ones;
                internal_node->left = left_node;
                internal_node->right = right_node;
                internal_node->balance = right_height - left_height;

                next_nodes.emplace_back(make_tuple(internal_node, left_num + right_num, left_ones + right_ones, max(left_height, right_height) + 1));
            }

            nodes = next_nodes;
        }

        uint64_t height;
        tie(this->root, this->size, this->num_one, height) = nodes.front(); nodes.pop_front();
        assert(this->size == v.size());
    }

    // B[pos]
    uint64_t access(uint64_t pos) {
        assert(pos < this->size);

        return access(this->root, pos);
    }

    // B[0, pos)にある指定されたbitの数
    uint64_t rank(uint64_t bit, uint64_t pos) {
        assert(bit == 0 or bit == 1);
        assert(pos <= this->size);

        if (bit) {
            return rank1(this->root, pos, 0);
        }
        else {
            return pos - rank1(this->root, pos, 0);
        }
    }

    // rank番目の指定されたbitの位置 + 1(rankは1-origin)
    uint64_t select(uint64_t bit, uint64_t rank) {
        assert(bit == 0 or bit == 1);
        assert(rank > 0);

        if (bit == 0 and rank > this->size - this-> num_one) { return NOTFOUND; }
        if (bit == 1 and rank > this-> num_one)              { return NOTFOUND; }

        if (bit) {
            return select1(this->root, 0, rank);
        }
        else {
            return select0(this->root, 0, rank);
        }
    }

    // posにbitを挿入する
    void insert(uint64_t pos, uint64_t bit) {
        assert(bit == 0 or bit == 1);
        assert(pos <= this->size);  // 現在もってるbitsの末尾には挿入できる

        if (root == nullptr) {
            root = new Node(bit, 1, true);
        } else {
            insert(this->root, nullptr, bit, pos, this->size);
        }
        this->size++;
        this->num_one += (bit == 1);
    }

    // 末尾にbitを追加する
    void push_back(uint64_t bit) {
        assert(bit == 0 or bit == 1);

        this->insert(this->size, bit);
    }

    // posを削除する
    void erase(uint64_t pos) {
        assert(pos < this->size);

        uint64_t bit = this->remove(this->root, nullptr, pos, this->size, 0, true).first;
        this->size--;
        this->num_one -= (bit == 1);
    }

    // posにbitをセットする
    void update(uint64_t pos, uint64_t bit) {
        assert(bit == 0 or bit == 1);
        assert(pos < this->size);

        if (bit == 1) {
            this->bitset(pos);
        }
        else {
            this->bitclear(pos);
        }
    }

    // posのbitを1にする
    void bitset(uint64_t pos) {
        assert(pos < this->size);

        bool flip = bitset(this->root, pos);
        this->num_one += flip;
    }

    // posのbitを0にする
    void bitclear(uint64_t pos) {
        assert(pos < this->size);

        bool flip = bitclear(this->root, pos);
        this->num_one -= flip;
    }

    // dotファイルを作成する(debug用)
    void graphviz(const string &file_path) {

#ifdef USE_GRAPHVIZ
        boost::adjacency_list<> graph;
        vector<string> labels;

        auto root = boost::add_vertex(graph);
        labels.emplace_back(this->root->info());

        queue<pair<Node*, boost::adjacency_list<>::vertex_descriptor>> que;
        que.emplace(make_pair(this->root, root));

        while (not que.empty()) {
            Node *node = que.front().first;
            auto parent = que.front().second;
            que.pop();

            if (not node->is_leaf) {
                // left
                auto left = boost::add_vertex(graph);
                boost::add_edge(parent, left, graph);
                labels.emplace_back("L\n" + node->left->info());
                que.emplace(make_pair(node->left, left));

                // right
                auto right = boost::add_vertex(graph);
                boost::add_edge(parent, right, graph);
                labels.emplace_back("R\n" + node->right->info());
                que.emplace(make_pair(node->right, right));
            }
        }
        ofstream file(file_path);
        boost::write_graphviz(file, graph, boost::make_label_writer(&labels[0]));
#else
        cerr << "please define USE_GRAPHVIZ" << endl;
#endif
    }

    // 木の状態が正しいかチェックする(debug用)
    // 各ノードのbalanceの値が正しいか．AVL木の制約を守っているかのチェック
    bool is_valid_tree(bool verbose) {
        if (this->root == nullptr) {
            if (this->size == 0 and this->num_one == 0) {
                return true;
            }
            cerr << "root is nullptr but size is " << this->size << " and num_one is " << this->num_one << endl;
            return false;
        }
        map<uint64_t, uint64_t> height;
        get_height(this->root, height);

        queue<Node*> que;
        que.emplace(this->root);

        while (not que.empty()) {
            Node *node = que.front(); que.pop();

            if (not node->is_valid_node()) {
                if (verbose) {
                    cerr << "node " << node->no << " is invalid node" << endl;
                    cerr << node->info() << endl;
                }
                return false;
            }

            if (not node->is_leaf) {
                auto left_height = height[node->left->no];
                auto right_height = height[node->right->no];
                // バランスの値が正しいかチェック
                if (node->balance != right_height - left_height) {
                    if (verbose) {
                        cerr << "node" << node->no << "'s balance is " << node->balance << "(left height:" << left_height << ", right height:" << right_height << ")" << endl;
                    }
                    return false;
                }

                // AVL木の制約を満たしていない
                if (node->balance < -1 or 1 < node->balance) {
                    if (verbose) {
                        cerr << "node" << node->no << "is not balanced." << "(balance:" << node->balance << ", left height:" << left_height << ", right height:" << right_height << ")" << endl;
                    }
                    return false;
                }

                que.emplace(node->left);
                que.emplace(node->right);
            }
            else {
                // バランスの値が正しいかチェック
                if (node->balance != 0) {
                    if (verbose) {
                        cerr << "node " << node->no << "'s balance is not 0" << endl;
                    }
                    return false;
                }
            }
        }
        return true;
    }

private:
    uint64_t access(const Node *node, uint64_t pos) {
        if (node->is_leaf) {
            assert(pos <= 2 * this->bits_size_limit);
            return (node->bits >> pos) & (uint64_t)1;
        }

        if (pos < node->num) {
            return this->access(node->left, pos);
        } else {
            return this->access(node->right, pos - node->num);
        }
    }

    uint64_t rank1(const Node *node, uint64_t pos, uint64_t ones) {
        if (node->is_leaf) {
            assert(node->bits_size >= pos);
            const uint64_t mask = ((uint64_t)1 << pos) - 1;
            return ones + popCount(node->bits & mask);
        }

        if (pos < node->num) {
            return this->rank1(node->left, pos, ones);
        } else {
            return this->rank1(node->right, pos - node->num, ones + node->ones);
        }
    }

    uint64_t select1(const Node *node, uint64_t pos, uint64_t rank) {
        if (node->is_leaf) {
            return pos + this->selectInBlock(node->bits, rank) + 1;
        }

        if (rank <= node->ones) {
            return this->select1(node->left, pos, rank);
        } else {
            return this->select1(node->right, pos + node->num, rank - node->ones);
        }
    }

    uint64_t select0(const Node *node, uint64_t pos, uint64_t rank) {
        if (node->is_leaf) {
            return pos + this->selectInBlock(~node->bits, rank) + 1;
        }

        if (rank <= (node->num - node->ones)) {
            return this->select0(node->left, pos, rank);
        } else {
            return this->select0(node->right, pos + node->num, rank - (node->num - node->ones));
        }
    }

    // bits(64bit)のrank番目(0-index)の1の数
    uint64_t selectInBlock(uint64_t bits, uint64_t rank) {
        const uint64_t x1 = bits - ((bits & 0xAAAAAAAAAAAAAAAALLU) >> (uint64_t)1);
        const uint64_t x2 = (x1 & 0x3333333333333333LLU) + ((x1 >> (uint64_t)2) & 0x3333333333333333LLU);
        const uint64_t x3 = (x2 + (x2 >> (uint64_t)4)) & 0x0F0F0F0F0F0F0F0FLLU;

        uint64_t pos = 0;
        for (;;  pos += 8){
            const uint64_t rank_next = (x3 >> pos) & 0xFFLLU;
            if (rank <= rank_next) break;
            rank -= rank_next;
        }

        const uint64_t v2 = (x2 >> pos) & 0xFLLU;
        if (rank > v2) {
            rank -= v2;
            pos += 4;
        }

        const uint64_t v1 = (x1 >> pos) & 0x3LLU;
        if (rank > v1){
            rank -= v1;
            pos += 2;
        }

        const uint64_t v0  = (bits >> pos) & 0x1LLU;
        if (v0 < rank){
            pos += 1;
        }

        return pos;
    }

    // nodeから辿れる葉のpos番目にbitをいれる(葉のbitの長さはlen)
    // 高さの変化を返す
    int64_t insert(Node *node, Node *parent, uint64_t bit, uint64_t pos, uint64_t len) {
        assert(bit == 0 or bit == 1);
        if (node->is_leaf) {
            assert(pos <= 2 * this->bits_size_limit);

            // posより左をとりだす
            const uint64_t up_mask = (((uint64_t)1 << (len - pos)) - 1) << pos;
            const uint64_t up = (node->bits & up_mask);

            // posより右をとりだす
            const uint64_t down_mask = ((uint64_t)1 << pos) - 1;
            const uint64_t down = node->bits & down_mask;

            node->bits = (up << (uint64_t)1) | (bit << pos) | down;
            node->bits_size++;
            assert(node->bits_size == len + 1);

            // bitsのサイズが大きくなったので分割する
            if (len + 1 >= 2 * bits_size_limit) {
                this->splitNode(node, len + 1); // 引数のlenはinsert後の長さなので+1する
                return 1;
            }

            return 0;
        }

        if (pos < node->num) {
            const int64_t diff = this->insert(node->left, node, bit, pos, node->num);
            node->num += 1;
            node->ones += (bit == 1);
            return achieveBalance(parent, node, diff, 0);
        } else {
            const int64_t diff = this->insert(node->right, node, bit, pos - node->num, len - node->num);
            return achieveBalance(parent, node, 0, diff);
        }
    }

    // nodeのpos番目のbitを削除する
    // 消したbitと高さの変化のpairを返す
    pair<uint64_t, int64_t> remove(Node *node, Node *parent, uint64_t pos, uint64_t len, uint64_t ones, bool allow_under_flow) {
        if (node->is_leaf) {
            // 消すとunder flowになるので消さない
            if (node != this->root and len <= bits_size_limit / 2 and not allow_under_flow) {
                return make_pair(NOTFOUND, 0);
            }

            assert(pos <= 2 * this->bits_size_limit);
            assert(pos < len);
            const uint64_t bit = (node->bits >> pos) & (uint64_t)1;

            // posより左をとりだす(posを含まないようにする)
            const uint64_t up_mask = (((uint64_t)1 << (len - pos - 1)) - 1) << (pos + 1);
            const uint64_t up = (node->bits & up_mask);

            // posより右をとりだす
            const uint64_t down_mask = ((uint64_t)1 << pos) - 1;
            const uint64_t down = node->bits & down_mask;

            node->bits = (up >> (uint64_t)1) | down;
            node->bits_size--;
            assert(node->bits_size == len - 1);

            return make_pair(bit, 0);
        }

        if (pos < node->num) {
            const auto bit_diff = this->remove(node->left, node, pos, node->num, node->ones, allow_under_flow);
            if (bit_diff.first == NOTFOUND) {
                return bit_diff;
            }

            node->ones -= (bit_diff.first == 1);
            // 左の子の葉のbitを1つ消したのでunder flowが発生している
            if (node->num == bits_size_limit / 2) {
                const auto b_d = remove(node->right, node, 0, len - bits_size_limit / 2, 0, false);  // 右の葉の先頭bitを削る

                // 右の葉もunder flowになって消せない場合は2つの葉を統合する
                if (b_d.first == NOTFOUND) {
                    assert(node->left->is_leaf);
                    assert(node->left->bits_size == bits_size_limit / 2 - 1);
                    // 右の子から辿れる一番左の葉の先頭にleftのbitsを追加する
                    mergeNodes(node->right, 0, len - bits_size_limit / 2, node->left->bits, bits_size_limit / 2 - 1, node->ones, true);
                    this->replace(parent, node, node->right);    // parentの子のnodeをnode->rightに置き換える
                    return make_pair(bit_diff.first, -1);
                }

                // 右の葉からとった先頭bitを左の葉の末尾にいれる
                assert(node->left->bits_size == bits_size_limit / 2 - 1);
                insert(node->left, node, b_d.first, bits_size_limit / 2 - 1, bits_size_limit / 2 - 1);
                node->ones += (b_d.first == 1);
            }
            else {
                node->num -= 1;
            }

            const int64_t diff = achieveBalance(parent, node, bit_diff.second, 0);
            return make_pair(bit_diff.first, diff);

        } else {
            const auto bit_diff = this->remove(node->right, node, pos - node->num, len - node->num, ones - node->ones, allow_under_flow);
            if (bit_diff.first == NOTFOUND) {
                return bit_diff;
            }

            ones -= (bit_diff.first == 1);
            // 右の子の葉のbitを1つ消したのでunder flowが発生する
            if ((len - node->num) == bits_size_limit / 2) {
                const auto b_d = remove(node->left, node, node->num - 1, node->num, 0, false);    // 左の葉の末尾をbitを削る

                // 左の葉もunder flowになって消せない場合は2つの葉を統合する
                if (b_d.first == NOTFOUND) {
                    assert(node->right->is_leaf);
                    assert(node->right->bits_size == bits_size_limit / 2 - 1);
                    // 左の子から辿れる一番右の葉の末尾にleftのbitsを追加する
                    mergeNodes(node->left, node->num, node->num, node->right->bits, bits_size_limit / 2 - 1, ones - node->ones, false);
                    this->replace(parent, node, node->left);    // parentの子のnodeをnode->leftに置き換える
                    return make_pair(bit_diff.first, -1);
                }

                // 左の葉からとった最後尾bitを右の葉の先頭にいれる
                insert(node->right, node, b_d.first, 0, bits_size_limit / 2 - 1);
                node->num -= 1;
                node->ones -= (b_d.first == 1);
            }

            const int64_t diff = achieveBalance(parent, node, 0, bit_diff.second);
            return make_pair(bit_diff.first, diff);
        }
    }

    // pos番目のbitを1にする．0から1への反転が起きたらtrueを返す
    bool bitset(Node *node, uint64_t pos) {
        if (node->is_leaf) {
            assert(pos <= 2 * this->bits_size_limit);
            const uint64_t bit = (node->bits >> pos) & 1;
            if (bit == 1) {
                return false;
            }

            node->bits |= ((uint64_t)1 << pos);
            return true;
        }

        if (pos < node->num) {
            bool flip = this->bitset(node->left, pos);
            node->ones += flip;
            return flip;
        } else {
            return this->bitset(node->right, pos - node->num);
        }
    }

    // pos番目のbitを0にする．1から0への反転が起きたらtrueを返す
    bool bitclear(Node *node, uint64_t pos) {
        if (node->is_leaf) {
            assert(pos <= 2 * this->bits_size_limit);

            const uint64_t bit = (node->bits >> pos) & 1;
            if (bit == 0) {
                return false;
            }
            node->bits &= ~((uint64_t)1 << pos);
            return true;
        }

        if (pos < node->num) {
            const bool flip = this->bitclear(node->left, pos);
            node->ones -= flip;
            return flip;
        } else {
            return this->bitclear(node->right, pos - node->num);
        }
    }

    // nodeを2つの葉に分割する
    void splitNode(Node *node, uint64_t len) {
        assert(node->is_leaf);
        assert(node->bits_size == len);

        // 左の葉
        const uint64_t left_size = len / 2;
        const uint64_t left_bits = node->bits & (((uint64_t)1 << left_size) - 1);
        node->left = new Node(left_bits, left_size, true);

        // 右の葉
        const uint64_t right_size = len - left_size;
        const uint64_t right_bits = node->bits >> left_size;
        node->right = new Node(right_bits, right_size, true);

        // nodeを内部ノードにする
        node->is_leaf = false;
        node->num = left_size;
        node->ones = this->popCount(left_bits);
        node->bits = 0;
        node->bits_size = 0;
    }

    // nodeから辿れる葉のpos番目にbitsを格納する
    void mergeNodes(Node *node, uint64_t pos, uint64_t len, uint64_t bits, uint64_t s, uint64_t ones, bool left) {
        if (node->is_leaf) {
            if (left) {
                node->bits = (node->bits << s) | bits;
            }
            else {
                assert(len == node->bits_size);
                node->bits = node->bits | (bits << len);
            }
            node->bits_size += s;
            return;
        }

        if (pos < node->num) {
            node->num += s;
            node->ones += ones;
            mergeNodes(node->left, pos, node->num, bits, s, ones, left);
        }
        else {
            mergeNodes(node->right, pos, len - node->num, bits, s, ones, left);
        }
    }

    // nodeの左の高さがleftHeightDiffだけ変わり，右の高さがrightHeightDiffだけ変わったときにnodeを中心に回転させる
    // 高さの変化を返す
    int64_t achieveBalance(Node *parent, Node *node, int64_t leftHeightDiff, int64_t rightHeightDiff) {
        assert(-1 <= node->balance and node->balance <= 1);
        assert(-1 <= leftHeightDiff and leftHeightDiff <= 1);
        assert(-1 <= rightHeightDiff and rightHeightDiff <= 1);

        if (leftHeightDiff == 0 and rightHeightDiff == 0) {
            return 0;
        }

        int64_t heightDiff = 0;
        // 左が高いときに，左が高くなる or 右が高いときに右が高くなる
        if ((node->balance <= 0 and leftHeightDiff > 0) or (node->balance >= 0 and rightHeightDiff > 0)) {
            ++heightDiff;
        }
        // 左が高いときに左が低くなる or 右が高いときに右が低くなる
        if ((node->balance < 0 and leftHeightDiff < 0) or (node->balance > 0 and rightHeightDiff < 0)) {
            --heightDiff;
        }

        node->balance += -leftHeightDiff + rightHeightDiff;
        assert(-2 <= node->balance and node->balance <= 2);

        // 左が2高い
        if (node->balance == -2) {
            assert(-1 <= node->left->balance and node->left->balance <= 1);
            if (node->left->balance != 0) {
                heightDiff--;
            }

            if (node->left->balance == 1) {
                replace(node, node->left, rotateLeft(node->left));
            }
            replace(parent, node, rotateRight(node));
        }
            // 右が2高い
        else if (node->balance == 2) {
            assert(-1 <= node->right->balance and node->right->balance <= 1);
            if (node->right->balance != 0) {
                heightDiff--;
            }

            if (node->right->balance == -1) {
                replace(node, node->right, rotateRight(node->right));
            }
            replace(parent, node, rotateLeft(node));
        }

        return heightDiff;
    }

    // node Bを中心に左回転する．新しい親を返す
    Node *rotateLeft(Node *B) {
        Node *D = B->right;

        const int64_t heightC = 0;
        const int64_t heightE = heightC + D->balance;
        const int64_t heightA = max(heightC, heightE) + 1 - B->balance;

        B->right = D->left;
        D->left = B;

        B->balance = heightC - heightA;
        D->num += B->num;
        D->ones += B->ones;
        D->balance = heightE - (max(heightA, heightC) + 1);

        assert(-2 <= B->balance and B->balance <= 2);
        assert(-2 <= D->balance and D->balance <= 2);

        return D;
    }

    // node Dを中心に右回転する．新しい親を返す
    Node *rotateRight(Node *D) {
        Node *B = D->left;

        const int64_t heightC = 0;
        const int64_t heightA = heightC - B->balance;
        const int64_t heightE = max(heightA, heightC) + 1 + D->balance;

        D->left = B->right;
        B->right = D;

        D->num -= B->num;
        D->ones -= B->ones;
        D->balance = heightE - heightC;
        B->balance = max(heightC, heightE) + 1 - heightA;


        assert(-2 <= B->balance and B->balance <= 2);
        assert(-2 <= D->balance and D->balance <= 2);

        return B;
    }

    // parentの子のoldNodeをnewNodeに置き換える
    void replace(Node *parent, Node *oldNode, Node *newNode) {
        if (parent == nullptr) {
            this->root = newNode;
            return;
        }

        if (parent->left == oldNode) {
            parent->left = newNode;
        }
        else if (parent->right == oldNode) {
            parent->right = newNode;
        }
        else {
            throw "old node is not child";
        }
    }

    uint64_t popCount(uint64_t x) {
        x = (x & 0x5555555555555555ULL) + ((x >> (uint64_t)1) & 0x5555555555555555ULL);
        x = (x & 0x3333333333333333ULL) + ((x >> (uint64_t)2) & 0x3333333333333333ULL);
        x = (x + (x >> (uint64_t)4)) & 0x0f0f0f0f0f0f0f0fULL;
        x = x + (x >>  (uint64_t)8);
        x = x + (x >> (uint64_t)16);
        x = x + (x >> (uint64_t)32);
        return x & 0x7FLLU;
    }

    // 各ノードの高さ(一番遠い葉からの距離)を取得(debug用)
    uint64_t get_height(Node *node, map<uint64_t, uint64_t> &height) {
        if (node->is_leaf) {
            return 0;
        }

        if (height.find(node->no) != height.end()) {
            return height[node->no];
        }

        auto left_height = get_height(node->left, height);
        auto right_height = get_height(node->right, height);
        return height[node->no] = max(left_height, right_height) + 1;
    }
};

class DynamicWaveletMatrix {
public:
    std::vector<DynamicBitVector> bit_arrays;
    std::vector<uint64_t> begin_one;               // 各bitの1の開始位置
    vector<vector<uint64_t>> cumulative_sum;       // 各bitに着目したときの累積和

    uint64_t size;                                 // 与えられた配列のサイズ
    uint64_t maximum_element;                      // 最大の文字
    uint64_t bit_size;                             // 文字を表すのに必要なbit数

public:
    // max_element: 入ってくる中で一番大きい数値
    DynamicWaveletMatrix (uint64_t maximum_element) : size(0), maximum_element(maximum_element + 1) {
        this->bit_size = this->get_num_of_bit(maximum_element);
        if (bit_size == 0) {
            bit_size = 1;
        }
        this->begin_one.resize(bit_size);

        for (uint64_t i = 0; i < bit_size; ++i) {
            DynamicBitVector sv;
            bit_arrays.push_back(sv);
        }
    }

    DynamicWaveletMatrix (uint64_t num_of_alphabet, const std::vector<uint64_t> &array) : size(0), maximum_element(num_of_alphabet + 1) {
        this->bit_size = this->get_num_of_bit(num_of_alphabet);
        if (bit_size == 0) {
            bit_size = 1;
        }
        this->begin_one.resize(bit_size);

        if (array.empty()) {
            for (uint64_t i = 0; i < bit_size; ++i) {
                DynamicBitVector sv;
                bit_arrays.push_back(sv);
            }
            return;
        }

        size = array.size();

        std::vector<uint64_t> v(array), b(array.size(), 0);

        for (uint64_t i = 0; i < bit_size; ++i) {

            std::vector<uint64_t> temp;
            // 0をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 0) {
                    temp.push_back(c);
                    b[j] = 0;
                }
            }

            this->begin_one.at(i) = temp.size();

            // 1をtempにいれてく
            for (uint64_t j = 0; j < v.size(); ++j) {
                uint64_t c = v.at(j);
                uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
                if (bit == 1) {
                    temp.push_back(c);
                    b[j] = 1;
                }
            }

            DynamicBitVector dbv(b);
            bit_arrays.emplace_back(dbv);
            v = temp;
        }
    }

    // v[pos]
    uint64_t access(uint64_t pos) {
        if (pos >= this->size) { return NOTFOUND; }

        uint64_t c = 0;
        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            uint64_t bit = bit_arrays.at(i).access(pos);   // もとの数値がのi番目のbit
            c = (c <<= 1) | bit;
            pos = bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
        }
        return c;
    }

    // v[0, pos)のcの数
    uint64_t rank(uint64_t c, uint64_t pos) {
        assert(pos <= size);
        if (c >= maximum_element) {
            return 0;
        }

        uint64_t left = 0, right = pos;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  // 上からi番目のbit
            left = bit_arrays.at(i).rank(bit, left);             // cのi番目のbitと同じ数値の数
            right = bit_arrays.at(i).rank(bit, right);           // cのi番目のbitと同じ数値の数
            if (bit) {
                left += this->begin_one.at(i);
                right += this->begin_one.at(i);
            }
        }

        return right - left;
    }

    // i番目のcの位置 + 1を返す。rankは1-origin
    uint64_t select(uint64_t c, uint64_t rank) {
        assert(rank > 0);
        if (c >= maximum_element) {
            return NOTFOUND;
        }

        uint64_t left = 0;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  // 上からi番目のbit
            left = bit_arrays.at(i).rank(bit, left);               // cのi番目のbitと同じ数値の数
            if (bit) {
                left += this->begin_one.at(i);
            }
        }

        uint64_t index = left + rank;
        for (uint64_t i = 0; i < bit_arrays.size(); ++i){
            uint64_t bit = ((c >> i) & 1);      // 下からi番目のbit
            if (bit == 1) {
                index -= this->begin_one.at(bit_size - i - 1);
            }
            index = this->bit_arrays.at(bit_size - i - 1).select(bit, index);
        }
        return index;
    }

    // posにcを挿入する
    void insert(uint64_t pos, uint64_t c) {
        assert(pos <= this->size);

        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;  //　上からi番目のbit
            bit_arrays.at(i).insert(pos, bit);
            pos = bit_arrays.at(i).rank(bit, pos);
            if (bit) {
                pos += this->begin_one.at(i);
            }
            else {
                this->begin_one.at(i)++;
            }
        }

        this->size++;
    }

    // 末尾にcを追加する
    void push_back(uint64_t c) {
        this->insert(this->size, c);
    }

    // posを削除する
    void erase(uint64_t pos) {
        assert(pos < this->size);
        if (pos >= this->size) {
            throw "Segmentation fault";
        }

        for (uint64_t i = 0; i < bit_arrays.size(); ++i) {
            uint64_t bit = bit_arrays.at(i).access(pos);   // もとの数値のi番目のbit

            auto next_pos = bit_arrays.at(i).rank(bit, pos);
            bit_arrays.at(i).erase(pos);

            if (bit) {
                next_pos += this->begin_one.at(i);
            }
            else {
                this->begin_one.at(i)--;
            }
            pos = next_pos;
        }
        this->size--;
    }

    // posにcをセットする
    void update(uint64_t pos, uint64_t c) {
        assert(pos < this->size);
        this->erase(pos);
        this->insert(pos, c);
    }

    // v[begin_pos, end_pos)で最大値のindexを返す
    uint64_t maxRange(uint64_t begin_pos, uint64_t end_pos) {
        return quantileRange(begin_pos, end_pos, end_pos - begin_pos - 1);
    }

    // v[begin_pos, end_pos)で最小値のindexを返す
    uint64_t minRange(uint64_t begin_pos, uint64_t end_pos) {
        return quantileRange(begin_pos, end_pos, 0);
    }

    // v[begin_pos, end_pos)でk番目に小さい数値のindexを返す(kは0-origin)
    // つまり小さい順に並べてk番目の値
    uint64_t quantileRange(uint64_t begin_pos, uint64_t end_pos, uint64_t k) {
        if ((end_pos > size || begin_pos >= end_pos) || (k >= end_pos - begin_pos)) {
            return NOTFOUND;
        }

        uint64_t val = 0;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t num_of_zero_begin = bit_arrays.at(i).rank(0, begin_pos);
            const uint64_t num_of_zero_end = bit_arrays.at(i).rank(0, end_pos);
            const uint64_t num_of_zero = num_of_zero_end - num_of_zero_begin;     // beginからendまでにある0の数
            const uint64_t bit = (k < num_of_zero) ? 0 : 1;                       // k番目の値の上からi番目のbitが0か1か

            if (bit) {
                k -= num_of_zero;
                begin_pos = this->begin_one.at(i) + begin_pos - num_of_zero_begin;
                end_pos = this->begin_one.at(i) + end_pos - num_of_zero_end;
            }
            else {
                begin_pos = num_of_zero_begin;
                end_pos = num_of_zero_begin + num_of_zero;
            }

            val = ((val << 1) | bit);
        }

        uint64_t left = 0;
        for (uint64_t i = 0; i < bit_size; ++i) {
            const uint64_t bit = (val >> (bit_size - i - 1)) & 1;  // 上からi番目のbit
            left = bit_arrays.at(i).rank(bit, left);               // cのi番目のbitと同じ数値の数
            if (bit) {
                left += this->begin_one.at(i);
            }
        }

        const uint64_t rank = begin_pos + k - left + 1;
        return select(val, rank) - 1;
    }

    // v[begin_pos, end_pos)で[min, max)に入る値の個数
    uint64_t rangeFreq(uint64_t begin_pos, uint64_t end_pos, uint64_t min_c, uint64_t max_c) {
        if ((end_pos > size || begin_pos >= end_pos) || (min_c >= max_c) || min_c >= maximum_element) {
            return 0;
        }

        const auto maxi_t = rankAll(max_c, begin_pos, end_pos);
        const auto mini_t = rankAll(min_c, begin_pos, end_pos);
        return get<1>(maxi_t) - get<1>(mini_t);
    }

    // v[0, pos)でcより小さい文字の数
    uint64_t rankLessThan(uint64_t c, uint64_t begin, uint64_t end) {
        auto t = rankAll(c, begin, end);
        return get<1>(t);
    }

    // v[0, pos)でcより大きい文字の数
    uint64_t rankMoreThan(uint64_t c, uint64_t begin, uint64_t end) {
        auto t = rankAll(c, begin, end);
        return get<2>(t);
    }

    // v[begin, end)で(cと同じ値の数、cより小さい値の数、cより大きい値の数)を求める
    tuple<uint64_t, uint64_t, uint64_t> rankAll(const uint64_t c, uint64_t begin, uint64_t end) {
        assert(end <= size);
        const uint64_t num = end - begin;

        if (begin >= end) {
            return make_tuple(0, 0, 0);
        }
        if (c >= maximum_element || end == 0) {
            return make_tuple(0, num, 0);
        }

        uint64_t rank_less_than = 0, rank_more_than = 0;
        for (size_t i = 0; i < bit_size && begin < end; ++i) {
            const uint64_t bit = (c >> (bit_size - i - 1)) & 1;

            const uint64_t rank0_begin = this->bit_arrays.at(i).rank(0, begin);
            const uint64_t rank0_end = this->bit_arrays.at(i).rank(0, end);
            const uint64_t rank1_begin = begin - rank0_begin;
            const uint64_t rank1_end = end - rank0_end;

            if (bit) {
                rank_less_than += (rank0_end - rank0_begin);    // i番目のbitが0のものは除外される
                begin = this->begin_one.at(i) + rank1_begin;
                end = this->begin_one.at(i) + rank1_end;
            } else {
                rank_more_than += (rank1_end - rank1_begin);    // i番目のbitが1のものは除外される
                begin = rank0_begin;
                end = rank0_end;
            }
        }

        const uint64_t rank = num - rank_less_than - rank_more_than;
        return make_tuple(rank, rank_less_than, rank_more_than);
    }

    // T[s, e)で出現回数が多い順にk個の(値，頻度)を返す
    // 頻度が同じ場合は値が小さいものが優先される
    vector<pair<uint64_t, uint64_t>> topk(uint64_t s, uint64_t e, uint64_t k) {
        assert(s < e);
        vector<pair<uint64_t, uint64_t>> result;

        // (頻度，深さ，値)の順でソート
        auto c = [](const tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t> &l, const tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t> &r) {
            // width
            if (get<0>(l) != get<0>(r)) {
                return get<0>(l) < get<0>(r);
            }
            // depth
            if (get<3>(l) != get<3>(r)) {
                return get<3>(l) > get<3>(r);
            }
            // value
            if (get<4>(l) != get<4>(r)) {
                return get<4>(l) > get<4>(r);
            }
            return true;
        };

        priority_queue<tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t>, vector<tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t>>, decltype(c)> que(c);  // width, left, right, depth, value
        que.push(make_tuple(e - s, s, e, 0, 0));

        while (not que.empty()) {
            auto element = que.top(); que.pop();
            uint64_t width, left, right, depth, value;
            tie(width, left, right, depth, value) = element;

            if (depth >= this->bit_size) {
                result.emplace_back(make_pair(value, right - left));
                if (result.size() >= k) {
                    break;
                }
                continue;
            }

            // 0
            const uint64_t left0 = this->bit_arrays.at(depth).rank(0, left);
            const uint64_t right0 = this->bit_arrays.at(depth).rank(0, right);
            if (left0 < right0) {
                que.push(make_tuple(right0 - left0, left0, right0, depth + 1, value));
            }

            // 1
            const uint64_t left1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, left);
            const uint64_t right1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, right);
            if (left1 < right1) {
                que.push(make_tuple(right1 - left1, left1, right1, depth + 1, value | (1 << (bit_size - depth - 1))));
            }
        }

        return result;
    };

    // T[begin_pos, end_pos)でx <= c < yを満たすcの和を返す
    // uint64_t rangeSum(const uint64_t begin, const uint64_t end, const uint64_t x, const uint64_t y) {
    //     return rangeSum(begin, end, 0, 0, x, y);
    // }

    // T[begin_pos, end_pos)でx <= c < yを満たす最大のcを返す
    uint64_t prevValue(const uint64_t begin_pos, const uint64_t end_pos, const uint64_t x, uint64_t y) {
        assert(end_pos <= size);
        const uint64_t num = end_pos - begin_pos;

        if (x >= y or y == 0) {
            return NOTFOUND;
        }
        if (y > maximum_element) {
            y = maximum_element;
        }

        if (begin_pos >= end_pos) {
            return NOTFOUND;
        }
        if (x >= maximum_element || end_pos == 0) {
            return NOTFOUND;
        }

        y--; // x <= c <= yにする

        stack<tuple<uint64_t, uint64_t, uint64_t, uint64_t, bool>> s;   // (begin, end, depth, c, tight)
        s.emplace(make_tuple(begin_pos, end_pos, 0, 0, true));

        while (not s.empty()) {
            uint64_t b, e, depth, c;
            bool tight;
            tie(b, e, depth, c, tight) = s.top(); s.pop();

            if (depth == bit_size) {
                if (c >= x) {
                    return c;
                }
                continue;
            }

            const uint64_t bit = (y >> (bit_size - depth - 1)) & 1;

            const uint64_t rank0_begin = this->bit_arrays.at(depth).rank(0, b);
            const uint64_t rank0_end = this->bit_arrays.at(depth).rank(0, e);
            const uint64_t rank1_begin = b - rank0_begin;
            const uint64_t rank1_end = e - rank0_end;

            // d番目のbitが0のものを使う
            const uint64_t b0 = rank0_begin;
            const uint64_t e0 = rank0_end;
            if (b0 != e0) { // 範囲がつぶれてない
                const uint64_t c0 = ((c << 1) | 0);
                s.emplace(make_tuple(b0, e0, depth + 1, c0, tight and bit == 0));
            }

            // d番目のbitが1のものを使う
            const uint64_t b1 = this->begin_one.at(depth) + rank1_begin;
            const uint64_t e1 = this->begin_one.at(depth) + rank1_end;
            if (b1 != e1) {
                if (not tight or bit == 1) {
                    const auto c1 = ((c << 1) | 1);
                    s.emplace(make_tuple(b1, e1, depth + 1, c1, tight));
                }
            }
        }

        return NOTFOUND;
    }

    // T[begin_pos, end_pos)でx <= c < yを満たす最小のcを返す
    uint64_t nextValue(const uint64_t begin_pos, const uint64_t end_pos, const uint64_t x, const uint64_t y) {
        assert(end_pos <= size);
        const uint64_t num = end_pos - begin_pos;

        if (x >= y or y == 0) {
            return NOTFOUND;
        }

        if (begin_pos >= end_pos) {
            return NOTFOUND;
        }
        if (x >= maximum_element || end_pos == 0) {
            return NOTFOUND;
        }

        stack<tuple<uint64_t, uint64_t, uint64_t, uint64_t, bool>> s;   // (begin, end, depth, c, tight)
        s.emplace(make_tuple(begin_pos, end_pos, 0, 0, true));

        while (not s.empty()) {
            uint64_t b, e, depth, c;
            bool tight;
            tie(b, e, depth, c, tight) = s.top(); s.pop();

            if (depth == bit_size) {
                if (c < y) {
                    return c;
                }
                continue;
            }

            const uint64_t bit = (x >> (bit_size - depth - 1)) & 1;

            const uint64_t rank0_begin = this->bit_arrays.at(depth).rank(0, b);
            const uint64_t rank0_end = this->bit_arrays.at(depth).rank(0, e);
            const uint64_t rank1_begin = b - rank0_begin;
            const uint64_t rank1_end = e - rank0_end;

            // d番目のbitが1のものを使う
            const uint64_t b1 = this->begin_one.at(depth) + rank1_begin;
            const uint64_t e1 = this->begin_one.at(depth) + rank1_end;
            if (b1 != e1) {
                const auto c1 = ((c << 1) | 1);
                s.emplace(make_tuple(b1, e1, depth + 1, c1, tight and bit == 1));
            }

            // d番目のbitが0のものを使う
            const uint64_t b0 = rank0_begin;
            const uint64_t e0 = rank0_end;
            if (b0 != e0) {
                if (not tight or bit == 0) {
                    const uint64_t c0 = ((c << 1) | 0);
                    s.emplace(make_tuple(b0, e0, depth + 1, c0, tight));
                }
            }
        }

        return NOTFOUND;
    }

    // T[s1, e1)とT[s2, e2)に共通して出現する要素を求める
    vector<tuple<uint64_t, uint64_t, uint64_t>> intersect(uint64_t _s1, uint64_t _e1, uint64_t _s2, uint64_t _e2) {
        assert(_s1 < _e1);
        assert(_s2 < _e2);

        vector<tuple<uint64_t, uint64_t, uint64_t>> intersection;

        queue<tuple<uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t>> que; // s1, e1, s2, e2, depth, value
        que.push(make_tuple(_s1, _e1, _s2, _e2, 0, 0));
        while (not que.empty()) {
            auto e = que.front(); que.pop();
            uint64_t s1, e1, s2, e2, depth, value;
            tie(s1, e1, s2, e2, depth, value) = e;

            if (depth >= this->bit_size) {
                intersection.emplace_back(make_tuple(value, e1 - s1, e2 - s2));
                continue;
            }

            // 0
            uint64_t s1_0 = this->bit_arrays.at(depth).rank(0, s1);
            uint64_t e1_0 = this->bit_arrays.at(depth).rank(0, e1);
            uint64_t s2_0 = this->bit_arrays.at(depth).rank(0, s2);
            uint64_t e2_0 = this->bit_arrays.at(depth).rank(0, e2);

            if (s1_0 != e1_0 and s2_0 != e2_0) {
                que.push(make_tuple(s1_0, e1_0, s2_0, e2_0, depth + 1, value));
            }

            // 1
            uint64_t s1_1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, s1);
            uint64_t e1_1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, e1);
            uint64_t s2_1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, s2);
            uint64_t e2_1 = this->begin_one.at(depth) + this->bit_arrays.at(depth).rank(1, e2);

            if (s1_1 != e1_1 and s2_1 != e2_1) {
                que.push(make_tuple(s1_1, e1_1, s2_1, e2_1, depth + 1, value | (1 << bit_size - depth - 1)));
            }
        }

        return intersection;
    };

    // uint64_t rangeSum(const uint64_t begin, const uint64_t end, const uint64_t depth, const uint64_t c, const uint64_t x, const uint64_t y) {
    //     if (begin == end) {
    //         return 0;
    //     }

    //     if (depth == bit_size) {
    //         if (x <= c and c < y) {
    //             return c * (end - begin);   // 値 * 頻度
    //         }
    //         return 0;
    //     }

    //     const uint64_t next_c = ((uint64_t)1 << (bit_size - depth - 1)) | c;                   // 上からdepth番目のbitを立てる
    //     const uint64_t all_one_c = (((uint64_t)1 << (bit_size - depth - 1)) - 1) | next_c;     // depth以降のbitをたてる(これ以降全部1を選んだときの値)
    //     if(all_one_c < x or y <= c) {
    //         return 0;
    //     }

    //     // [begin, pos)のすべての要素は[x, y)
    //     if (x <= c and all_one_c < y) {
    //         return this->cumulative_sum.at(depth).at(end) - this->cumulative_sum.at(depth).at(begin);
    //     }

    //     const uint64_t rank0_begin = this->bit_arrays.at(depth).rank(0, begin);
    //     const uint64_t rank0_end = this->bit_arrays.at(depth).rank(0, end);
    //     const uint64_t rank1_begin = begin - rank0_begin;
    //     const uint64_t rank1_end = end - rank0_end;

    //     return rangeSum(rank0_begin, rank0_end, depth + 1, c, x, y) +
    //            rangeSum(this->begin_one.at(depth) + rank1_begin, this->begin_one[depth] + rank1_end, depth + 1, next_c, x, y);
    // }

private:
    uint64_t get_num_of_bit(uint64_t x) {
        if (x == 0) return 0;
        x--;
        uint64_t bit_num = 0;
        while (x >> bit_num) {
            ++bit_num;
        }
        return bit_num;
    }
};

// access(k)              : データのk番目の値を取得する
// insert(pos,c)          : posにcを挿入する
// erase(pos)             : posを削除する
// maxRange(s,e)          : [s,e)で最大値のインデックス番号を返す
// minRange(s,e)          : [s,e)で最小値値のインデックス番号を返す
// quantileRange(s,e,k)   : [s,e)でk番目に小さい数値のインデックス番号を返す(kは0-indexedなので注意)
// rank                   : 使うことはなさそう
// rangeFreq(s,e,x,y)     : [s,e)において[x,y)となる値の個数を返す
// rankLessThan(c,s,e)    : [s,e)においてcより小さい値の個数を返す
// rankMoreThan(c,s,e)    : [s,e)においてcより大きい値の個数を返す
// rankAll(c,s,e)         : [s,e)においてcと同じ個数、cより小さい個数、cより大きい個数をを返す(tupleで返すので注意) => (同じ、小さい、大きいs)
// topk
// prevValue(s,e,x,y)     : [s,e)においてx <= c < y を満たす最大のcを返す
// nextValue(s,e,x,y)     : [s,e)においてx <= c < y を満たす最小のcを返す
// intersect(s1,e1,s2,e2) : [s1, e1)と[s2, e2)に共通して出現する要素を求めvector<tuple<uint64_t,uint64_t,uint64_t>>>で返す

int main(){
    vector<uint64_t> A = {3,5,7,7,4,1,8,3,6,5,6,7,7,7,7,7,7,7} ;
    int n = A.size() ;
    DynamicWaveletMatrix dw(n,A) ;
    // cout << dw.rangeFreq(0,4,1,2) << endl ;
    // cout << dw.maxRange(3,9) << endl ;
    // cout << dw.rankLessThan(5,3,5) << endl;
    // cout << dw.quantileRange(0,1,0) << endl;
    // cout << dw.prevValue(2,9,3,6) << endl ;
    // cout << dw.access(4) << endl;
    TP tp = dw.rankAll(7,3,9) ;
    ll a , b , c ;
    tie(a,b,c) = tp ;
    cout << a << " " << b << " " << c << endl ;
    vector<tuple<uint64_t,uint64_t,uint64_t>> vec = dw.intersect(1,5,1,5) ;
    for(tuple<uint64_t,uint64_t,uint64_t> u : vec){
        uint64_t x, y, z ;
        tie(x,y,z) = u ;
        cout << x << " " << y << " " << z << endl ;
    }

    dw.insert(1,1) ;
    dw.insert(2,1) ;
    dw.insert(3,1) ;
    cout << dw.rangeFreq(0,4,1,2) << endl ;
}