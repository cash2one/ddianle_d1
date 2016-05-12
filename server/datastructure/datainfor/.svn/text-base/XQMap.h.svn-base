#ifndef _XQ_MAP_H_
#define _XQ_MAP_H_

#include <map>
#include <list>

using namespace std;

template<class KEY ,class T>
class XQMap
{
public:
	XQMap(){};
	~XQMap(){};

public:
	void Add(KEY nKey ,T value)
	{
		typename map< KEY , T >::iterator it = MapData.find(nKey);
		if(it == MapData.end())
		{
			MapData.insert(typename map<KEY,T>::value_type(nKey,value));
			ListData.push_back(nKey);
		}
	}

	void Add2(KEY nKey ,T value)
	{
		typename map< KEY , T >::iterator it = MapData.find(nKey);
		if(it == MapData.end())
		{
			MapData.insert(typename map<KEY,T>::value_type(nKey,value));
			ListData.push_front(nKey);
		}
	}

	void Remove(KEY nKey)
	{
		MapData.erase(nKey);
		ListData.remove(nKey);
	}

	unsigned short Count()
	{
		if (MapData.size() == ListData.size()){
			return (unsigned short)MapData.size();
		}
		return 0;
	}

	void Clear()
	{
		MapData.clear();
		ListData.clear();
	}

	T * Find(KEY nKey)
	{
		typename map<KEY,T>::iterator it = MapData.find(nKey);
		if(it != MapData.end())
		{
			return &(it->second);
		}
		return NULL;
	}

	list<KEY> & GetKeyList()
	{
		return ListData;
	}

	map<KEY,T> & GetMap()
	{
		return MapData;
	}

private:
	list<KEY>   ListData;
	map<KEY,T>  MapData;
};

#endif


//file end

