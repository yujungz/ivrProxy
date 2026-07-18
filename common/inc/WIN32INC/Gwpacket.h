#ifndef __GPACKET_H_
#define __GPACKET_H_
#include "cttypes.h"
#include "ctutil.h"

#define GW_MAXPAIRLEN  1024
#define GW_MAXKVSETLEN 4096
#define GW_PACKETHEAD  0xEF

#define GWT_Connect		1	
#define GWT_Disconnect		2
#define GWT_UnRegister		10

#define GWT_Console		20
#define GWT_ConsoleACK		21

#define GWT_ConnectACK		100
#define GWT_Register		101
#define GWT_RegisterACK		102
#define GWT_RequestData		103
#define GWT_RequestDataACK	104
#define GWT_ChangeData		105
#define GWT_ChangeDataACK	106
#define GWT_CreateBill		107
#define GWT_CreateBillACK	108
#define GWT_CancelUser		109
#define GWT_CancelUserACK	110
#define GWT_AddMoney		111
#define GWT_AddMoneyACK		112
#define GWT_AddUser			113
#define GWT_AddUserACK		114
#define GWT_DeleteUser		115
#define GWT_DeleteUserACK	116

#define GWT_Test		200
#define GWT_TestACK		201

#define GWT_IPRequestData 	1000
#define GWT_IPRequestDataACK 	1001

#define GWKEY_UserID	        1
#define GWKEY_AccountID		2
#define GWKEY_UserType		3
#define GWKEY_AccountType	4
#define GWKEY_Password		5
#define GWKEY_AreaLimit		6
#define GWKEY_CardArea	 	7	
#define GWKEY_WanderFlag	8
#define GWKEY_Balance		9
#define GWKEY_AllowOverdraw	10
#define GWKEY_MonthLimit	11
#define GWKEY_MonthTotal	12
#define GWKEY_UserState		13

#define GWKEY_SystemID		20
#define GWKEY_ServiceNode	21
#define GWKEY_ServiceChannel	22
#define GWKEY_ServiceStartTime	23
#define GWKEY_CallBeginTime	24
#define GWKEY_ServiceTotalTime  25
#define GWKEY_AccountTimes	26
#define GWKEY_ServiceType	27
#define GWKEY_Caller		28
#define GWKEY_Called		29
#define GWKEY_BaseCharge	30
#define GWKEY_ServiceCharge	31
#define GWKEY_AddCharge		32
#define GWKEY_Rate		33
#define GWKEY_TotalCharge	34
#define GWKEY_CalledAreaType	35

#define GWKEY_CurrentTime	50
#define GWKEY_AddMoneyMode	51

#define GWKEY_RegisterName	100
#define GWKEY_ServiceClass	101	
#define GWKEY_AreaPrefix	102
#define GWKEY_UserDataSet	103

#define GWKEY_RequestData	1000
#define GWKEY_AnswerData	1001

#define GWKEY_Command		10000

#define GWERR_Success		0
#define GWERR_User		1
#define GWERR_Password		2
#define GWERR_UserLocked	3
#define GWERR_UserInUsed	4
#define GWERR_UserCanceled	5
#define GWERR_UserPaused	6
#define GWERR_Database		8
#define GWERR_DataKey		9
#define GWERR_WanderLimit	10
#define GWERR_Power             12
#define GWERR_SupportService	13
#define GWERR_TableFull		14
#define GWERR_Communication		20

#define GWERR_GatewayNotExist	30


class GWKVPair
{
public:
    GWKVPair() :
	key(0),type(CTkvs_valtypNULL), size(0), value(0), next(0){}

    GWKVPair( CTshort toKey, CTbyte toType, CTshort toSize, const char *toValue, GWKVPair  *p ) ;

    ~GWKVPair() ;

    CTshort	key ;
    CTbyte	type ;
    CTshort 	size ;
    void 	*value ;
    GWKVPair	*next ;
};

class GWKVSet
{
public  :
     GWKVSet();
     GWKVSet( GWKVSet &kvset) ;
     ~GWKVSet()
	{
	Clear();
	}

     int Get( CTshort Key, CTbyte &Type, CTshort &Size,  char *&Value) ;
     int GetByte( CTshort Key, CTbyte &pByte ) ;
     int GetShort( CTshort Key, CTshort &pShort ) ;
     int GetInt( CTshort Key, CTint &pInt ) ;

     int GetString( CTshort Key, char *&pString ) ;

     int GetByteArray( CTshort Key, CTbyte *&pByte, int &puiElements ) ;
     int GetShortArray( CTshort Key, CTshort *&pShort, int &puiElements ) ;
     int GetIntArray( CTshort Key, CTint *&pInt, int &puiElements ) ;

     int Put( CTshort Key, CTbyte Type, CTshort Size, char *Value);
     int PutByte( CTshort key, CTbyte cValue ) ;
     int PutShort( CTshort key, CTshort cValue ) ;
     int PutInt( CTshort key, CTint iValue ) ;

     int PutString( CTshort key, char *szString ) ;
     int PutByteArray( CTshort key, CTbyte aByte[], int  uiElements  ) ;
     int PutShortArray( CTshort key, CTshort aShort[], int  uiElements  ) ;
     int PutIntArray( CTshort key, CTint aInt[], int  uiElements  ) ;
     int GetFirstKey( CTshort &Key, CTbyte &Type, CTshort &Size, char *&Value) ;
     int GetNextKey( CTshort &Key, CTbyte &type,  CTshort &Size, char *&Value) ;
     int   GetNumElements();
     int Remove( CTshort Key) ;
     void Clear() ;

	 GWKVPair *GetHead(){return &head;}

     int  toString( char *szString, int maxlen ) ;
     CTbool  fromString( const char *szString, int maxlen) ;

     GWKVSet & operator = ( GWKVSet &kvset ) ;

     void   printLog( void ) const ;
     CTbool  isEmpty()
	{
	return head.next == &tail;
	}

     CTbool  isMember( CTshort Key ) ;

private :
     GWKVPair head, tail, *pCurrent;
     void    initList() ;
     GWKVPair *findPred( CTshort Key ) ;
};


struct GWPACKET
{
	public :
		CTshort type;
		CTshort connectID;
		CTshort sequence;
		CTshort status;
	private:
		GWKVSet  *messageData ;

	public :
		GWPACKET(): type(0),connectID(0),sequence(0),status(0)

		{
			messageData = new GWKVSet ;
		};
		GWPACKET( CTshort iType, CTshort iConnectID, CTshort iSequence, CTshort iStatus=0, GWKVSet *kvset=NULL )
			:type(iType), connectID(iConnectID),sequence(iSequence),status(iStatus)
		{
			messageData = new GWKVSet ;
			if( kvset )
				*messageData = *kvset ;
		};
		GWPACKET(const GWPACKET &);
		~GWPACKET(){ delete messageData;  }
		GWPACKET & operator = (const GWPACKET &);

		int   toString(char *str, int maxlen) const ;
		CTbool   fromString( const char *str, int len);
		void printLog( void )  const ;
		GWKVSet &getMessageData() const
		{
			return *messageData;
		}

		GWKVSet *GetMessageDataPtr()
		{
			return messageData;
		}

		void SetMessageData(GWKVSet *gwkvset);
};

const int constGWPACKETHEADSIZE = 11;

void    CTshort2CTbyte( CTshort number, char *szString );
CTshort CTbyte2CTshort( const char *szString, CTshort *number );
void    CTuint2CTbyte( CTuint number, char *szString );
CTuint  CTbyte2CTuint(  const char *szString, CTuint *number );

#endif
