--require("../Gene.lua")
--增加经验值的基因
function OnGene1002Add(iEntity,nParam1,nParam2,szPara)
    AddRoleExp(iEntity,nParam1);
    return 1;
end

function OnGene1002Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(1002,OnGene1002Add,OnGene1002Delete,GeneType_OneTime);