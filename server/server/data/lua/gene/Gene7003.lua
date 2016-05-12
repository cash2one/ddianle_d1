--require("../Gene.lua")
--发言变色的基因
function OnGene7003Add(iEntity,nParam1,nParam2,szPara)
   RoleSetCurChatColor(iEntity,nParam1);
    return 1;
end

function OnGene7003Delete(iEntity,nParam1,nParam2,szPara)
    RoleSetCurChatColor(iEntity,0);
end

RegisterGeneScript(7003,OnGene7003Add,OnGene7003Delete,GeneType_Equip);