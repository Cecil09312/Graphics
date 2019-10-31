#ifndef DATAPOOL_H
#define DATAPOOL_H

//定义数据池的大小：5M
#define DEFULT_DATAPOOL_SIZE (1024*1024*5)

/************************************************************************/
/* 数据池                                                               */
/************************************************************************/
class DataPool  
{
public:
	DataPool();
	~DataPool();

public:
	//清楚数据池
    void clearBuffer();
	//获取数据池的容量大小
    unsigned int getFreeSize();
	//数据池中数据的大小
    unsigned int getDataSize();
	//获取数据池中的数据
    char* getData(unsigned int& uSize);
	//将数据从数据池中移出
    bool outPool(unsigned int uSize);
	//将数据注入进数据池
    bool putPool(const char* pBuff, unsigned int uSize);

protected:
	//数据池
	char* m_pBuff;
	//数据池大小
	unsigned int m_uBuffSize;
	//数据池中数据的大小
	unsigned int m_uDataSize;
};

#endif
