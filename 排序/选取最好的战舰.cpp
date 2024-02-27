#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
/*
    可以自己假设表格的输入细节，那还要字典树干啥，我直接假设军舰
    的名字都是int型数字不好吗，多方便编制和读取的。

    输入格式：
    第一行：   整数n，m，表示将输入n张小表，共有m艘战舰
    接下来，每m+1行表示一张表中的数据，共 n × (m + 1) 行
    这m + 1行中，首行输入一个不带空格的字符串s，表示该表记录的军舰的属性名，然后一个整数w，表示权重
                 接下来的m行，每行一个字符串q，表示军舰名称，接着是一个浮点数a，表示属性值

    s 和 q 的长度均不超过10，并且只包含英文字符(大小写不敏感)和 '-'连字符
    1 <= n <= 20 
    1 <= m <= 100000  ( 这个范围主要是参考迎接水滴时的那十万响 )
    1 <= w <= 10
    1 <= a <= 100.00
*/

typedef struct TNode {
    /* Trie树结点，子节点数目动态扩容 */
    int noc;         // num of childs
    int soc;         // size of childs
    char tag;        // 唯一标识结点
    int idx;         // 表示对应信息在链表数组中的下标
    TNode** childs;

    TNode(char name) : noc(0), soc(10), tag(name), idx(-1), childs(nullptr)
    {
        childs = new TNode * [soc];   // 初始10个
    }

    ~TNode()
    {
        if (childs) {
            for (int i = 0; i < noc; i++)
                delete childs[i];
            delete childs;
        }
    }

    void expand()
    { // 倍增扩容
        if (noc == soc) {
            soc *= 2;
            TNode** tmp = childs;
            childs = new TNode * [soc];
            for (int i = 0; i < soc / 2; ++i)
                childs[i] = tmp[i];
            delete tmp;
        }
    }

    int add_child(char t)
    {
        TNode* child = new TNode(t);
        childs[noc++] = child;
        expand();
        return noc - 1;
    }

    int find_child(char tar)
    {
        for (int i = 0; i < noc; i++) {
            if (childs[i]->tag == tar)
                return i;
        }
        return -1;
    }

}TNode;

typedef struct LNode {
    // 记录得分的
    double score;   // 得分(已乘上权值)
    LNode* next;    // 链表的下一位

    LNode() : score(0), next(nullptr) {};

    ~LNode()
    {
        if (next)
            delete next;
    }
}LNode;

void check_cmd_input(int argc, char** argv)
{
    if (argc == 1) {
        cout << "Welcome! This is a statistic system." << endl;
        cout << "-h for help" << endl;
        cout << "-w to work" << endl;
        exit(0);
    }
    if (argc == 2) {
        if (!strcmp(argv[1], "-h")) {
            cout << endl;
            cout << endl;
            cout << "--------------------------------------------------------------------" << endl;
            cout << endl;
            cout << "北海，如果有一天我不在了，你能替我保护好大家吗？" << endl;
            cout << endl;
            cout << "北海啊，我只能告诉你，那以前，要多想" << endl;
            cout << endl;
            cout << "自然选择，前进四!" << endl;
            cout << endl;
            cout << "报告长官，逃离是事实，但我没有背叛！我不需要思想钢印，我是自己信念的主人" << endl;
            cout << endl;
            cout << "没关系的，都一样" << endl;
            cout << endl;
            cout << "--------------------------------------------------------------------" << endl;
            cout << endl;
            cout << endl;
            cout << "Things below may help you:" << endl;
            cout << "    数据输入格式：\n\
            第一行：   整数n，m，表示将输入n张小表，共有m艘战舰\n\
            接下来，每m + 1行表示一张表中的数据，共 n ×(m + 1) 行\n\
            这m + 1行中，首行输入一个不带空格的字符串s，表示该表记录的军舰的属性名，然后一个整数w，表示权重\n\
            接下来的m行，每行一个字符串q，表示军舰名称，接着是一个浮点数a，表示属性值\n\
\n\
            s 和 q 的长度均不超过10，并且只包含英文字符(大小写不敏感)和 '-'连字符\n\
            1 <= n <= 20\n\
            1 <= m <= 100000  (这个范围主要是参考迎接水滴时的那十万响)\n\
            1 <= w <= 10\n\
            1 <= a <= 100.00" << endl;
            cout << endl;
            cout << "    命令清单:" << endl;
            cout << "      -h for help" << endl;
            cout << "      -w to work" << endl;
            exit(0);
        }
        else if (strcmp(argv[1], "-w")) {
            cout << "Wrong cmd! -h for help" << endl;
            exit(-1);
        }
    }
    else {
        cout << "Num of arg are wrong!  -h for help" << endl;
        exit(-1);
    }
}

