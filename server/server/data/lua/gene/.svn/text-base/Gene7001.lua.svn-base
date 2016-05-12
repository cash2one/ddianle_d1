--require("../Gene.lua")
--用在装备上的增加经验百分比的基因
function OnGene7001Add(iEntity,nParam1,nParam2,szPara)
    ModifyRoleExpGain(iEntity,nParam1);
    return 1;
end

function OnGene7001Delete(iEntity,nParam1,nParam2,szPara)
    ModifyRoleExpGain(iEntity,nParam1*(-1));
end

RegisterGeneScript(7001,OnGene7001Add,OnGene7001Delete,GeneType_Equip);