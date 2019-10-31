#include "datapool.h"
#include <string.h>

//数据池初始化（构造数据池）
DataPool::DataPool()
{
	m_pBuff = new char[DEFULT_DATAPOOL_SIZE];
	m_uDataSize = 0;
	m_uBuffSize = DEFULT_DATAPOOL_SIZE;
}

//删除数据池
DataPool::~DataPool()
{
	if ( m_pBuff )
	{
		delete[] m_pBuff;
		m_pBuff = NULL;
	}
}

/*
功能：获取数据池中的数据
参数：
unsigned int&	usize	数据大小（输出）
返回值：指向数据池的指针
*/
char* DataPool::getData(unsigned int& uSize)
{
	uSize = m_uDataSize;
	return m_pBuff;
}

/*
功能：从数据池中移除大小为usize的数据，然后，后面的数据向前移；注意移除的数据是没有返回的
参数：
unsigned usize    移出数据的大小  （输入）
返回值：true 移除成功； false 移除失败
*/
bool DataPool::outPool(unsigned int uSize)
{
	if( uSize > m_uDataSize || uSize <= 0) //移除数据大于已有数据大小时，直接返回false；***其实小于零的分支不用判断，因为unsigned int型不可能有小于零的值***
    {   
        return false;
    }
	else if(uSize == m_uDataSize) //如果要移动数据大小等于数据池中数据大小，直接将数据池清空
	{
		memset(m_pBuff, 0, m_uBuffSize);
		m_uDataSize = 0;
	}
	else
	{
		//从m_pBuff+uSize开始，移动m_DataSize-uSize大小的数据到数据池开始处
		memmove(m_pBuff, m_pBuff + uSize, m_uDataSize - uSize);
		m_uDataSize -= uSize;//数据大小变化
	}

	return true;
}

/*
功能：向数据池中注入大小为uSize、指针指向pBuff的数据
参数：
const char*		pBuff	待注入数据池的数据指针
int				uSize	待注入数据池的数据大小
返回值：bool  数据写入是否成功
false	数据写入失败
true	数据写入成功
*/
bool DataPool::putPool(const char* pBuff, unsigned int uSize)
{
	if(m_uDataSize + uSize > m_uBuffSize || uSize < 0)
    {
		return false;
    }

	memcpy(m_pBuff + m_uDataSize, pBuff, uSize);
	m_uDataSize += uSize;

	return true;
}


/*
功能：获取数据池中数据的大小
返回值：
unsigned int	数据大小
*/
unsigned int DataPool::getDataSize()
{
	return m_uDataSize;
}

/*
功能：获取数据池中可用数据的大小
返回值:
unsigned int	可用数据的大小
*/
unsigned int DataPool::getFreeSize()
{
	return m_uBuffSize - m_uDataSize;
}

/*
功能：清空数据池
*/
void DataPool::clearBuffer()
{
	memset(m_pBuff, 0, m_uBuffSize);
	m_uDataSize = 0;
}
