--require("../Gene.lua")
--增加金钱值的基因
function OnGene1102Add(iEntity,nParam1,nParam2,szPara)
    ModifyRoleMoney(iEntity,nParam1);
    return 1;
end

function OnGene1102Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(1102,OnGene1102Add,OnGene1102Delete,GeneType_OneTime);