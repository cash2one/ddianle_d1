#pragma once
template< typename T ,unsigned int nBufferSize>
class CRingBuffer
{
public:
	CRingBuffer(void);
	~CRingBuffer(void);
	
private:
	T m_buffIn[nBufferSize];
	T *m_pInHead;
	T *m_pInTail;
	const unsigned int m_nBufferSize = nBufferSize;

public :
	void ClearRingBuffer();
	BOOL AddToRingBuffer();
	BOOL popFromRingBuffer();
};

