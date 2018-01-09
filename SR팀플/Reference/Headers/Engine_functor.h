/*!
 * \file Engine_functor.h
 * \date 2016/01/13 13:54
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Engine_functor_h__
#define Engine_functor_h__

namespace ENGINE
{

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(){}

	public:
		template<typename T>
		void operator()(T& pInstance)
		{
			if(NULL != pInstance)
			{
				delete pInstance;
				pInstance = NULL;
			}
		}
	};

	class CDeleteMap
	{
	public:
		explicit CDeleteMap(){}
		~CDeleteMap(){}

	public:
		template<typename T>
		void operator()(T& Pair)
		{
			if( NULL != Pair.second)
			{
				delete Pair.second;
				Pair.second = NULL;
			}
		}
	};

	// 문자열 검사

	class CTagFinder
	{
	public:
		explicit CTagFinder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CTagFinder(void) {}
	public:
		template <typename T> bool operator () (T& Pair)
		{
			int iResult = lstrcmp(m_pTag, Pair.first);

			if(0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*				m_pTag;
	};

}

#endif // Engine_functor_h__