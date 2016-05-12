--require("../Gene.lua")
--增加金钱百分比的基因
function OnGene1101Add(iEntity,nParam1,nParam2,szPara)
    ModifyRoleMoneyGain(iEntity,nParam1*-1);
    return 1;
end

function OnGene1101Delete(iEntity,nParam1,nParam2,szPara)
	ModifyRoleMoneyGain(iEntity,nParam1*-1);
end

RegisterGeneScript(1101,OnGene1101Add,OnGene1101Delete,GeneTyp_Duration);