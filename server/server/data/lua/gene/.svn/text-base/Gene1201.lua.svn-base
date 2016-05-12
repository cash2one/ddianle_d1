--require("../Gene.lua")
--增加自己人气值的基因
function OnGene1201Add(iEntity,nParam1,nParam2,szPara)
    AddRoleHot(iEntity,nParam1);
    return 1;
end

function OnGene1201Delete(iEntity,nParam1,nParam2,szPara)
end

RegisterGeneScript(1201,OnGene1201Add,OnGene1201Delete,GeneType_OneTime);