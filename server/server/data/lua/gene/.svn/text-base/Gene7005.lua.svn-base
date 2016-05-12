--require("../Gene.lua")
--跟随称号的基因
function OnGene7005Add(iEntity,nParam1,nParam2,szPara)
    AddRoomTop(iEntity,1);
    return 1;
end

function OnGene7005Delete(iEntity,nParam1,nParam2,szPara)
    DeleteRoomTop(iEntity,0);
end

RegisterGeneScript(7005,OnGene7005Add,OnGene7005Delete,GeneType_Equip);