/*
 * Pet.h
 * Description: 
 * Copyright (C) 2015 ddianle Inc. All rights reserved.
 * Author: XieJiangBei
 * Create time: 2015-6-13 11:31:01
 */

#ifndef __PET_H__
#define __PET_H__

#include "../Macros/Macros.h"
#include "PetConfig/PetBaseInfo.h"
#include "PetConfig/PetQualityInfo.h"
#include "PetConfig/PetEvolutionInfo.h"
#include "PetConfig/PetSkillConfig.h"

#include <set>
#include <string>

struct PetDB;

enum ePetState
{
    ePetState_Rest = 0,         // ��Ϣ
    ePetState_Carrying,         // Я��
    ePetState_Destroy,          // �ֽ�

    ePetState_Max
};

/**
 * ���＼��ʵ��
 */
class PetSkill
{
public:
    PetSkill();
    virtual ~PetSkill();

public:
    // virtual void OnCast() = 0; // ���ڵ����輼��ȫ�Ǳ������ܣ��ݲ���Ҫ�˷������������������ܺ���Ҫ����

protected:
    CLASS_PROPERTY(int, m_nSkillIndex, SkillIndex);
    CLASS_PROPERTY(SkillID, m_nSkillID, SkillID);
    CLASS_PROPERTY(int, m_nLevel, Level);
    CLASS_PROPERTY(int, m_nSkillValue, SkillValue);
};

/**
* Pet:
* 
*/
class Pet
{
protected:
    Pet();

public:
    CLASS_CREATE_FUNC(Pet);
    virtual ~Pet();

public:
    void SetTypeID(unsigned short nTypeID);
    void SetPotentialID(unsigned char nPotentialID);

    bool IsResting() const;
    bool IsCarrying() const;
    bool IsDestroyed() const;

    void SetState(unsigned char nState);

    bool CanImproveQuality() const;
    unsigned char GetMaxQuality() const;
    void ImproveQuality();

    bool CanEvolution() const;
    unsigned char GetMaxEvolutionTimes() const;
    void Evolution();

    unsigned short GetMaxEndurance() const;

    bool CanLevelUp() const;
    unsigned short GetTopLevel() const;
    void LevelUp();
    unsigned int GetMaxExpOfLevel() const;

    void PrepareSkillIndex();
    bool HasSkill(SkillID nSkillID) const;
    bool HasSkill(SkillType nSkillType) const;
    bool IsActivatedSkill(const PetSkill &skill) const;
    PetSkill *GetSkill(SkillID nSkillID) const;
    PetSkill *GetSkill(SkillType nSkillType) const;
    void SetSkill(SkillID nOldSkillID, PetSkill *skill);
    void RemoveSkill(SkillID nSkillID);
    void RemoveSkill(SkillType nSkillType);
    int GetDanceSkillCount() const;
    int GetFightSkillCount() const;

    void GetSkillList(std::list<PetSkill *> &skillList) const;
    void GetOrderedSkillList(std::list<PetSkill *> &skillList) const; // ��ѯ���ݼ�����λ����õļ����б�
    void GetDanceSkillList(std::list<PetSkill *> &skillList) const;
    void GetActivatedDanceSkillList(std::list<PetSkill *> &skillList) const;
    int GetActivatedSkillCount() const;
    void GetFightSkillList(std::list<PetSkill *> &skillList) const;
    void ClearSkill();
    void ClearSkillExclude(const std::set<SkillID> &excludeSkillList);

    void Init(unsigned int nPetID, unsigned char nState, const std::string &strName, unsigned char nPotentialID,
        unsigned short nLevel, unsigned int nExp, unsigned short nEndurance, unsigned int nEnduranceRecoveryTime,
        unsigned char nAttributeType, unsigned char nQualityType, unsigned char nEvolutionTimes,
        unsigned short nPetTypeID, const std::string &strSkillList, unsigned int nResetSkillTime, int nResetSkillTimes);
    void Init(const PetDB &petDb);
    void ToPetDB(PetDB &petDb);

    void LoadSkill(const std::string &strSkillList);
    void SaveSkill(std::string &strSkillList);

protected:
    unsigned short ResetMaxEndurance() const;

protected:
    CLASS_PROPERTY(unsigned int, m_nID, ID);                            // ����ID�������ݿ�����
    CLASS_PROPERTY_READONLY(unsigned char, m_nState, State);            // ״̬
    CLASS_PROPERTY(unsigned short, m_nLevel, Level);                    // �ȼ�
    CLASS_PROPERTY(unsigned int, m_nExp, Exp);                          // ��ǰ�ȼ�����
    CLASS_PROPERTY(unsigned char, m_nQualityType, QualityType);         // Ʒ������
    CLASS_PROPERTY(unsigned char, m_nEvolutionTimes, EvolutionTimes);   // ��������
    CLASS_PROPERTY(unsigned char, m_nAttributeType, AttributeType);     // ��������
    CLASS_PROPERTY_READONLY(unsigned short, m_nTypeID, TypeID);         // ��������ID, Set������д
    CLASS_PROPERTY_READONLY(unsigned char, m_nPotentialID, PotentialID);// ����Ǳ��, Set������д
    CLASS_PROPERTY_BY_REF(std::string, m_strName, Name);                // ������
    CLASS_PROPERTY(unsigned short, m_nEndurance, Endurance);            // ��ǰ����
    CLASS_PROPERTY(unsigned int, m_nEnduranceRecoveryTime, EnduranceRecoveryTime);// ��ǰ�����ָ�ʱ��
    CLASS_PROPERTY(unsigned int, m_nResetSkillTime, ResetSkillTime);    // ��������ʱ��
    CLASS_PROPERTY(int, m_nResetSkillTimes, ResetSkillTimes);           // ���켼�����ô���

protected:
    std::map<SkillID, PetSkill *> m_petSkill;                           // �ó�����еļ����б� -- ���ָ���Է�����ڼ��ܹ�����չ
    std::map<int, SkillID> m_petSkillIndex;                             // ���＼������Ӧ�ļ���ID�����������->����ID

    unsigned short m_nMaxEndurance;                                     // ����������ڴ�ֵ����д�����ݿ�

    const PetBaseConfig *m_pBaseConfig;
    const PetPotentialConfig *m_pPotentialConfig;
    const PetQualityInfo *m_pQualityConfig;
    const PetEvolutionInfo *m_pEvolutionConfig;
};

#endif // __PET_H__

