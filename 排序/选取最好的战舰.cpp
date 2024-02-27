#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
/*
    �����Լ������������ϸ�ڣ��ǻ�Ҫ�ֵ�����ɶ����ֱ�Ӽ������
    �����ֶ���int�����ֲ����𣬶෽����ƺͶ�ȡ�ġ�

    �����ʽ��
    ��һ�У�   ����n��m����ʾ������n��С������m��ս��
    ��������ÿm+1�б�ʾһ�ű��е����ݣ��� n �� (m + 1) ��
    ��m + 1���У���������һ�������ո���ַ���s����ʾ�ñ��¼�ľ�������������Ȼ��һ������w����ʾȨ��
                 ��������m�У�ÿ��һ���ַ���q����ʾ�������ƣ�������һ��������a����ʾ����ֵ

    s �� q �ĳ��Ⱦ�������10������ֻ����Ӣ���ַ�(��Сд������)�� '-'���ַ�
    1 <= n <= 20 
    1 <= m <= 100000  ( �����Χ��Ҫ�ǲο�ӭ��ˮ��ʱ����ʮ���� )
    1 <= w <= 10
    1 <= a <= 100.00
*/

typedef struct TNode {
    /* Trie����㣬�ӽڵ���Ŀ��̬���� */
    int noc;         // num of childs
    int soc;         // size of childs
    char tag;        // Ψһ��ʶ���
    int idx;         // ��ʾ��Ӧ��Ϣ�����������е��±�
    TNode** childs;

    TNode(char name) : noc(0), soc(10), tag(name), idx(-1), childs(nullptr)
    {
        childs = new TNode * [soc];   // ��ʼ10��
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
    { // ��������
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
    // ��¼�÷ֵ�
    double score;   // �÷�(�ѳ���Ȩֵ)
    LNode* next;    // �������һλ

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
            cout << "�����������һ���Ҳ����ˣ��������ұ����ô����" << endl;
            cout << endl;
            cout << "����������ֻ�ܸ����㣬����ǰ��Ҫ����" << endl;
            cout << endl;
            cout << "��Ȼѡ��ǰ����!" << endl;
            cout << endl;
            cout << "���泤�٣���������ʵ������û�б��ѣ��Ҳ���Ҫ˼���ӡ�������Լ����������" << endl;
            cout << endl;
            cout << "û��ϵ�ģ���һ��" << endl;
            cout << endl;
            cout << "--------------------------------------------------------------------" << endl;
            cout << endl;
            cout << endl;
            cout << "Things below may help you:" << endl;
            cout << "    ���������ʽ��\n\
            ��һ�У�   ����n��m����ʾ������n��С������m��ս��\n\
            ��������ÿm + 1�б�ʾһ�ű��е����ݣ��� n ��(m + 1) ��\n\
            ��m + 1���У���������һ�������ո���ַ���s����ʾ�ñ��¼�ľ�������������Ȼ��һ������w����ʾȨ��\n\
            ��������m�У�ÿ��һ���ַ���q����ʾ�������ƣ�������һ��������a����ʾ����ֵ\n\
\n\
            s �� q �ĳ��Ⱦ�������10������ֻ����Ӣ���ַ�(��Сд������)�� '-'���ַ�\n\
            1 <= n <= 20\n\
            1 <= m <= 100000  (�����Χ��Ҫ�ǲο�ӭ��ˮ��ʱ����ʮ����)\n\
            1 <= w <= 10\n\
            1 <= a <= 100.00" << endl;
            cout << endl;
            cout << "    �����嵥:" << endl;
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
    int cnt = 0;      // λ�ü�¼��
    
    TNode* p = &root;
    while (m--) {
        cin >> vessel >> score;

        int x;
        for (auto it : vessel) {
            x = p->find_child(it);
            if (x == -1)  // û�ҵ���Ӧ�����ǰ׺���ӽڵ�
                x = p->add_child(it);   // û�ҵ����½�
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

    // �����н�����ʾ
    check_cmd_input(argc, argv);

    cout << "��ӭ���뱾ϵͳ����ģʽ\n��ܰ��ʾ���밴��ʹ��˵������������(������ -h �鿴)����������������" << endl;
    cout << "�����������Ŀ�;�������Ŀ" << endl;
    int n, m;
    cin >> n >> m;

    vector<string> attr_names(n);    // ������˳���¼������
    vector<int> weights(n);          // ������˳���¼Ȩ��
    vector<LNode> DataLs;            // ��������ͷ�������
    vector<LNode*> tails;            // ��������βָ������
    vector<string> vessel_names(m);  // ս������

    for (int i = 0; i < m; i++) {
        LNode h_tmp;
        DataLs.push_back(h_tmp);
        tails.push_back(nullptr);
    }

    cout << "�������һ�ű��������������Ȩ��" << endl;
    cin >> attr_names[0];
    cin >> weights[0];

    cout << "�������һ�ű������:" << endl;
    TNode root(0);  // Trie����
    build_trie_tree(root, DataLs, tails, vessel_names, m);

    string vessel;
    double score;
    for(int i = 1; i < n; i++) {
        cout << "�������" << i + 1 << "�ű��������������Ȩ��" << endl;
        cin >> attr_names[i] >> weights[i];

        cout << "�������" << i + i << "�ű������" << endl;
        for (int j = 0; j < m; j++) {
            cin >> vessel >> score;
            int id = search_id(root, vessel);
            add_data(id, score,tails);
        }
    }

    // ͳ�Ʋ�����ܷ�
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

    // �����ܷ�����
    vector<int> Seq(m), merge_cache(m);
    for (int i = 0; i < m; i++) Seq[i] = i;
    merge_sort(Seq, merge_cache, tails, 0, m - 1);

    // �����ֵܷݼ����
    cout << setw(12) << "ս������";
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