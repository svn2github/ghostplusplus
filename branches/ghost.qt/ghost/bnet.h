/*

   Copyright [2008] [Trevor Hogan]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   CODE PORTED FROM THE ORIGINAL GHOST PROJECT: http://ghost.pwner.org/

*/

#ifndef BNET_H
#define BNET_H

//
// CBNET
//

class CCommandPacket;
class CBNCSUtilInterface;
class CBNETProtocol;
class CBNLSClient;
class CIncomingFriendList;
class CIncomingClanList;
class CIncomingChatEvent;
class CCallableAdminCount;
class CCallableAdminAdd;
class CCallableAdminRemove;
class CCallableAdminList;
class CCallableBanCount;
class CCallableBanAdd;
class CCallableBanRemove;
class CCallableBanList;
class CCallableGamePlayerSummaryCheck;
class CCallableDotAPlayerSummaryCheck;
class CDBBan;
class CGHost;
class CSaveGame;
class CBaseGame;
class CMap;

#include "includes.h"
#include <QTcpSocket>
#include <QTimer>

typedef pair<QString,CCallableAdminCount *> PairedAdminCount;
typedef pair<QString,CCallableAdminAdd *> PairedAdminAdd;
typedef pair<QString,CCallableAdminRemove *> PairedAdminRemove;
typedef pair<QString,CCallableBanCount *> PairedBanCount;
typedef pair<QString,CCallableBanAdd *> PairedBanAdd;
typedef pair<QString,CCallableBanRemove *> PairedBanRemove;
typedef pair<QString,CCallableGamePlayerSummaryCheck *> PairedGPSCheck;
typedef pair<QString,CCallableDotAPlayerSummaryCheck *> PairedDPSCheck;


#include <QTime>

class CBNET : public QObject
{
	Q_OBJECT
public slots:
	void socketConnected();
	void socketDisconnected();
	void socketDataReady();
	void socketConnect();
	void socketError();
	void sendWardenResponse(const QByteArray & response);
	void timeout_NULL();

	void EventCallableUpdateTimeout();
	void EventUpdateAdminList();
	void EventRefreshBanList();

private:
	QTimer m_NULLTimer;
	QTimer m_CallableUpdateTimer, m_AdminListUpdateTimer, m_BanListRefreshTimer;

public:
	CGHost *m_GHost;

private:
	int m_Retries;
	QTcpSocket *m_Socket;							// the connection to battle.net
	CBNETProtocol *m_Protocol;						// battle.net protocol
	CBNLSClient *m_BNLSClient;						// the BNLS client (for external warden handling)
	QQueue<CCommandPacket *> m_Packets;				// queue of incoming packets
	CBNCSUtilInterface *m_BNCSUtil;					// the interface to the bncsutil library (used for logging into battle.net)
	QQueue<QByteArray> m_OutPackets;					// queue of outgoing packets to be sent (to prevent getting kicked for flooding)
	QVector<CIncomingFriendList *> m_Friends;		// vector of friends
	QVector<CIncomingClanList *> m_Clans;			// vector of clan members
	QVector<PairedAdminCount> m_PairedAdminCounts;	// vector of paired threaded database admin counts in progress
	QVector<PairedAdminAdd> m_PairedAdminAdds;		// vector of paired threaded database admin adds in progress
	QVector<PairedAdminRemove> m_PairedAdminRemoves;	// vector of paired threaded database admin removes in progress
	QVector<PairedBanCount> m_PairedBanCounts;		// vector of paired threaded database ban counts in progress
	QVector<PairedBanAdd> m_PairedBanAdds;			// vector of paired threaded database ban adds in progress
	QVector<PairedBanRemove> m_PairedBanRemoves;		// vector of paired threaded database ban removes in progress
	QVector<PairedGPSCheck> m_PairedGPSChecks;		// vector of paired threaded database game player summary checks in progress
	QVector<PairedDPSCheck> m_PairedDPSChecks;		// vector of paired threaded database DotA player summary checks in progress
	CCallableAdminList *m_CallableAdminList;		// threaded database admin list in progress
	CCallableBanList *m_CallableBanList;			// threaded database ban list in progress
	QVector<QString> m_Admins;						// vector of cached admins
	QVector<CDBBan *> m_Bans;						// vector of cached bans
	QString m_Server;								// battle.net server to connect to
	QString m_ServerIP;								// battle.net server to connect to (the IP address so we don't have to resolve it every time we connect)
	QString m_ServerAlias;							// battle.net server alias (short name, e.g. "USEast")
	QString m_BNLSServer;							// BNLS server to connect to (for warden handling)
	quint16 m_BNLSPort;							// BNLS port
	quint32 m_BNLSWardenCookie;					// BNLS warden cookie
	QString m_CDKeyROC;								// ROC CD key
	QString m_CDKeyTFT;								// TFT CD key
	QString m_CountryAbbrev;							// country abbreviation
	QString m_Country;								// country
	quint32 m_LocaleID;							// see: http://msdn.microsoft.com/en-us/library/0h88fahh%28VS.85%29.aspx
	QString m_UserName;								// battle.net username
	QString m_UserPassword;							// battle.net password
	QString m_FirstChannel;							// the first chat channel to join upon entering chat (note: we hijack this to store the last channel when entering a game)
	QString m_CurrentChannel;						// the current chat channel
	QString m_RootAdmin;								// the root admin
	char m_CommandTrigger;							// the character prefix to identify commands
	unsigned char m_War3Version;					// custom warcraft 3 version for PvPGN users
	QByteArray m_EXEVersion;							// custom exe version for PvPGN users
	QByteArray m_EXEVersionHash;						// custom exe version hash for PvPGN users
	QString m_PasswordHashType;						// password hash type for PvPGN users
	QString m_PVPGNRealmName;						// realm name for PvPGN users (for mutual friend spoofchecks)
	int m_MaxMessageLength;					// maximum message length for PvPGN users
	quint32 m_HostCounterID;						// the host counter ID to identify players from this realm
	quint32 m_LastDisconnectedTime;				// GetTime when we were last disconnected from battle.net
	quint32 m_LastConnectionAttemptTime;			// GetTime when we last attempted to connect to battle.net
	quint32 m_LastNullTime;						// GetTime when the last null packet was sent for detecting disconnects
	quint32 m_LastOutPacketSize;
	quint32 m_LastAdminRefreshTime;				// GetTime when the admin list was last refreshed from the database
	quint32 m_LastBanRefreshTime;					// GetTime when the ban list was last refreshed from the database
	bool m_FirstConnect;							// if we haven't tried to connect to battle.net yet
	bool m_LoggedIn;								// if we've logged into battle.net or not
	bool m_InChat;									// if we've entered chat or not (but we're not necessarily in a chat channel yet)
	bool m_HoldFriends;								// whether to auto hold friends when creating a game or not
	bool m_HoldClan;								// whether to auto hold clan members when creating a game or not
	bool m_PublicCommands;							// whether to allow public commands or not

