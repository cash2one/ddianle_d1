--require("../Gene.lua")
--增加经验百分比的基因
function OnGene1001Add(iEntity,nParam1,nParam2,szPara)
    ModifyRoleExpGain(iEntity,nParam1);
    return 1;
end

function OnGene1001Delete(iEntity,nParam1,nParam2,szPara)
    ModifyRoleExpGain(iEntity,nParam1*-1);
end

RegisterGeneScript(1001,OnGene1001Add,OnGene1001Delete,GeneTyp_Duration);