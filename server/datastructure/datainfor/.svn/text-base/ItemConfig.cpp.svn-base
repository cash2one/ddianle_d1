#include "./ItemConfig.h"
#include "./FileStream.h"
#include "../../socket/Utility.h"
#include "../../socket/Windefine.h"

void CheckReturn(string &str);

GeneInfo::GeneInfo()
    :m_nGeneID(0)
    ,m_nParam1(0)
    ,m_nParam2(0)
    ,m_strParam("")
{
}

GeneInfo::~GeneInfo()
{

}

GeneInfo & GeneInfo::operator = (const GeneInfo& rhs)
{
    if(this == &rhs) return *this;

    m_nGeneID = rhs.m_nGeneID;
    m_nParam1 = rhs.m_nParam1;
    m_nParam2 = rhs.m_nParam2;
    m_strParam = rhs.m_strParam;

    return *this;
}


ItemConfig::ItemConfig(void)
    :m_nType(0)
    ,m_strName("")
    ,m_SexNeed(ESexType_No)
    ,m_nDueTime(0)
    ,m_nUseTimes(0)
    ,m_nNeedLevel(0)
    ,m_bUnique(false)
    ,m_nMaxStackCount(1)
    ,m_nCoolDownTag(0)
    ,m_nCooldownTime(0)
    ,m_strIcon("")
    ,m_strAtlas("")
    ,m_strAnimation("")
    ,m_nClothEffectID(0)
    ,m_nVIP(0)
    ,m_nIntimacy(0)
    ,m_strIntro("")
    , m_bIsFresher(false)
    ,m_bTransform(false)
    ,m_bOldEffectCloth(false)
{
    m_anType[0] = 0;
    m_anType[1] = 0;
    m_anType[2] = 0;
}

ItemConfig::~ItemConfig(void)
{
}

bool ItemConfig::Load(CFileStream&file)
{
    file.ReadUInt(m_nType);

    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strName,nSize);

    file.ReadUChar(m_anType[0]);
    file.ReadUChar(m_anType[1]);
    file.ReadUChar(m_anType[2]);

    short lTemp = 0;
    file.ReadShort(lTemp);
    m_SexNeed = (ESexType)lTemp;

    file.ReadInt(m_nDueTime);
    file.ReadShort(m_nUseTimes);

    file.ReadUShort(m_nNeedLevel);
    file.ReadBool(m_bUnique);	
    file.ReadUShort(m_nMaxStackCount);

    file.ReadUShort(m_nCoolDownTag);
    file.ReadInt(m_nCooldownTime);

    file.ReadUShort(nSize);
    file.ReadString(m_strIcon,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strAtlas,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strAnimation,nSize);

    file.ReadUInt(m_nClothEffectID);
    file.ReadUChar(m_nClothColor);
    file.ReadBool(m_bOldEffectCloth);

    file.ReadUShort(m_nVIP);
    file.ReadInt(m_nIntimacy);


    file.ReadUShort(nSize);
    file.ReadString(m_strIntro,nSize);
    if(m_strIntro.size()==1)
        m_strIntro.clear();
    CheckReturn(m_strIntro);
    file.ReadBool(m_bIsFresher);

    unsigned short nCount = 0;
    file.ReadUShort(nCount);
    for(unsigned i = 0;i<nCount;i++)
    {
        GeneInfo * info = new GeneInfo();
        if (info != NULL)
        {
            file.ReadUShort(info->m_nGeneID);
            file.ReadInt(info->m_nParam1);
            file.ReadInt(info->m_nParam2);
            file.ReadUShort(nSize);
            file.ReadString(info->m_strParam,nSize);

            if ( info->m_nGeneID == Gene_ID_Transform )
            {
                m_bTransform = true;//是变身道具
            }

            m_listGene.push_back(info);
        }
    }

    if(m_anType[0] == eItemClassType_Equip){
        ASSERT(m_nUseTimes<=0);
    }

    ASSERT(m_nMaxStackCount>=1);
    ASSERT(m_nUseTimes<=1||m_nMaxStackCount==1);

    return true;
};			


