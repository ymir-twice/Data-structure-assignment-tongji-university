#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
using namespace chrono;

const int Max_size = 1000000;
const int TIW = 10;  // table interval width

vector<int> nums;
vector<int> tmp(Max_size); // �鲢������
vector<long long> baseline(8);

void ini_nums(int size, int mode = 0, int shift = 0)
{
	if (mode == 1) {
		nums.clear();
		for (int i = 0; i < size; i++)
			nums.push_back(i + 1);
	}
	else if (mode == -1) {
		nums.clear();
		for (int i = 0; i < size; i++)
			nums.push_back(size - i);
	}
	else {
		nums.clear();
		for (int i = 0; i < size; i++)
			nums.push_back(i + 1);
		std::random_device rd;
		std::mt19937 rng(rd());
		shuffle(nums.begin(), nums.end(), rng);
	}

	if (shift) {
		for (int i = 0; i < shift; i++) nums.push_back(0);
		for (int i = nums.size() - 1; i >= shift; --i) swap(nums[i], nums[i - shift]);
	}
	return;
}

void get_baseline()
{
	int cnt = 0;

	// ���˳�򡢲�ͬ��ģ
	for (int i = 10; i <= Max_size; i*=10) {
		ini_nums(i);
		auto start = system_clock::now();
		sort(nums.begin(), nums.end());
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(10) << tt;
		baseline[cnt++] = tt;
	}
	
	// 10K����
	ini_nums(10000, 1);
	{
		auto start = system_clock::now();
		sort(nums.begin(), nums.end());
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(10) << tt;
		baseline[cnt++] = tt;
	}

	// 10K����
	ini_nums(10000, -1);
	{
		auto start = system_clock::now();
		sort(nums.begin(), nums.end());
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(10) << tt;
		baseline[cnt++] = tt;
	}
}

void quck_sort(int lt, int rt)
{
	if (lt >= rt) return;
	int i = lt - 1, j = rt + 1, x = nums[(lt + rt) >> 1];
	while (i < j) {
		do ++i; while (nums[i] < x);
		do --j; while (nums[j] > x);
		if (i < j) swap(nums[i], nums[j]);
	}
	quck_sort(lt, j);
	quck_sort(j+1, rt);
}

void merge_sort(int lt, int rt)
{
	if (lt >= rt) return;
	int mid = (lt + rt) >> 1;
	merge_sort(lt, mid);
	merge_sort(mid + 1, rt);
	int i = lt, j = mid + 1, k = lt;
	while (i <= mid && j <= rt) {
		if (nums[i] <= nums[j]) tmp[k++] = nums[i++];
		else tmp[k++] = nums[j++];
	}
	while (i <= mid) tmp[k++] = nums[i++];
	while (j <= rt) tmp[k++] = nums[j++];
	for (int z = lt; z <= rt; ++z) nums[z] = tmp[z];
}

void sort_alg_test(void (*salg)(int, int), int shift = 0)
{
	for (int i = 10; i <= Max_size; i *= 10) {
		ini_nums(i, 0, shift);
		auto start = system_clock::now();
		salg(0, nums.size() - 1);
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(TIW) << tt;
	}

	// 10K����
	ini_nums(10000, 1, shift);
	{
		auto start = system_clock::now();
		salg(0, nums.size() - 1);
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(TIW) << tt;
	}

	// 10K����
	ini_nums(10000, -1, shift);
	{
		auto start = system_clock::now();
		salg(0, nums.size() - 1);
		auto end = system_clock::now();
		auto duration = duration_cast<microseconds>(end - start);
		long long tt = long long(duration.count());  //΢��
		cout << setw(TIW) << tt;
	}
}

// ά��������
void down(int u, int top)
{
	int t = u;
	if (u * 2 <= top && nums[t] > nums[u * 2]) t = 2 * u;
	if (u * 2 + 1 <= top && nums[t] > nums[u * 2 + 1]) t = 2 * u + 1;
	if (t != u) {
		swap(nums[t], nums[u]);
		down(t, top);
	}

}
void heap_sort(int lt, int rt)
{
	// ��ʼ����
	int top = nums.size() - 1;
	for (int i = (top >> 1); i; --i) down(i, top);

	// ������(���ǡ���ǵ���)
	for (int i = nums.size() - 1; i; --i) {
		swap(nums[1], nums[i]);
		down(1, --top);
	}
}

void selection_sort(int lt, int rt) {
	for (int i = 0; i < nums.size() - 1; i++) {
		int min = i;
		for (int j = i + 1; j < nums.size(); j++)
			if (nums[j] < nums[min])
				min = j;
		swap(nums[i], nums[min]);
	}
}

void bubble_sort(int lt, int rt) {
	int i, j;
	for (i = 0; i < rt; i++)
		for (j = 0; j < rt - i; j++)
			if (nums[j] > nums[j + 1])
				swap(nums[j], nums[j + 1]);
}

void shell_sort(int lt, int rt) {
	int h = 1;
	while (h < (rt + 1) / 3) {
		h = 3 * h + 1;
	}
	while (h >= 1) {
		for (int i = h; i < rt + 1; i++) {
			for (int j = i; j >= h && nums[j] < nums[j - h]; j -= h) {
				std::swap(nums[j], nums[j - h]);
			}
		}
		h = h / 3;
	}
}

void insertion_sort(int lt, int rt) {
	for (int i = 1;i < rt+1;i++) {
		int key = nums[i];
		int j = i - 1;
		while ((j >= 0) && (key < nums[j])) {
			nums[j + 1] = nums[j];
			j--;
		}
		nums[j + 1] = key;
	}
}

int main()
{
	// ��ͷ
	cout << setw(12) << "�㷨\\��ģ";
	cout << setw(TIW) << "10";
	cout << setw(TIW) << "100";
	cout << setw(TIW) << "1K";
	cout << setw(TIW) << "10K";
	cout << setw(TIW) << "100K";
	cout << setw(TIW) << "1M";
	cout << setw(TIW) << "10K����";
	cout << setw(TIW) << "10K����";
	cout << endl << endl;

	// ��׼
	cout << setw(12) << "STL: sort";
	get_baseline();
	cout << endl << endl;

	// ����
	cout << setw(12) << "��������";
	sort_alg_test(quck_sort);
	cout << endl << endl;

	// �鲢����
	cout << setw(12) << "�鲢����";
	sort_alg_test(merge_sort);
	cout << endl << endl;

	// ������
	cout << setw(12) << "������";
	sort_alg_test(heap_sort, 1);   // ƫ�ƣ�����ģ���(��ȫ������)
	cout << endl << endl;

	// ѡ������
	cout << setw(12) << "ѡ������";
	cout << "����ʵ�飬̫����(һ��������)�������";
	//sort_alg_test(selection_sort);
	cout << endl << endl;

	// ð������
	cout << setw(12) << "ð������";
	cout << "����ʵ�飬̫����(һ��������)�������";
	//sort_alg_test(bubble_sort);
	cout << endl << endl;

	// ��������
	cout << setw(12) << "��������";
	cout << "����ʵ�飬̫����(һ��������)�������";
	//sort_alg_test(insertion_sort);
	cout << endl << endl;

	// ϣ������
	cout << setw(12) << "ϣ������";
	sort_alg_test(shell_sort);
	cout << endl << endl;

	cout << endl;
	cout << endl;
	cout << "----------------------------------------------------------------------------" << endl;
	cout << endl;
	return 0;
}
