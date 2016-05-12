--require("../Gene.lua")
--房间名变色的基因
function OnGene7004Add(iEntity,nParam1,nParam2,szPara)
    SetRoomColor(iEntity,nParam1);
    return 1;
end

function OnGene7004Delete(iEntity,nParam1,nParam2,szPara)
    SetRoomColor(iEntity,0);
end

RegisterGeneScript(7004,OnGene7004Add,OnGene7004Delete,GeneType_Equip);