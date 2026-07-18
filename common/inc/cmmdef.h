#pragma once

#ifndef  YQ_FREE_VAR
#define  YQ_FREE_VAR(obj)  	if (nullptr != obj) { delete[] obj; obj = nullptr;  }
#endif 

#ifndef  YQ_FREE_OBJ 
#define  YQ_FREE_OBJ(obj)  	if (nullptr != obj) { delete obj; obj = nullptr;  }
#endif 

#ifndef  YQ_FREE_DLL
	#define  YQ_FREE_DLL(hi)  if(NULL!=hi){  ::FreeLibrary(hi); hi=NULL; }
#endif 

#ifndef  ARRAY_SIZE
#define  ARRAY_SIZE(a)  sizeof(a) / sizeof(a[0])
#endif

#define  YC_BUFF_MAX  99
#define  YR_HTTP_OK   200