bool ItemConfig::Save(CFileStream&file)
{
    ASSERT(m_nMaxStackCount>=1);
    ASSERT(m_nUseTimes<=1||m_nMaxStackCount==1);

    if(m_anType[0] == eItemClassType_Equip){
        ASSERT(m_nUseTimes<=0);
    }

    file.WriteUInt(m_nType);
    file.WriteUShort((unsigned short)m_strName.length());
    file.WriteString(m_strName);

    file.WriteUChar(m_anType[0]);
    file.WriteUChar(m_anType[1]);
    file.WriteUChar(m_anType[2]);

    short lTemp = 0;
    lTemp = (short)m_SexNeed;
    file.WriteShort(lTemp);

    file.WriteInt(m_nDueTime);
    file.WriteShort(m_nUseTimes);
    file.WriteUShort(m_nNeedLevel);
    file.WriteBool(m_bUnique);
    file.WriteUShort(m_nMaxStackCount);

    file.WriteUShort(m_nCoolDownTag);
    file.WriteInt(m_nCooldownTime);	


    file.WriteUShort((unsigned short)m_strIcon.length());
    file.WriteString(m_strIcon);

    file.WriteUShort((unsigned short)m_strAtlas.length());
    file.WriteString(m_strAtlas);

    file.WriteUShort((unsigned short)m_strAnimation.length());
    file.WriteString(m_strAnimation);

    file.WriteUInt(m_nClothEffectID);
    file.WriteUChar(m_nClothColor);
    file.WriteBool(m_bOldEffectCloth);

    file.WriteUShort(m_nVIP);
    file.WriteInt(m_nIntimacy);

    file.WriteUShort((unsigned short)m_strIntro.length());
    file.WriteString(m_strIntro);

    file.WriteBool(m_bIsFresher);


    file.WriteUShort((unsigned short)m_listGene.size());
    list<GeneInfo *> ::iterator aBegin = m_listGene.begin();
    while (aBegin != m_listGene.end())
    {
        GeneInfo * info = (*aBegin);
        if (info != NULL)
        {
            file.WriteUShort(info->m_nGeneID);
            file.WriteInt(info->m_nParam1);
            file.WriteInt(info->m_nParam2);
            file.WriteUShort((unsigned short)info->m_strParam.length());
            file.WriteString(info->m_strParam);
        }
        aBegin++;
    }
    return true;
};

itemtype_t ItemConfig::GetType() const
{
    return m_nType;
}

EItemColumn ItemConfig::GetItemColumnByType()
{
    switch(m_anType[0])
    {
    case eItemClassType_Equip:
        if (IsCloth()|| IsHorse())
        {
            return EItemColumn_ClothBag;
        }
        else if (IsBadge())
        {
            return EItemColumn_BadgeBag;
        }
    case eItemClassType_Expendable:
        return EItemColumn_ExpandableBag;
    default:
        return EItemColumn_Invalid;
    }
}

bool ItemConfig::IsEquip() const
{
    return m_anType[0] == eItemClassType_Equip;
}

bool ItemConfig::IsExpandable() const
{
    return m_anType[0] == eItemClassType_Expendable;
}

/////////////////////////////////////////////////////////////
//二级类型判断
/////////////////////////////////////////////////////////////

//服饰
bool ItemConfig::IsCloth() const
{
    return IsEquip()&&(m_anType[1] == eItemEquipType_Cloth);
}

//徽章
bool ItemConfig::IsBadge() const
{
    return IsEquip()&&(m_anType[1] == eItemEquipType_Badge);
}

//坐骑
bool ItemConfig::IsHorse() const
{
    return IsEquip()&&(m_anType[1] == eItemEquipType_Horse);
}

//功能类道具
bool ItemConfig::IsFunctionExpandable() const
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_Function);
}

//社交类道具
bool ItemConfig::IsSocialExpandable() const
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_Social);
}

//增益类道具
bool ItemConfig::IsAdditionExpandable() const
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_Addition);
}

bool ItemConfig::IsPackage() const
{
    return IsPacketExpandable() || IsBoxExpandable() || IsDynamicBoxExpandable();
}

//礼包类道具
bool ItemConfig::IsPacketExpandable() const
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_Packet);
}

//宝箱类道具
bool ItemConfig::IsBoxExpandable() const 
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_Box);
}

//动态宝箱类道具
bool ItemConfig::IsDynamicBoxExpandable() const
{
    return IsExpandable()&&(m_anType[1] == eItemExpendableType_DynamicBox);
}

//宠物蛋
bool ItemConfig::IsPetEgg() const
{
    return IsExpandable() && (m_anType[1] == eItemExpendableType_PetEgg);
}

