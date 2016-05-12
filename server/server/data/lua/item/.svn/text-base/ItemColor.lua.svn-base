local color={"default" ,"紫色","蓝色","绿色"}
local color_int={};
color_int[color_default]={39362,39362}
color_int[color_purple]={47580,47580}
color_int[color_blue]={19551,19551}
color_int[color_green]={11829,11829}

function modifycolor( RoleIndex ,colorindex )
    local result = nil
  local bResult = 0   
  local itemGene = 108
  local pGene108 ,nParam108,nDuration108 = FindRoleGene( RoleIndex , 108 )
    if pGene108 ~= nil then
        if nParam108 ~= colorindex then
            SetRoleGeneParam(RoleIndex,itemGene,nDuration108,colorindex)
            RoleSetChatColor(RoleIndex , color_int[colorindex][1] , color_int[colorindex][2]) 
            bResult =1
            result = "您的昵称及发言内容颜色修改为"..color[colorindex]
        else
            bResult = 0;
            result = "您已经是"..color[colorindex].."昵称了"
        end
    else
        if RoleAddGene( RoleIndex,itemGene,GeneTyp_Duration,days30,colorindex,"" ) then 
            bResult = 1;
            result = "您的昵称及发言内容颜色修改为"..color[colorindex]
        else
            result = "物品使用失败"
        end
    end
    return bResult.."",result;
end

function Item_10013( RoleIndex )
    return modifycolor(RoleIndex,color_purple)
end
function Item_10014( RoleIndex )
    return modifycolor(RoleIndex,color_blue)
end

function Item_10015( RoleIndex )
    return modifycolor(RoleIndex,color_green)
end

RegisterItemUse(10013,Item_10013)
RegisterItemUse(10014,Item_10014)
RegisterItemUse(10015,Item_10015)


	public override bool doDecode(NetReadBuffer DataIn)
	{
		if(null != DataIn)
		{
			m_nRoleID = DataIn.GetUInt() ;
			m_strName = DataIn.GetPerfixString() ;
			m_nSex = DataIn.GetByte() ;
			m_nLevel = DataIn.GetUShort() ;
		}
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_RecvFriendApplication();
	}
	




public class GameMsg_S2C_AddFriend : GameMsgBase
{
	public FriendInfo m_pFriendInfo ;
	public GameMsg_S2C_AddFriend() : base((ushort)GameMsgType.MSG_S2C_FriendApplication)
	{
		if(null == m_pFriendInfo)
		{
			m_pFriendInfo = new FriendInfo() ;
		}
	}
	public override bool doDecode(NetReadBuffer DataIn)
	{
		if(null != m_pFriendInfo)
		{
			m_pFriendInfo.m_nRoleID = DataIn.GetUInt() ;
			m_pFriendInfo.m_nSex = DataIn.GetByte() ;
			m_pFriendInfo.m_strRoleName = DataIn.GetPerfixString() ;
			m_pFriendInfo.m_nLine = DataIn.GetUShort() ;
			m_pFriendInfo.m_nRoomID = DataIn.GetUShort() ;
		}
		return true ;
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_AddFriend();
	}
}

//被加为接受好友收到的消息
public class GameMsg_S2C_RecvFriendApplication : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strName ;
	public byte m_nSex;
	public ushort m_nLevel ;
	
	public GameMsg_S2C_RecvFriendApplication() : base((ushort)GameMsgType.MSG_S2C_ApplicationResult)
	{
		m_nRoleID = 0 ;
		m_strName = "" ;
		m_nSex = 0 ;
		m_nLevel = 0 ;
	}
	public override bool doDecode(NetReadBuffer DataIn)
	{
		if(null != DataIn)
		{
			m_nRoleID = DataIn.GetUInt() ;
			m_strName = DataIn.GetPerfixString() ;
			m_nSex = DataIn.GetByte() ;
			m_nLevel = DataIn.GetUShort() ;
		}
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_RecvFriendApplication();
	}
}

