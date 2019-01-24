#ifndef _H_LINKEDLIST
#define _H_LINKEDLIST

#include <windows.h>
#include <assert.h>

template<class type>
class CLinkedList
{
public:
	CLinkedList()  { Create(); }
	~CLinkedList() { Destroy(); }

	void Create()
	{
		_pHead = NULL;
	}
	void Destroy()
	{
		while (_pHead != NULL)
		{
			CNode* pNode = _pHead;
			_pHead = _pHead->Next();
			delete pNode;
		}
	}

	bool IsEmpty() { return _pHead == NULL; }

	void Prepend(type data)
	{
		CNode* node = new CNode(data, _pHead);
		_pHead = node;
	}

	/*
	bool Append (type data)
	{
		CNode* node   = new CNode;
		node->_data  = data;
		node->_pNext = NULL;

		if (node != NULL)
		{
			if (_pTail != NULL)
				_pTail->_pNext = node;
			else
				_pHead = node;
				
			_pTail = node;

			return true;
		}
		else
			return false;
	}
	*/

	void DeleteFirst()
	{
		assert(_pHead != NULL);

		CNode* temp = _pHead;
		_pHead = _pHead->Next();

		delete temp;
	}

	type GetTop() { return _pHead->Data(); }

	// Stack
	void Push(type data) { Prepend(data); }
	type Pop ()
	{
		assert(_pHead != NULL);

		type temp = GetTop();
		DeleteFirst(); 
		return temp;
	}

	// Queue
	/*
	void Enqueue(type data) { Append(data); }
	type Dequeue() { return Pop(); }
	*/

protected:
	class CNode
	{
	public:
		CNode() {}
		CNode(type data, CNode* pNext)
			: _data(data), _pNext(pNext)
		{
		}

		CNode* Next() { return _pNext; }
		type   Data() { return _data;  }

		CNode* _pNext;
		type   _data;
	};

	CNode* _pHead;
};

class CUndoList : public CLinkedList<char*>
{
public:
	CUndoList() : _width(0), _height(0) {}

	void PushState(const char* state);
	void PopState (char** state);

	void SetDimentions(int width, int height) { _width = width; _height = height; }

	void Delete() { Destroy(); }

protected:
	int _width, _height;
};

#endif