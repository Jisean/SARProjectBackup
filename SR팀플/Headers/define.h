#pragma once

#define			NO_COPY(CLASSNAME)					\
				private:							\
				CLASSNAME(const CLASSNAME&);		\
				CLASSNAME& operator = (const CLASSNAME&); 

#define			DECLARE_SINGLETON2(CLASSNAME)			\
				NO_COPY(CLASSNAME)						\
				private:								\
					static CLASSNAME*		pInstance;	\
				public:									\
					static CLASSNAME*	GetInst(void);	\
					static void			DestroyInst(void);	

#define			IMPLEMENT_SINGLETON2(CLASSNAME)		\
		 CLASSNAME* CLASSNAME::pInstance = NULL;	\
		 CLASSNAME*	CLASSNAME::GetInst(void)		\
			{										\
				if(pInstance == NULL)				\
				{									\
					pInstance = new CLASSNAME;		\
				}									\
				return pInstance;					\
			}										\
			void CLASSNAME::DestroyInst(void)		\
			{										\
				if(pInstance != NULL)				\
				{									\
					delete pInstance;				\
					pInstance = NULL;				\
				}									\
			}


#define			ERR_MSG(Message)MessageBox(g_hWnd, Message, L"System Error", NULL)

#define			GET_TIME CTimeMgr::GetInst()->GetTime()