//接受好友
public class GameMsg_C2S_AcceptApplication : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strName ;
	public byte m_nSex ;
	public ushort m_nLevel ;
	public GameMsg_C2S_AcceptApplication() : base((ushort)GameMsgType.MSG_C2S_AcceptApplication)
	{
		m_nRoleID = 0 ;
		m_strName = "" ;
		m_nSex = 0 ;
		m_nLevel = 0 ;
	}
	public override bool doEncode(NetWriteBuffer DataOut)
	{
		if(null != DataOut)
		{
			DataOut.PutUShort(m_nRoleID) ;
			DataOut.PutString(m_strName) ;
			DataOut.PutByte(m_nSex) ;
			DataOut.PutUShort(m_nLevel) ;
		}
	}
}

public class GameMsg_S2C_AcceptApplication : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strRoleName ;
	public byte m_nSex ;
	public ushort m_nLevel ;
	public GameMsg_S2C_AcceptApplication() : base((ushort)GameMsgType.MSG_S2C_AcceptApplicationResult)
	{
		m_nRoleID = 0 ;
		m_strRoleName = "" ;
		m_nSex = 0 ;
		m_nLevel = 0 ;
	}
	public override bool doDecode(NetReadBuffer DataIn)
	{
		if(null != DataIn)
		{
			m_nRoleID = DataIn.GetUInt() ;
			m_strName = DataIn.GetPerfixString() ;
			m_nSex = DataIn.GetByte() ;
			m_nLevel = DataIn.GetUShort() ;
		}
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_AcceptApplication();
	}
}

//拒绝好友申请
public class GameMsg_C2S_RefuseApplication : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strName ;
	public GameMsg_C2S_RefuseApplication() : base((ushort)GameMsgType.MSG_C2S_RefuseApplication)
	{
		m_nRoleID = 0 ;
		m_strName = "" ;
	}
	public override bool doEncode(NetWriteBuffer DataOut)
	{
		if(null != DataOut)
		{
			DataOut.PutUShort(m_nRoleID) ;
			DataOut.PutString(m_strName) ;
		}
	}
}



//拉黑
public class GameMsg_C2S_AddFriendBlackList : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strName ;
	public byte m_nSex ;
	public ushort m_nLevel ;
	public GameMsg_C2S_AddFriendBlackList() : base((ushort)GameMsgType.MSG_C2S_AddBlackList)
	{
		m_nRoleID = 0 ;
		m_strName = "" ;
		m_nSex = 0 ;
		m_nLevel = 0 ;
	}
	public override bool doEncode(NetWriteBuffer DataOut)
	{
		DataOut.PutUInt(m_nRoleID) ;
		DataOut.PutString(m_strName) ;
		DataOut.PutByte(m_nSex) ;
		DataOut.PutUShort(m_nLevel) ;
		return true ;
	}
}

public class GameMsg_S2C_AddFriendBlackList : GameMsgBase
{
	public uint m_nRoleID ;
	
	public GameMsg_S2C_AddFriendBlackList() : base((ushort)GameMsgType.MSG_S2C_AddBlackListResult)
	{
		m_nRoleID = 0  ;
	}
	
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_AddFriendBlackList();
	}
	
	public override bool doDecode(NetReadBuffer DataIn)
	{
		m_nRoleID = DataIn.GetUInt() ;
		return true ;
	}
}

//删除好友
public class GameMsg_C2S_DeleteFriend : GameMsgBase
{
	public uint m_nRoleID ;
	public GameMsg_C2S_DeleteFriend(): base((ushort)GameMsgType.MSG_C2S_RemoveFriend)
	{
		m_nRoleID = 0 ;
	}
	public override bool doEncode(NetWriteBuffer DataOut)
	{
		DataOut.PutUInt(m_nRoleID) ;
		return true ;
	}
}

