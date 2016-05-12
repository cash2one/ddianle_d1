#ifndef	__FRIENDDEFINE_H__
#define __FRIENDDEFINE_H__



const	int   MAX_RECENTLY_PLAYMATE = 20;
const	int   MAX_RECENTLY_CHATMATE = 20;

enum	QueryType
{
	NotifiedAddFriend = 0,
	AddFriendFromFollowedMuteList,
	AddFriendFromMuteList,
	AddFriendFromBlackList,	

	AddFriendFromStrangers,
	AddFriendFromFollowedList,

	AddBlackList,
	AddMuteList,

};


#endif