bool ItemConfig::IsEffectStone() const
{
    return IsExpandable() && (m_anType[1] == eItemExpendableType_EffectStone);
}

/////////////////////////////////////////////////////////////
//三级类型判断
/////////////////////////////////////////////////////////////
unsigned char ItemConfig::GetClothPos() const 
{
    if(IsCloth())
    {
        return m_anType[2];
    }

    return 0;
}

bool ItemConfig::IsBadge_Type_WeddingRing() const
{
    return IsBadge() && ( m_anType[2] == eItemBadgeType_WingdingRing );
}

bool ItemConfig::IsItemCloth_Type_Skin() const
{
    return IsCloth() && ( m_anType[2] == eItemClothType_Skin);
}

bool ItemConfig::IsItemCloth_Type_Suit() const
{
    return IsCloth() && ( m_anType[2] == eItemClothType_Suit);
}

bool ItemConfig::IsUnique() const
{
    return m_bUnique;
}

CSkinInfo::CSkinInfo()
{
    m_nType = 0;
    m_strName = "";
    m_cR = 1;
    m_cG = 1;
    m_cB = 1;
    m_cA = 1;
}

CSkinInfo::~CSkinInfo()
{

}

bool CSkinInfo::Load(CFileStream&file)
{
    file.ReadUInt(m_nType);

    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strName,nSize);

    file.ReadUChar(m_cR);
    file.ReadUChar(m_cG);
    file.ReadUChar(m_cB);
    file.ReadUChar(m_cA);

    return true;
}

bool CSkinInfo::Save(CFileStream&file)
{
    file.WriteUInt(m_nType);

    file.WriteUShort((unsigned short)m_strName.length());
    file.WriteString(m_strName);

    file.WriteUChar(m_cR);
    file.WriteUChar(m_cG);
    file.WriteUChar(m_cB);
    file.WriteUChar(m_cA);

    return true;
}

CItemAniInfo ::CItemAniInfo()
{
    m_nType = 0;
    m_nMoveState = 0;
    m_strPlayerIdleAni = "";
    m_strPlayerMoveAni = "";
    m_strWingMoveAni = "";
}

CItemAniInfo ::~CItemAniInfo()
{

}

bool CItemAniInfo::Load(CFileStream&file)
{
    file.ReadUShort(m_nType);
    file.ReadUChar(m_nMoveState);

    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strPlayerIdleAni,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strPlayerMoveAni,nSize);

    file.ReadUShort(nSize);
    file.ReadString(m_strWingMoveAni, nSize);

    return true;
}

bool CItemAniInfo::Save(CFileStream&file)
{
    file.WriteUShort(m_nType);
    file.WriteUChar(m_nMoveState);

    file.WriteUShort((unsigned short)m_strPlayerIdleAni.length());
    file.WriteString(m_strPlayerIdleAni);

    file.WriteUShort((unsigned short)m_strPlayerMoveAni.length());
    file.WriteString(m_strPlayerMoveAni);

    file.WriteUShort((unsigned short)m_strWingMoveAni.length());
    file.WriteString(m_strWingMoveAni);

    return true;
}

CItemTransformInfo::CItemTransformInfo()
{
    m_nId = 0;
    memset(m_szItemCloth, 0, sizeof(m_szItemCloth));
}

CItemTransformInfo::~CItemTransformInfo()
{

}

bool CItemTransformInfo::Load( CFileStream&file )
{
    file.ReadUInt(m_nId);
    unsigned short nSize = 0;
    file.ReadUShort(nSize);
    file.ReadString(m_strName, nSize);
    for (int i = 0; i < eItemClothType_Max; ++i)
    {
        file.ReadUInt(m_szItemCloth[i]);
    }
    return true;
}

bool CItemTransformInfo::Save( CFileStream&file )
{
    file.WriteUInt(m_nId);
    file.WriteUShort((unsigned short)m_strName.length());
    file.WriteString(m_strName);
    for (int i = 0; i < eItemClothType_Max; ++i)
    {
        file.WriteUInt(m_szItemCloth[i]);
    }
    return true;
}

CTransformAniInfo::CTransformAniInfo()
{
    m_nTransformId = 0;
}

bool CTransformAniInfo::Save(CFileStream &file)
{
    file.WriteInt(m_nTransformId);
    file.WriteString2(m_strWomanAniName);
    file.WriteString2(m_strManAniName);
    return true;
}

//end file