	inline int getWaitTicks()
	{
		// this formula has changed many times but currently we wait 1 second if the last packet was "small", 3.5 seconds if it was "medium", and 4 seconds if it was "big"
		if( m_LastOutPacketSize < 10 )
			return 1000;

		else if( m_LastOutPacketSize < 100 )
			return 3500;

		return 4000;
	}

	QTime m_LastPacketSent;

public slots:
	void EnqueuePacket(const QByteArray &pkg);
	void SendPacket();

public:
	CBNET( CGHost *nGHost, QString nServer, QString nServerAlias, QString nBNLSServer, quint16 nBNLSPort, quint32 nBNLSWardenCookie, QString nCDKeyROC, QString nCDKeyTFT, QString nCountryAbbrev, QString nCountry, quint32 nLocaleID, QString nUserName, QString nUserPassword, QString nFirstChannel, QString nRootAdmin, char nCommandTrigger, bool nHoldFriends, bool nHoldClan, bool nPublicCommands, unsigned char nWar3Version, QByteArray nEXEVersion, QByteArray nEXEVersionHash, QString nPasswordHashType, QString nPVPGNRealmName, quint32 nMaxMessageLength, quint32 nHostCounterID );
	virtual ~CBNET( );

	QString GetServer( )					{ return m_Server; }
	QString GetServerAlias( )			{ return m_ServerAlias; }
	QString GetCDKeyROC( )				{ return m_CDKeyROC; }
	QString GetCDKeyTFT( )				{ return m_CDKeyTFT; }
	QString GetUserName( )				{ return m_UserName; }
	QString GetUserPassword( )			{ return m_UserPassword; }
	QString GetFirstChannel( )			{ return m_FirstChannel; }
	QString GetCurrentChannel( )			{ return m_CurrentChannel; }
	QString GetRootAdmin( )				{ return m_RootAdmin; }
	char GetCommandTrigger( )			{ return m_CommandTrigger; }
	QByteArray GetEXEVersion( )			{ return m_EXEVersion; }
	QByteArray GetEXEVersionHash( )		{ return m_EXEVersionHash; }
	QString GetPasswordHashType( )		{ return m_PasswordHashType; }
	QString GetPVPGNRealmName( )			{ return m_PVPGNRealmName; }
	quint32 GetHostCounterID( )		{ return m_HostCounterID; }
	bool GetLoggedIn( )					{ return m_LoggedIn; }
	bool GetInChat( )					{ return m_InChat; }
	bool GetHoldFriends( )				{ return m_HoldFriends; }
	bool GetHoldClan( )					{ return m_HoldClan; }
	bool GetPublicCommands( )			{ return m_PublicCommands; }
	quint32 GetOutPacketsQueued( )		{ return m_OutPackets.size( ); }
	QByteArray GetUniqueName( );

	// processing functions

	void ExtractPackets( );
	void ProcessPackets( );
	void ProcessChatEvent( CIncomingChatEvent *chatEvent );

	// functions to send packets to battle.net

	void SendJoinChannel( QString channel );
	void SendGetFriendsList( );
	void SendGetClanList( );
	void QueueEnterChat( );
	void QueueChatCommand( QString chatCommand );
	void QueueChatCommand( QString chatCommand, QString user, bool whisper );
	void QueueGameCreate( unsigned char state, QString gameName, QString hostName, CMap *map, CSaveGame *saveGame, quint32 hostCounter );
	void QueueGameRefresh( unsigned char state, QString gameName, QString hostName, CMap *map, CSaveGame *saveGame, quint32 upTime, quint32 hostCounter );
	void QueueGameUncreate( );

	void UnqueuePackets( unsigned char type );
	void UnqueueChatCommand( QString chatCommand );
	void UnqueueGameRefreshes( );

	// other functions

	bool IsAdmin( QString name );
	bool IsRootAdmin( QString name );
	CDBBan *IsBannedName( QString name );
	CDBBan *IsBannedIP( QString ip );
	void AddAdmin( QString name );
	void AddBan( QString name, QString ip, QString gamename, QString admin, QString reason );
	void RemoveAdmin( QString name );
	void RemoveBan( QString name );
	void HoldFriends( CBaseGame *game );
	void HoldClan( CBaseGame *game );
};

#endif
