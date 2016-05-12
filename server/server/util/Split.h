#ifndef __SPLIT_H__
#define __SPLIT_H__

#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

template<typename T>
inline bool ToType(const std::string& s, T& t)
{
	std::stringstream ss;
	ss << s;
	ss >> t;
	return true;
}

template<>
inline bool ToType(const std::string& s, std::string& t)
{
	t = s;
	return true;
}

template<typename T>
inline T ToType(const std::string& s)
{
	T t;
	ToType(s, t);
	return t;
}
//s：被切割的字符串；d：分隔符集合；e：目标容器，可以是vector list set等任意一元容器，元素类型也可以任意
template< class T >
inline bool Split(std::string s, std::string d, T& e)
{
	bool allok = true;
	typename T::value_type v;
	std::string::iterator pos = s.begin(), start = s.begin();
	while( start != s.end() )
	{
		pos = std::find_first_of(start, s.end(), d.begin(), d.end());
		if ( (pos == s.end()) || (pos > start) )
		{
			const std::string& tmp = s.substr(std::distance(s.begin(), start), std::distance(start, pos));
			bool ret = ToType(tmp, v);
			if (ret)
			{
				e.insert(e.end(), v);
			}
			else
			{
				allok = false;
				printf("Split error string:%s\n", tmp.c_str());
			}
			if (pos == s.end())
			{
				break;
			}
		}
		start = pos + 1;
	}
	return allok;
}
//s：被切割的字符串；bigd：大分隔符集合；smalld：小分隔符集合；e：目标容器，可以是map hash_map等任意二元容器，元素类型也可以任意
template<class T>
inline bool Split(std::string s, std::string bigd, std::string smalld, T& e)
{
	bool allok = true;
	typename T::key_type k;
	typename T::mapped_type v;
	std::string::iterator pos = s.begin(), start = s.begin();
	while (start != s.end())
	{
		pos = std::find_first_of(start, s.end(), bigd.begin(), bigd.end());
		if ((pos == s.end()) || (pos > start))
		{
			std::string tmp = s.substr(std::distance(s.begin(), start), std::distance(start, pos));
			std::vector<std::string> vec;
			Split(tmp, smalld, vec);
			if (vec.size() == 2)
			{
				if (ToType(vec[0], k) && ToType(vec[1], v))
				{
					e[k] = v;
				}
				else
				{
					allok = false;
					printf("SlitMap error string:%s\n", tmp.c_str());
				}
			}
			else
			{
				allok = false;
				printf("SlitMap error string:%s\n", tmp.c_str());
			}
			if (pos == s.end())
			{
				break;
			}
		}
		start = pos + 1;
	}
	return allok;
}

inline bool BeginWithString(const std::string& s, const std::string& subs)
{
	size_t size = subs.size();
	if (s.size() >= size)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (s[i] != subs[i])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}



// 将容器中的数据拼接成一个字符串，中间用指定分隔符分隔，容器是一元容器（vector, list, set等）
template<class T>
void ConcatToString(const T &container, const std::string &delimeter, std::string &result)
{
    std::stringstream ss;
    typename T::const_iterator it;
    for (it = container.begin(); it != container.end(); ++it)
    {
        ss << *it;
        ss << delimeter;
    }
    result = ss.str();

    if (result.length() > delimeter.length()) // 去除最后一个分隔符
    {
        result = result.substr(0, result.length() - delimeter.length());
    }
}

// 将容器中的数据拼接成一个字符串，中间用指定分隔符分隔，容器是二元容器（map, hash_map等）
// 如，有map<string, int>，内容如下：Jhon->28, Lily->23, 指定大小分隔符分别为“|”和“,”，则拼接成“Jhon,28|Lily,23”
template<class T>
void ConcatToString(const T &container, const std::string &largeDelimeter, const std::string &smallDelimeter, std::string &result)
{
    std::stringstream ss;
    typename T::const_iterator it;
    for (it = container.begin(); it != container.end(); ++it)
    {
        ss << it->first;
        ss << smallDelimeter;
        ss << it->second;
        ss << largeDelimeter;
    }
    result = ss.str();

    if (result.length() > largeDelimeter.length()) // 去除最后一个大分隔符
    {
        result = result.substr(0, result.length() - largeDelimeter.length());
    }
}


#endif

