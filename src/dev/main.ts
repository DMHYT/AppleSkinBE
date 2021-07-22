//TODO
Callback.addCallback("ServerPlayerTick", function(player, isPlayerDead){

    //if(Game.getGameMode() != 1 && Game.getGameMode() != 3){
    //работа 
    var item = Player.getCarriedItem();
    var getHunger = Player.getHunger();
    var hungerX = getHunger + item.id;
    var getSatur = Player.getSaturation();
    var saturX = getSatur + item.id;
    if(hungerX < 0){hungerX = 0};
    if(hungerX > 20){hungerX = 20};
    if(saturX < 0){saturX = 0};
    if(saturX > 20){saturX = 20};
    
    var Satur = Math.round(saturX);
    var Satur1 = Satur / 2;
    var SaturationFull = Math.floor(Satur1)
    var Satur2 = Satur1 - SaturationFull;
    var Satur3 = Math.ceil(Satur2);
    var SaturationHalf = Satur3 + SaturationFull;
    
    var hunger = Math.round(hungerX);
    var hunger1 = hunger / 2;
    var hungerFull = Math.floor(hunger1);
    var hunger2 = hunger1 - hungerFull;
    var hunger3 = Math.ceil(hunger2);
    var hungerHalf = hunger3 + hungerFull;
    
    //время
    var a = World.getThreadTime();
    var b = a / 100;
    var c = Math.trunc(b);
    var d = b - c;
    var e = d * 100;
    var time = Math.trunc(e)
    
    function setAnimation(Cadr){
    for(var a = 0; a < 11; a++){
    //HangerFull
    AppleSkinContainer.setScale("HungerFull_"+a, 0);
    
    //HungerHalf  
    AppleSkinContainer.setScale("HungerHalf_"+a, 0);
    
    //SaturationFull
    AppleSkinContainer.setScale("SaturationFull_"+a, 0);
    
    //SaturationHalf
    AppleSkinContainer.setScale("SaturationHalf_"+a, 0);};
    
    //setAnimation
    AppleSkinContainer.setScale("HungerFull_"+Cadr, hungerFull / 10);
    AppleSkinContainer.setScale("HungerHalf_"+Cadr, hungerHalf / 10);
    AppleSkinContainer.setScale("SaturationFull_"+Cadr, SaturationFull / 10);
    AppleSkinContainer.setScale("SaturationHalf_"+Cadr, SaturationHalf/ 10);
    };
    
    //истощение
    var exhaustion = Player.getExhaustion()
    AppleSkinContainer.setScale("Exhaustion", exhaustion / 4);
    
    //анимация
    if(item.id != 0){
    switch(time){
    case 0:
    case 50:
    setAnimation(0);
    break;
    case 2:
    case 48:
    case 52:
    case 98:
    setAnimation(1);
    break;
    case 5:
    case 45:
    case 55:
    case 95:
    setAnimation(2);
    break;
    case 7:
    case 43:
    case 57:
    case 93:
    setAnimation(3);
    break;
    case 10:
    case 40:
    case 60:
    case 90:
    setAnimation(4);
    break;
    case 12:
    case 38:
    case 62:
    case 88:
    setAnimation(5);
    break;
    case 15:
    case 35:
    case 65:
    case 85:
    setAnimation(6);
    break;
    case 17:
    case 33:
    case 67:
    case 83:
    setAnimation(7);
    break;
    case 20:
    case 30:
    case 70:
    case 80:
    setAnimation(8);
    break;
    case 22:
    case 28:
    case 72:
    case 78:
    setAnimation(9);
    break;
    case 25:
    case 75:
    setAnimation(10);
    break;};
    };
    
    if(item.id == 0){
    for(var i; i < 11; i++){
    AppleSkinContainer.setScale("HungerFull_"+i, 0);
    AppleSkinContainer.setScale("HungerHalf_"+i, 0);
    AppleSkinContainer.setScale("SaturationFull_"+i, 0);
    AppleSkinContainer.setScale("SaturationHalf_"+i, 0);
    };
    };
    //};
    });
    
    
    Callback.addCallback("ItemUse", function(coords, item, block, isExternal, player){
    var client = Network.getClientForPlayer(player);
    if (client != null){
    if (item.id == 280){
    Player.setHunger(9);
    Player.setSaturation(11);
    };
    };
    });