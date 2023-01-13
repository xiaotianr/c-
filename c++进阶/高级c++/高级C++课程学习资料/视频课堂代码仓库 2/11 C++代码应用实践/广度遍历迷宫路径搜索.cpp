#include "pch.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/*
Ѱ���Թ����·��
���Թ�������ô����̵�·��������
��ȱ����Թ�·������
0 0 1 1 1 1
1 0 0 0 0 1
1 0 1 1 0 1
1 0 0 0 0 1
1 0 1 1 1 1
1 0 0 0 0 0


������ȱ��������Թ�·�����������Ҫ�����£�

�������Թ���������(���磺10 10):5 5
�������Թ���·����Ϣ(0��ʾ�����ߣ�1��ʾ������):
0 0 0 1 1
1 0 1 0 1
1 1 0 1 1
1 1 0 0 1
1 1 1 0 0
�Թ�·��������...
>>>���û��·����ֱ�����<<<
������һ���Թ�·����
>>>�����·����ֱ�����<<<
* * * 1 1
1 0 * 0 1
1 1 * 1 1
1 1 * * 1
1 1 1 * *
*/
#if 0
// ���巽��
const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;
const int WAY_NUM = 4;

// ��������״̬
const int YES = 4;
const int NO = 5;

// �Թ�
class Maze
{
public:
	Maze(int row, int col)
		:_row(row)
		, _col(col)
	{
		_pMaze = new Node*[_row];
		for (int i = 0; i < _row; ++i)
		{
			_pMaze[i] = new Node[_col];
		}

		// node._x*_row + node._y
		_pPath.resize(_row * _col);
	}

	void initNode(int x, int y, int val)
	{
		_pMaze[x][y]._x = x;
		_pMaze[x][y]._y = y;
		_pMaze[x][y]._val = val;
		for (int i = 0; i < WAY_NUM; ++i)
		{
			_pMaze[x][y]._state[i] = NO;
		}
	}

	void setNodeState()
	{
		for (int i = 0; i < _row; ++i)
		{
			for (int j = 0; j < _col; ++j)
			{
				if (_pMaze[i][j]._val == 1)
				{
					continue;
				}

				if (j < _col - 1 && _pMaze[i][j + 1]._val == 0)
				{
					_pMaze[i][j]._state[RIGHT] = YES;
				}

				if (i < _row - 1 && _pMaze[i + 1][j]._val == 0)
				{
					_pMaze[i][j]._state[DOWN] = YES;
				}

				if (j > 0 && _pMaze[i][j - 1]._val == 0)
				{
					_pMaze[i][j]._state[LEFT] = YES;
				}

				if (i > 0 && _pMaze[i - 1][j]._val == 0)
				{
					_pMaze[i][j]._state[UP] = YES;
				}
			}
		}
	}

	void searchMazePath()
	{
		if (_pMaze[0][0]._val == 1)
		{
			return;
		}
		_queue.push(_pMaze[0][0]);

		while (!_queue.empty())
		{
			Node front = _queue.front();
			int x = front._x;
			int y = front._y;

			// �ҷ���
			if (_pMaze[x][y]._state[RIGHT] == YES)
			{
				_pMaze[x][y]._state[RIGHT] = NO;
				_pMaze[x][y + 1]._state[LEFT] = NO;
				// �ڸ��������м�¼һ�½ڵ��������Ϣ
				_pPath[x*_row + y + 1] = _pMaze[x][y];
				_queue.push(_pMaze[x][y + 1]);
				if (check(_pMaze[x][y + 1]))
					return;
			}

			// �·���
			if (_pMaze[x][y]._state[DOWN] == YES)
			{
				_pMaze[x][y]._state[DOWN] = NO;
				_pMaze[x + 1][y]._state[UP] = NO;
				_pPath[(x + 1)*_row + y] = _pMaze[x][y];
				_queue.push(_pMaze[x + 1][y]);
				if (check(_pMaze[x + 1][y]))
					return;
			}

			// ����
			if (_pMaze[x][y]._state[LEFT] == YES)
			{
				_pMaze[x][y]._state[LEFT] = NO;
				_pMaze[x][y - 1]._state[RIGHT] = NO;
				_pPath[x*_row + y - 1] = _pMaze[x][y];
				_queue.push(_pMaze[x][y - 1]);
				if (check(_pMaze[x][y - 1]))
					return;
			}

			// �Ϸ���
			if (_pMaze[x][y]._state[UP] == YES)
			{
				_pMaze[x][y]._state[UP] = NO;
				_pMaze[x - 1][y]._state[DOWN] = NO;
				_pPath[(x - 1)*_row + y] = _pMaze[x][y];
				_queue.push(_pMaze[x - 1][y]);
				if (check(_pMaze[x - 1][y]))
					return;
			}

			// ������
			_queue.pop();
		}
	}

	void showMazePath()
	{
		if (_queue.empty())
		{
			cout << "������һ���Թ�·����" << endl;
		}
		else
		{
			// ����Ѱ���Թ�·���ڵ�
			int x = _row - 1;
			int y = _col - 1;
			for (;;)
			{
				_pMaze[x][y]._val = '*';
				if (x == 0 && y == 0)
					break;
				Node node = _pPath[x*_row + y];
				x = node._x;
				y = node._y;
			}

			for (int i = 0; i < _row; ++i)
			{
				for (int j = 0; j < _col; ++j)
				{
					if (_pMaze[i][j]._val == '*')
					{
						cout << "* ";
					}
					else
					{
						cout << _pMaze[i][j]._val << " ";
					}
				}
				cout << endl;
			}
		}
	}
private:
	// �����Թ��ڵ�·����Ϣ
	struct Node
	{
		int _x;
		int _y;
		int _val; // �ڵ��ֵ
		int _state[WAY_NUM]; // ��¼�ڵ��ĸ������״̬
	};

	// ����Ƿ������½ǵ��Թ����ڽڵ�
	bool check(Node &node)
	{
		return node._x == _row - 1 && node._y == _col - 1;
	}

	Node **_pMaze;
	int _row;
	int _col;
	queue<Node> _queue; // ��ȱ��������Ķ��нṹ
	vector<Node> _pPath; // ��¼������ȱ���ʱ���ڵ��������Ϣ
};

int main()
{
	cout << "�������Թ���������(���磺10 10):";
	int row, col, data;
	cin >> row >> col;

	Maze maze(row, col); // �����Թ�����

	cout << "�������Թ���·����Ϣ(0��ʾ�����ߣ�1��ʾ������):" << endl;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			cin >> data;
			// ���Գ�ʼ���Թ��ڵ�Ļ�����Ϣ
			maze.initNode(i, j, data);
		}
	}

	// ��ʼ�������нڵ���ĸ���

	//���״̬
	maze.setNodeState();

	// ��ʼ�����Ͻ������Թ���·����Ϣ��
	maze.searchMazePath();

	// ��ӡ�Թ�·�������Ľ��
	maze.showMazePath();

	return 0;
}
#endif