void build_trie_tree(TNode& root, vector<LNode>& DataLs, vector<LNode*>& tails, vector<string>& vs_n, int m)
{
    string vessel;
    double score;
    int cnt = 0;      // 位置记录仪
    
    TNode* p = &root;
    while (m--) {
        cin >> vessel >> score;

        int x;
        for (auto it : vessel) {
            x = p->find_child(it);
            if (x == -1)  // 没找到对应于这个前缀的子节点
                x = p->add_child(it);   // 没找到就新建
            p = p->childs[x];
        }
        p->idx = cnt;

        DataLs[cnt].next = new LNode;
        tails[cnt] = DataLs[cnt].next;
        tails[cnt]->next = nullptr;
        tails[cnt]->score = score;
        vs_n[cnt] = vessel;
        cnt++;
        p = &root;
    }
}

int search_id(TNode& root, string vessel)
{
    TNode* p = &root;
    for (auto it : vessel) {
        p = p->childs[p->find_child(it)];
    }
    return p->idx;
}

void add_data(int id, double score, vector<LNode*>& tails)
{
    tails[id]->next = new LNode;
    tails[id] = tails[id]->next;
    tails[id]->next = nullptr;
    tails[id]->score = score;
}

void merge_sort(vector<int>& Seq, vector<int>& merge_cache, vector<LNode*>& tails, int lt, int rt)
{
    if (lt >= rt)
        return;
    int mid = (lt + rt) >> 1;
    merge_sort(Seq, merge_cache, tails, lt, mid);
    merge_sort(Seq, merge_cache, tails, mid + 1, rt);
    int i = lt, j = mid + 1, k = lt;

    while (i <= mid && j <= rt) {
        if (tails[Seq[i]]->score <= tails[Seq[j]]->score) merge_cache[k++] = Seq[i++];
        else merge_cache[k++] = Seq[j++];
    }
    while (i <= mid) merge_cache[k++] = Seq[i++];
    while (j <= rt) merge_cache[k++] = Seq[j++];
    for (int z = lt; z <= rt; ++z)
        Seq[z] = merge_cache[z];
}

int main(int argc, char** argv)
{
    freopen("test.txt", "r", stdin);

    // 命令行交互提示
    check_cmd_input(argc, argv);

    cout << "欢迎进入本系统工作模式\n温馨提示：请按照使用说明来输入数据(命令行 -h 查看)，否则结果可能有误" << endl;
    cout << "请输入表格的数目和军舰的数目" << endl;
    int n, m;
    cin >> n >> m;

    vector<string> attr_names(n);    // 按输入顺序记录属性名
    vector<int> weights(n);          // 按输入顺序记录权重
    vector<LNode> DataLs;            // 数据链表头结点数组
    vector<LNode*> tails;            // 数据链表尾指针数组
    vector<string> vessel_names(m);  // 战舰名称

    for (int i = 0; i < m; i++) {
        LNode h_tmp;
        DataLs.push_back(h_tmp);
        tails.push_back(nullptr);
    }

    cout << "请输入第一张表的属性名和属性权重" << endl;
    cin >> attr_names[0];
    cin >> weights[0];

    cout << "请输入第一张表的数据:" << endl;
    TNode root(0);  // Trie树根
    build_trie_tree(root, DataLs, tails, vessel_names, m);

    string vessel;
    double score;
    for(int i = 1; i < n; i++) {
        cout << "请输入第" << i + 1 << "张表的属性名和属性权重" << endl;
        cin >> attr_names[i] >> weights[i];

        cout << "请输入第" << i + i << "张表的数据" << endl;
        for (int j = 0; j < m; j++) {
            cin >> vessel >> score;
            int id = search_id(root, vessel);
            add_data(id, score,tails);
        }
    }

    // 统计并添加总分
    attr_names.push_back("Total");
    for (int i = 0; i < m; i++) {
        LNode* p = DataLs[i].next;
        double sum = 0;
        int pi = 0;
        while (p != nullptr) {
            p->score *= weights[pi++];
            sum += p->score;
            p = p->next;
        }

        tails[i]->next = new LNode;
        tails[i] = tails[i]->next;
        tails[i]->next = nullptr;
        tails[i]->score = sum;
    }

    // 根据总分排序
    vector<int> Seq(m), merge_cache(m);
    for (int i = 0; i < m; i++) Seq[i] = i;
    merge_sort(Seq, merge_cache, tails, 0, m - 1);

    // 根据总分递减输出
    cout << setw(12) << "战舰名称";
    for (int i = 0; i < n; i++) 
        cout << setw(attr_names[i].size() + 4) << attr_names[i];
    cout << setw(9) << "Total" << endl;

    
    for (int i = m - 1; ~i; --i) {
        cout << setw(12) << vessel_names[Seq[i]];

        LNode* p = DataLs[Seq[i]].next;
        for (int j = 0; j < n; j++, p = p->next) {
            cout << setw(attr_names[j].size() + 4) << p->score;
        }
        cout << setw(9) << p->score << endl;
    }

    cout << "Complete!" << endl;

    fclose(stdin);

    return 0;
}