public class GameMsg_S2C_DeletFriend : GameMsgBase
{
	public uint m_nRoleID ;
	public GameMsg_S2C_DeletFriend() : base((ushort)GameMsgType.MSG_S2C_RemoveFriendResult) 
	{
		m_nRoleID = 0 ;
	}
	public override bool doDecode(NetReadBuffer DataOut)
	{
		m_nRoleID = DataOut.GetUInt();
		return true ;
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_DeletFriend();
	}
}

//从黑名单删除好友
public class GameMsg_C2S_DeleteFromFriendBlack : GameMsgBase
{
	public uint m_nRoleID ;
	public GameMsg_C2S_DeleteFromFriendBlack() : base((ushort)GameMsgType.MSG_C2S_RemoveBlackList)
	{
		
	}
	public override bool doEncode(NetReadBuffer DataOut)
	{
		DataOut.PutUInt(m_nRoleID) ;
		return true ;
	}
}

public class GameMsg_S2C_DeleteFromFriendBlack : GameMsgBase
{
	public uint m_nRoleID ; //被删除的好友的ID
	
	public GameMsg_S2C_DeleteFromFriendBlack() : base((ushort)GameMsgType.MSG_S2C_RemoveBlackListResult)
	{
		m_nRoleID = 0 ;
	}
	public override bool doDecode(NetReadBuffer DataIn)
	{
		m_nRoleID = DataIn.GetUInt() ;
		return true ;
	}
	public static  GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_DeleteFromFriendBlack();
	}
}

//更新好友列表
public class GameMsg_C2S_GetFriendOnLineInfo : GameMsgBase
{
	public GameMsg_C2S_GetFriendOnLineInfo() : base((ushort)GameMsgType.GameMsg_C2S_GetFriendOnLineInfo)
	{
		
	}
	public override bool doEncode()
	{
		return true ;
	}
}

public class GameMsg_S2C_GetFriendOnLineInfoResult : GameMsgBase
{
	//public FriendInfo m_pFriendInfo;
	public uint m_nCount ;
	public List<FriendInfo> m_pListFriendInfo ;
	public GameMsg_S2C_GetFriendOnLineInfoResult() : base((ushort)GameMsgType.MSG_S2C_GetFriendOnlineInfoReuslt)
	{
		if(null == m_pListFriendInfo)
		{
			m_pListFriendInfo = new List<FriendInfo>() ;
		}
		m_nCount = 0 ;
	}
	public static GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_GetFriendOnLineInfoResult();
	}
	public override bool doDecode(NetReadBuffer DataOut)
	{
		m_nCount = DataOut.GetUInt() ;
		for(uint i = 0 ; i < m_nCount ; i++)
		{
			FriendInfo pFriendInfo = new FriendInfo() ;
			pFriendInfo.m_nRoleID = DataOut.GetUInt() ;
			pFriendInfo.m_nLine = DataOut.GetUInt() ;
			pFriendInfo.m_Stat = DataOut.GetByte() ;	
			pFriendInfo.m_nLine = DataOut.GetInt() ;
			if(null != m_pListFriendInfo)
			{
				m_pListFriendInfo.Add(pFriendInfo) ;
			}
		}
		return true ;
	}
}

//update friend'Lev and name 
public class GameMsg_S2C_UpdateFriendInfo : GameMsgBase
{
	public uint m_nRoleID ;
	public string m_strName ;
	public ushort m_nLev ;
	
	public GameMsg_S2C_UpdateFriendInfo() : base((ushort)GameMsgType.MSG_S2C_UpdateFriendInfo)
	{
		m_nRoleID = 0 ;
		m_strName = "" ;
		m_nLev = 0 ;
	}
	
	public override bool doDecode(NetReadBuffer DataOut)
	{
		m_nRoleID = DataOut.GetUInt() ;
		m_strName  = DataOut.GetPerfixString() ;
		m_nLev = DataOut.GetUShort() ;
		return true ;
	}
	
	public static GameMsgBase CreateMsg()
	{
		return new GameMsg_S2C_UpdateFriendInfo();
	}
}

public int m_nLine ;
	public uint m_nRoomID ;
	public byte   m_nSex ;
	public byte   m_Stat ; //在大厅，在房间
