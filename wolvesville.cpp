#include<iostream>
using namespace std;
#include<string>
#include<stdlib.h>
#include<time.h>
#define byte win_byte_override
#include <windows.h>
#undef byte

enum RoleType{
  Werewolves,
  Village,
  Solo,
  Killer
};
enum Aura{
  Evil,
  Good,
  UnknownA
};
enum Kill{
  Lynch,
  Wolves,
  Arson,
  BeastHunter,
  Avenger,
  JrWolf,
  Serial,
  Cupid
};
enum RoleID{
  VILLAGER,
  SEER,
  AURA_SEER,
  DETECTIVE,
  DOCTOR,
  PRIEST,
  MEDIUM,
  AVENGER,
  CUPID,
  BEAST_HUNTER,
  CURSED,
  WEREWOLF,
  WOLF_SEER,
  JUNIOR_WOLF,
  NIGHTMARE_WOLF,
  GUARDIAN_WOLF,
  HEADHUNTER,
  FOOL,
  ARSONIST,
  SERIAL_KILLER
};

class Roles{
  public:
  string name;
  string description;
  string icon;
  RoleType team;
  Aura aura;
  int id;
  bool dayAbility;
  bool nightAbility;

  Roles(){};
  Roles(int i,string ic, string n, string d, RoleType t, Aura a, bool nA=false, bool dA=false){
    id=i;
    icon=ic;
    name=n;
    description=d;
    team=t;
    aura=a;
    dayAbility=dA;
    nightAbility=nA;
  }

};

class Player{
  public:
  int id;
  int killedBy;
  int votes;
  int voted;
  string name;
  Roles role;
  bool isAlive;
  bool isDoused;
  bool isNightmared;
  bool isProtected;
  bool isCoupled;
  bool roleAssigned;
  bool roleRevealed;
  bool deathProc;
  void kill();
  void revive();

  Player(){};
  Player(string s,int i){
    id=i;
    name = s;
    killedBy = -1;
    roleAssigned = false;
    roleRevealed = false;
    isAlive = true;
    deathProc = false;
    votes = 0;
    isDoused = false;
    isNightmared = false;
    isProtected = false;
    isCoupled = false;
  };
};

class Game{
  private:
  int hhTarget,hh;
  int jrWolfTarget;
  int avengerTarget;
  int docTarget;
  int beastHunterTarget;
  int seerTarget;
  int auraSeerTarget;
  int det1Target,det2Target;
  int priestTarget;
  int doused;
  int couple1,couple2,cupid;
  int guardian;
  string auraSeerString;

  public:
  int day;
  int night;
  int revivePlayer;
  int beastHunterTrap;
  int nightmares;
  int medHasRevived;
  bool gameRunning;
  bool guardianProtected;
  Roles allRoles[25];
  Roles sandboxVillageRoles[15];
  Roles sandboxWerewolfRoles[5];
  Roles killerRoles[2];
  Roles soloRoles[2];
  Player players[16];
  int alivePlayers(RoleType team);
  int totalAlive();
  void extraKills(int id);
  void extraExtraKills(int id,int flag=1);
  void createRoles();
  void createPlayers();
  void showPlayers(int x=0);
  void assignRoles();
  void beginning();
  void nightRoleActivity();
  void dayRoleActivity();
  void killPhase();
  void wolvesActivity();
  void effectsRemoval();
  void discussion();
  void voting();
  bool winCheck(int id);
  void newGame();

  Game(){
    day=1;
    night=1;
    medHasRevived=0;
    guardianProtected=false;
    beastHunterTrap=0;
    nightmares=2;
    doused=0;
    gameRunning=true;
  }
};

void menu();
void title();

int SleepCD = 50;
int main(){
  srand(time(0));
  SetConsoleOutputCP(CP_UTF8);

  title();
  do{
    menu();
  }while(true);

  return 0;
}

void Game::createRoles(){
  Roles Villager(
    VILLAGER,
    "🚜",
    "Villager",
    "You are a regular villager without any special abilities.",
    Village,
    Good
  );
  Roles Seer(
    SEER,
    "🔮",
    "Seer",
    "Each night you can select a player to uncover their role.",
    Village,
    Good,true
  );
  Roles ASeer(
    AURA_SEER,
    "👁️ ",
    "Aura Seer",
    "Each night you can select a player to uncover their alignment: good, evil, or unknown. Evil players belong to the werewolves and good players belong to the villagers team.",
    Village,
    Good,true
  );
  Roles Detective(
    DETECTIVE,
    "🕵️ ",
    "Detective",
    "Each night you can select two players to uncover if they are in the same team. Possible teams are: village, werewolves, fool, headhunter, serial killer, etc.",
    Village,
    Good,true
  );
  Roles Doctor(
    DOCTOR,
    "🩺",
    "Doctor",
    "Choose one player to protect every night. That player cannot be killed that night.",
    Village,
    Good,true
  );
  Roles Priest(
    PRIEST,
    "⛪",
    "Priest",
    "You can throw holy water on another player. If that player is a werewolf, they die. If they are not a werewolf, you die.",
    Village,
    Good,false,true
  );
  Roles Medium(
    MEDIUM,
    "🕯️ ",
    "Medium",
    "During the night you can talk anonymously with the dead. Once during the game you can select a dead villager at night to revive at the end of the night.",
    Village,
    UnknownA,true
  );
  Roles Avenger(
    AVENGER,
    "⚔️ ",
    "Avenger",
    "After the first night you can select one player to kill when you die.",
    Village,
    Good,true
  );
  Roles Cupid(
    CUPID,
    "❤️ ",
    "Cupid",
    "On the first night, the Cupid selects 2 players to become a Love Couple. These players are now loyal to each other and not their original team (although the players can still win with their original team). If one of these players dies, the other will die as well.",
    Village,
    Good
  );
  Roles BeastHunter(
    BEAST_HUNTER,
    "🏹",
    "Beast Hunter",
    "At night you can place a trap on a player which will become active the following night. This player cannot be killed at night. If the player is attacked by werewolves, the weakest werewolf will die. The attack of solo killers, zombies and the sect leader will trigger the trap but not kill them.",
    Village,
    UnknownA,true
  );
  Roles Cursed(
    CURSED,
    "☠️ ",
    "Cursed",
    "You are a villager until the werewolves try to kill you, at which point you become a werewolf.",
    Village,
    Good
  );
  Roles Werewolf(
    WEREWOLF,
    "🐺",
    "Werewolf",
    "Each night you can vote with the wolves on a player to kill.",
    Werewolves,
    Evil
  );
  Roles WSeer(
    WOLF_SEER,
    "🌕",
    "Wolf Seer",
    "Each night you can select a player to uncover their role. If you are the last werewolf or you resign your ability, you become a regular werewolf.",
    Werewolves,
    Evil,true
  );
  Roles JrWolf(
    JUNIOR_WOLF,
    "🩸",
    "Junior Wolf",
    "You are a junior werewolf. Because you are so cute, you can select another player to be killed when you are killed",
    Werewolves,
    Evil,true,true
  );
  Roles NightmareWolf(
    NIGHTMARE_WOLF,
    "🌑",
    "Nightmare Werewolf",
    "Twice during the game you can select a player during the day to 'fall asleep' for one night. That player won't be able to use any abilities for one night.",
    Werewolves,
    Evil,false,true
  );
  Roles GuardianWolf(
    GUARDIAN_WOLF,
    "🛡️ ",
    "Guardian Wolf",
    "During the voting phase, the Guardian Wolf can protect a player from being lynched by the village. Once the Guardian Wolf successfully protects a player, its ability will be consumed.",
    Werewolves,
    Evil
  );
  Roles Headhunter(
    HEADHUNTER,
    "🎯",
    "Headhunter",
    "At the start of the game you are assigned a villager as a target. Your goal is to get your target lynched by the village during the day. Your target must be lynched before you die in order to win. If your target dies another way, you will remain a headhunter but win with the evil alliance (werewolves and solo killers) instead.",
    Solo,
    UnknownA
  );
  Roles Fool(
    FOOL,
    "🎭",
    "Fool",
    "Your goal is to get lynched by the village. You win if they lynch you.",
    Solo,
    UnknownA
  );
  Roles Arsonist(
    ARSONIST,
    "🔥",
    "Arsonist",
    "Every night you can either select two players to douse with gasoline, or ignite all doused players which kills them. Dousing bypasses all protections.",
    Killer,
    UnknownA,true
  );
  Roles SerialKiller(
    SERIAL_KILLER,
    "🔪",
    "Serial Killer",
    "Each night, the Serial Killer select a player to kill them.",
    Killer,
    UnknownA,true
  );

  int i=0,x=0;
  allRoles[i++]=Villager;

  sandboxVillageRoles[x++]=Seer;
  allRoles[i++]=Seer;

  sandboxVillageRoles[x++]=ASeer;
  allRoles[i++]=ASeer;

  sandboxVillageRoles[x++]=Detective;
  allRoles[i++]=Detective;

  sandboxVillageRoles[x++]=Doctor;
  allRoles[i++]=Doctor;

  sandboxVillageRoles[x++]=Priest;
  allRoles[i++]=Priest;

  sandboxVillageRoles[x++]=Medium;
  allRoles[i++]=Medium;

  sandboxVillageRoles[x++]=Avenger;
  allRoles[i++]=Avenger;

  sandboxVillageRoles[x++]=Cupid;
  allRoles[i++]=Cupid;

  sandboxVillageRoles[x++]=BeastHunter;
  allRoles[i++]=BeastHunter;

  sandboxVillageRoles[x++]=Cursed;
  allRoles[i++]=Cursed;

  x=0;
  allRoles[i++]=Werewolf;

  sandboxWerewolfRoles[x++]=WSeer;
  allRoles[i++]=WSeer;

  sandboxWerewolfRoles[x++]=JrWolf;
  allRoles[i++]=JrWolf;

  sandboxWerewolfRoles[x++]=NightmareWolf;
  allRoles[i++]=NightmareWolf;

  sandboxWerewolfRoles[x++]=GuardianWolf;
  allRoles[i++]=GuardianWolf;

  x=0;
  soloRoles[x++]=Headhunter;
  allRoles[i++]=Headhunter;

  soloRoles[x++]=Fool;
  allRoles[i++]=Fool;

  x=0;
  killerRoles[x++]=Arsonist;
  allRoles[i++]=Arsonist;

  killerRoles[x++]=SerialKiller;
  allRoles[i++]=SerialKiller;
}

void Game::createPlayers(){
  int i=1;
  Player user("dash",i++);
  Player player2("Kim Dokja",i++);
  Player player3("Yoo Joonghyuk",i++);
  Player player4("Shreejha",i++);
  Player player5("Epstein",i++);
  Player player6("Femboy",i++);
  Player player7("Evil Foid",i++);
  Player player8("Larper",i++);
  Player player9("Tung Tung Tung Sahur",i++);
  Player player10("Aura Monster Subaru",i++);
  Player player11("The Amazing Adrian",i++);
  Player player12("Mommy Mila",i++);
  Player player13("Big Yahu",i++);
  Player player14("Daddy Jason",i++);
  Player player15("Alysa Liu",i++);
  Player player16("Demon King of Salvation",i);

  i=0;
  players[i++]=user;
  players[i++]=player2;
  players[i++]=player3;
  players[i++]=player4;
  players[i++]=player5;
  players[i++]=player6;
  players[i++]=player7;
  players[i++]=player8;
  players[i++]=player9;
  players[i++]=player10;
  players[i++]=player11;
  players[i++]=player12;
  players[i++]=player13;
  players[i++]=player14;
  players[i++]=player15;
  players[i]=player16;
}

void Game::showPlayers(int x){
  cout<<"Players:"<<endl;
  for(int i=0;i<16;i++){
    Sleep(4*SleepCD);
    cout<<"[ "<<players[i].id<<" ] ";
    if(x==1 || i==0 ||
      (players[0].role.team == Werewolves && players[i].role.team == Werewolves) ||
      !players[i].isAlive ||
      players[i].roleRevealed){
      cout<<players[i].role.icon<<" "<<players[i].role.name<<"  ";
    }
    else{
      cout<<"? ";
    }
    if(!players[i].isAlive){
      cout<<players[i].name<<" 🪦";
    }
    else if(x==2){
      cout<<players[i].name<<"  "<<players[i].votes<<" votes!";
    }
    else{
      cout<<players[i].name<<"  ";
    }
    if(players[i].isCoupled && (players[0].isCoupled || players[0].role.id == CUPID)){
      cout<<"  💕";
    }
    cout<<endl;
  }
  cout<<"\n";
}

void Game::assignRoles(){
  int random;
  // players[0].role = killerRoles[1];
  // players[0].roleAssigned=true;
  for(int i=0;i<16;i++){
    do{
      random = rand()%16;
    }while(players[random].roleAssigned);
    players[random].roleAssigned=true;

    if(i<10){
      players[random].role = sandboxVillageRoles[i];
    }
    else if(i<14){
      players[random].role = sandboxWerewolfRoles[i-10];
    }
    else if(i==14){
      players[random].role = soloRoles[rand()%2];
    }
    else if(i==15){
      players[random].role = killerRoles[rand()%2];
    }
  }
}

void Game::beginning(){
  Sleep(7.5*SleepCD);
  cout<<"Your role is...'"<<players[0].role.name<<"'\n";
  cout<<players[0].role.description<<endl;
  Sleep(7.5*SleepCD);
  
  for(int i=0;i<16;i++){
    if(players[i].role.id == HEADHUNTER)
      hh=i;
    else if(players[i].role.id == CUPID)
      cupid=i;
    else if(players[i].role.id == GUARDIAN_WOLF)
      guardian=i;
  }

  do{
    hhTarget = rand()%16;
  }while(players[hhTarget].role.team!=Village);
  
  do{
    couple1=rand()%16;
    couple2=rand()%16;
  }while(couple1 == couple2 || couple1==cupid || couple2==cupid);
  if(players[0].role.id != CUPID){
    players[couple1].isCoupled=true;
    players[couple2].isCoupled=true;
  }
  
  // cout<<"\nCoupled players "<<players[couple1].name<<" and "<<players[couple2].name<<endl;
  
  Sleep(4*SleepCD);
  switch(players[0].role.id){
    case HEADHUNTER:
    cout<<"\nYour target is player '"<<players[hhTarget].name<<"'\n";
    break;

    case CUPID:
    cout<<"\nWhom do you want to couple:\n";
    cin>>couple1>>couple2;
    couple1--;
    couple2--;
    Sleep(5*SleepCD);
    cout<<"\nYou have coupled player "<<players[couple1].name<<" and "<<players[couple2].name<<endl;
    players[couple1].isCoupled=true;
    players[couple2].isCoupled=true;
    break;
  }
}

void Game::nightRoleActivity(){
  int choice;
  cout<<"\nNight "<<night<<"\n\n";
  Sleep(7.5*SleepCD);
  night++;

  for(int i=0;i<16;i++){
  Sleep(7.5*SleepCD);
    if(players[i].isAlive && !players[i].isNightmared){

    switch(players[i].role.id){

      case SEER:
      if(i==0){
        cout<<"Which player do you want to check: ";
        cin>>choice;
        Sleep(5*SleepCD);
        cout<<"The seer checks player '"<<players[choice-1].name<<"'.\n";
        Sleep(7.51*SleepCD);
        cout<<"Their role is...'"<<players[choice-1].role.name<<"'\n";
        players[choice-1].roleRevealed=true;
        }
      else{
        do{
          seerTarget=rand()%16;
        }while(seerTarget==i || !players[seerTarget].isAlive);
      }
      break;

      case AURA_SEER:
      if(i==0){
        cout<<"Which player do you want to check: ";
        cin>>choice;
        Sleep(4*SleepCD);
        cout<<"The aura seer checks player '"<<players[choice-1].name<<"'.\n";
        Sleep(4*SleepCD);
        cout<<"Their aura is...";
        Sleep(5*SleepCD);
        switch(players[choice-1].role.aura){
          case Evil:
          cout<<"Evil\n";
          break;
          case Good:
          cout<<"Good\n";
          break;
          case UnknownA:
          cout<<"Unknown\n";
          break;
        }
      }
      else{
        do{
          choice=rand()%16;
        }while(choice==i || !players[choice].isAlive);
        auraSeerTarget=choice;
        switch(players[choice].role.aura){
          case Evil:
          auraSeerString="Evil";
          break;
          case Good:
          auraSeerString="Good";
          break;
          case UnknownA:
          auraSeerString="Unknown";
          break;
        }
      }
      break;

      case DOCTOR:
      if(i==0){
        cout<<"Which player do you want to protect: ";
        cin>>choice;
        docTarget=choice-1;
        players[docTarget].isProtected = true;
        Sleep(4*SleepCD);
        cout<<"You protect player '"<<players[docTarget].name<<"' tonight.\n";
        }
      else{
        docTarget=rand()%16;
        players[docTarget].isProtected=true;
      }
      break;

      case MEDIUM:
      if(i==0){
        if(alivePlayers(Village)<10 && !medHasRevived){
          cout<<"\nDo you want to revive any place? 1. yes 0. no\n";
          cin>>choice;
          if(choice){
            Sleep(100);
            cout<<"Which player do you want to revive: ";
            cin>>choice;
            revivePlayer=choice-1;
            medHasRevived=1;
          }
        }
        else if(totalAlive()==16){
          cout<<"Nobodys dead yet!\n";
        }
      }
      else{
        if(alivePlayers(Village)<10 && !medHasRevived && rand()%2){
          do{
            choice=rand()%16;
          }while(choice==i || players[choice].role.team!= Village || players[choice].isAlive);
          revivePlayer=choice;
          medHasRevived=1;
        }
      }
      break;

      case DETECTIVE:
      int choice2;
      if(i==0){
        cout<<"Which two players do you want to check\n";
        cin>>choice>>choice2;
        Sleep(7.5*SleepCD);
        if(players[choice-1].role.team == players[choice2-1].role.team){
          cout<<players[choice-1].name<<" and "<<players[choice2-1].name<<" have the same team!\n";
        }
        else{
          cout<<players[choice-1].name<<" and "<<players[choice2-1].name<<" have a different team!\n";
        }
      }
      else{
        do{
          choice=rand()%16;
          choice2=rand()%16;
          if(choice!=choice2 && players[choice].isAlive && players[choice2].isAlive){
            break;
          }
        }while(true);
        det1Target=choice;
        det2Target=choice2;
      }
      break;

      case AVENGER:
      if(i==0){
        cout<<"Which player do you want to select to avenge: ";
        cin>>choice;
        avengerTarget=choice-1;
        Sleep(4*SleepCD);
        cout<<"Youve decided to avenge the player '"<<players[avengerTarget].name<<"'.\n";
      }
      else{
        do{
          avengerTarget=rand()%16;
        }while(avengerTarget == i || (players[i].isCoupled && players[avengerTarget].isCoupled));
      }
      break;

      case BEAST_HUNTER:
      if(i==0){
        if(beastHunterTrap){
          cout<<"Your trap has been set on '"<<players[beastHunterTarget].name;
          Sleep(4*SleepCD);
          cout<<"'.\nDo you want to let it stay(1) or set your trap on another player(0)\n";
          cin>>choice;
          beastHunterTrap = choice;
          players[beastHunterTarget].isProtected = choice ? true:false;
        }
        if(!beastHunterTrap){
          cout<<"On whom do you want to set your trap: ";
          cin>>choice;
          beastHunterTrap++;
          beastHunterTarget=choice-1;
          Sleep(4*SleepCD);
          cout<<"Your trap will be set on player '"<<players[beastHunterTarget].name<<"' on the next night!\n";
        }
      }
      else{
        if(!beastHunterTrap){
          choice=rand()%16;
        beastHunterTarget=choice;
        beastHunterTrap=1;
        }
      }
      break;

      case WOLF_SEER:
      if(i==0){
        cout<<"Which player do you want to check: ";
        Sleep(4*SleepCD);
        cin>>choice;
        cout<<"The wolf seer checks player '"<<players[choice-1].name<<"'.\n";
        Sleep(4*SleepCD);
        cout<<"Their role is...'"<<players[choice-1].role.name<<"'\n";
        players[choice-1].roleRevealed=true;
      }
      else{
        if(players[0].role.team == Werewolves){
          cout<<"\nWolf Chat:\n";
          Sleep(4*SleepCD);
          do{
            choice=rand()%16;
          }while(choice==i || players[choice].role.team==Werewolves || !players[choice].isAlive);
          cout<<"The wolf seer checks player '"<<players[choice].name<<"'.\n";
          Sleep(5*SleepCD);
          cout<<"Their role is...'"<<players[choice].role.name<<"'\n";
          players[choice].roleRevealed=true;
        }
      }
      break;
      
      case JUNIOR_WOLF:
      if(i==0){
        cout<<"Which player do you want to take down with you: ";
        Sleep(4*SleepCD);
        cin>>choice;
        jrWolfTarget=choice-1;
        cout<<"You've decided to take down the player '"<<players[jrWolfTarget].name<<"' if you die.\n";
      }
      else{
        do{
          jrWolfTarget=rand()%16;
        }while(jrWolfTarget == i || players[jrWolfTarget].role.team == Werewolves || (players[i].isCoupled && players[jrWolfTarget].isCoupled));
      }
      break;

      case ARSONIST:
      if(i==0){
        cout<<"Current doused players are: \n";
        for(int i=0;i<16;i++){
          if(players[i].isDoused && players[i].isAlive){
            Sleep(5*SleepCD);
            cout<<"[ "<<players[i].id<<" ] ";
            cout<<players[i].name<<" ⛽\n";
          }
        }
        Sleep(7.5*SleepCD);
        cout<<"\nDo you want to burn all doused players(0) or douse two more players(1)\n";
        cin>>choice;
        if(choice){
          int choice2;
          Sleep(4*SleepCD);
          cout<<"Which two players do you want to douse\n";
          cin>>choice>>choice2;
          Sleep(4*SleepCD);
          cout<<"Players '"<<players[choice-1].name<<"' and '"<<players[choice2-1].name<<"' have been doused!\n";
          players[choice-1].isDoused=true;
          players[choice2-1].isDoused=true;
        }
        else{
          for(int i=0;i<16;i++){
            if(players[i].isDoused){
              players[i].killedBy = Arson;
            }
          }
        }
      }
      else{
        int target = 0;
        for(int j=0;j<16;j++){
          if(j!=i && players[j].isAlive && !players[j].isDoused && !(players[i].isCoupled && players[j].isCoupled)){
            target++;
          }
        }

        if((target>2 && rand()%2) || target==2){
          for(int x=0;x<2;x++){
            do{
              choice=rand()%16;
            }while((players[choice].isDoused || choice==i || !players[choice].isAlive) || (players[i].isCoupled && players[choice].isCoupled));
            players[choice].isDoused=true;
          }
        }
        else if(target==1){
          for(int j=0;j<16;j++){
            if(j!=i && !players[j].isDoused && players[j].isAlive && !(players[i].isCoupled && players[j].isCoupled)){
              players[j].isDoused = true;
              break;
            }
          }
        }
        else{
          for(int i=0;i<16;i++){
            if(players[i].isDoused){
              players[i].killedBy = Arson;
            }
          }
        }
      }
      break;

      case SERIAL_KILLER:
      if(i==0){
        cout<<"Which player do you want to kill: ";
        cin>>choice;
        Sleep(5*SleepCD);
        cout<<"You try to kill player '"<<players[choice-1].name<<"'.\n";
        players[choice-1].killedBy = Serial;
        Sleep(7.51*SleepCD);
        }
      else{
        do{
          choice=rand()%16;
        }while(choice==i || !players[choice].isAlive || (players[i].isCoupled && players[choice].isCoupled));
        players[choice].killedBy = Serial;
      }
      break;

      default:
      if((players[0].role.team == Village || players[0].role.id == FOOL || players[0].role.id == HEADHUNTER)&&i==0){
        // cout<<"You have nothing to do, go to sleep!\n";
      }
      break;
    }
  }
    else if(players[0].isAlive && players[0].isNightmared && !i){
      // cout<<"You have been nightmared! You are currently in a deep sleep.\n\n";
    }
  }
}

void Game::dayRoleActivity(){
  int choice;
  Sleep(7.5*SleepCD);

  for(int i=0;i<16;i++){
    if(players[i].isAlive){
      switch(players[i].role.id){
        case PRIEST:
        int x;
        if(i==0){
          cout<<"\nDo you want to throw your holy water on anybody?(1. yes 0. no)\n";
          cin>>x;
          Sleep(5*SleepCD);
          if(x){
          cout<<"Which player do you want to throw holy water: ";
          cin>>choice;
          priestTarget=choice-1;
          }
        }
        else{
          x = rand()%100;
          if(x<20){
            do{
              priestTarget=rand()%16;
            }while(priestTarget==i || !players[priestTarget].isAlive || (players[i].isCoupled && players[priestTarget].isCoupled));
          }
        }
        
        Sleep(5*SleepCD);
        if(x<20 && x){
          cout<<endl;
          if(players[priestTarget].role.team == Werewolves){
            cout<<players[i].name<<" (Priest) has thrown holy water at and killed "<<players[priestTarget].name<<".";
            players[i].roleRevealed=true;
            players[priestTarget].kill();
            extraKills(priestTarget);
          }
          else{
          cout<<players[i].name<<" (Priest) has thrown holy water at "<<players[priestTarget].name<<" and killed themself. "<<players[priestTarget].name<<" is not a werewolf!";
          players[i].kill();
          extraKills(i);
          }
          gameRunning=winCheck(-1);
        }
        break;

        case JUNIOR_WOLF:
        if(i==0){
          cout<<"\nWhich player do you want to kill with you: ";
          cin>>choice;
          jrWolfTarget=choice-1;
          Sleep(4*SleepCD);
          cout<<"You've decided to kill the player '"<<players[jrWolfTarget].name<<"' if you die.\n";
        }
        else{
          do{
            jrWolfTarget=rand()%16;
          }while(players[jrWolfTarget].role.team==Werewolves || !players[jrWolfTarget].isAlive || (players[i].isCoupled && players[jrWolfTarget].isCoupled));
        }
        break;

        case AVENGER:
        if(i==0){
          cout<<"\nWhich player do you want to select to avenge: ";
          cin>>choice;
          avengerTarget=choice-1;
          Sleep(4*SleepCD);
          cout<<"Youve decided to avenge the player '"<<players[avengerTarget].name<<"'.\n";
        }
        else{
          do{
            avengerTarget=rand()%16;
          }while(avengerTarget==i || !players[avengerTarget].isAlive || (players[i].isCoupled && players[avengerTarget].isCoupled));
        }
        break;

        case NIGHTMARE_WOLF:
        if(nightmares>0){
          if(i==0){
            cout<<"\nWho do you want to nightmare for the next night: ";
            cin>>choice;
            players[choice-1].isNightmared=true;
            Sleep(4*SleepCD);
            cout<<"You decided to put player '"<<players[choice-1].name<<"' in a nightmare!\n";
            nightmares--;
          }
          else{
            choice=rand()%100;
            if(choice<50){
              do{
                choice=rand()%16;
              }while(players[choice].role.team==Werewolves || !players[choice].isAlive || (players[i].isCoupled && players[choice].isCoupled));
              players[choice].isNightmared=true;
              nightmares--;
            }
          }
        }
        break;
      }
      
    }
  }
  
  cout<<endl;
}

void Player::kill(){
  cout<<endl;
  Sleep(1500);
  switch(killedBy){
    case Arson:
    cout<<"🔥 The Arsonist set '"<<name<<"' on fire!\n";
    break;

    case Serial:
    cout<<"🔪 The Serial Killer stabbed '"<<name<<"'!\n";
    break;

    case Wolves:
    cout<<"🐺 The werewolves killed '"<<name<<"'!\n";
    break;

    case Lynch:
    cout<<"⛏️  The village hanged '"<<name<<"'!\n";
    break;

    case BeastHunter:
    cout<<"🪚  The beast hunters trap killed '"<<name<<"'!\n";
    break;

    case Avenger:
    cout<<"⚔️  The avenger has avenged their death and killed '"<<name<<"'!\n";
    break;

    case JrWolf:
    cout<<"🐾 The junior werewolf's death has been avenged, '"<<name<<"' is dead!\n";
    break;

    case Cupid:
    cout<<"💔 "<<name<<" was heartbroken and died!\n";
    break;

  }
  cout<<"Their role was '"<<role.name<<"'!\n";

  isAlive=false;
  deathProc=true;

  if(!id){
    Sleep(5*SleepCD);
    cout<<"\nYou Died!\n";
    Sleep(5*SleepCD);
  }
}

void Player::revive(){
  Sleep(7.5*SleepCD);
  cout<<"\n🕯️  The Medium revived player '"<<name<<"'.\n";
  isAlive=true;
  killedBy=-1;
  deathProc=false;
  roleRevealed=true;
}

void Game::killPhase(){
  for(int i=0;i<16;i++){
    if(players[i].killedBy == Wolves && !players[i].deathProc){
      if(players[i].role.id == ARSONIST || players[i].role.id == SERIAL_KILLER){
        if(players[0].role.team == Werewolves){
          Sleep(4*SleepCD);
          cout<<"\nPlayer '"<<players[i].name<<"' could not be killed!\n";
        }
      }
      else if(players[i].role.id==CURSED){
        if(players[0].role.id==CURSED){
          Sleep(4*SleepCD);
          cout<<"\nYou have been bitten! You are a werewolf now!\n";
        }
        else if(players[0].role.team == Werewolves){
          Sleep(4*SleepCD);
          cout<<"\n"<<players[i].name<<" was Cursed. They are a werewolf now!\n";
        }
        players[i].role = allRoles[WEREWOLF];
      }
      else if(players[i].isProtected){
        if(beastHunterTarget==i && beastHunterTrap){
          Sleep(4*SleepCD);
          if(players[0].role.id == BEAST_HUNTER)
            cout<<"\nThe beast hunters trap was triggered!\n";
          beastHunterTrap=0;
          players[beastHunterTarget].isProtected=false;
          do{
            int x=rand()%16;
            if(players[x].role.team == Werewolves && players[x].isAlive){
              players[x].killedBy = BeastHunter;
              players[x].kill();
              extraKills(x);
              break;
            }
          }while(alivePlayers(Werewolves));
        }
        if(docTarget==i){
          Sleep(4*SleepCD);
          cout<<"\n🩹 The doctor saved player '"<<players[i].name<<"'!\n";
          if(players[0].role.id==DOCTOR){
          cout<<"\nYour protection saved player '"<<players[i].name<<"' last night!\n";
          }
        }
      }
    else{
      players[i].kill();
      extraKills(i);
      }
    }

    else if((players[i].killedBy == Arson || players[i].killedBy == Serial) && !players[i].deathProc){
      if(players[i].isProtected){
        if(beastHunterTarget==i && beastHunterTrap){
          Sleep(4*SleepCD);
          if(players[0].role.id == BEAST_HUNTER)
            cout<<"\nThe beast hunters trap was triggered but the enemy was too strong to be killed!\n";
          beastHunterTrap=0;
        }
        if(docTarget==i){
          if(players[i].killedBy == Arson){
              players[i].kill();
              extraKills(i);
          }
          else{
              Sleep(4*SleepCD);
              cout<<"\n🩹 The doctor saves player '"<<players[i].name<<"'!\n";
              if(players[0].role.id==DOCTOR){
              cout<<"\nYour protection saved player '"<<players[i].name<<"' last night!\n";
              }
            }
          }
      }
    else{
      players[i].kill();
      extraKills(i);
      }
    }
  }
  gameRunning=winCheck(-1);
}

void Game::wolvesActivity(){
  if(!alivePlayers(Werewolves)){
      return;
  }
  
  int choice;
  if(players[0].role.team==Werewolves && players[0].isAlive){
    Sleep(4*SleepCD);
    cout<<"\nWhom does the werewolves want to kill: ";
    cin>>choice;
    players[choice-1].killedBy=Wolves;
  }
  else{
    do{
      choice = rand()%16;
    }while(!players[choice].isAlive || players[choice].role.team==Werewolves);
    players[choice].killedBy=Wolves;
  }
}

void Game::effectsRemoval(){
  for(int i=0;i<16;i++){
    players[i].votes=0;
    players[i].voted=0;
    players[i].killedBy=-1;
    players[i].isNightmared=false;
    if(players[i].isProtected){
      if(beastHunterTarget==i)
        continue;
      players[i].isProtected=false;
    }
  }
  if(medHasRevived==1){
    players[revivePlayer].revive();
    medHasRevived=-1;
  }
}

void Game::discussion(){
  gameRunning=winCheck(-1);
  cout<<"\nDay "<<day<<" has started. Get ready to discuss!\n\n";
  Sleep(7.5*SleepCD);

  if(day==1){
    int random;
    if(!players[couple1].isAlive){
      do{
        random = rand()%16;
      }while(!players[random].isAlive || random == couple1 || random == couple2 || random == cupid);
      if(!cupid){
        cout<<"Since "<<players[couple1].name<<" died, the couple has been redirected to player "<<players[random].name<<"\n\n";
      }
      players[couple1].isCoupled = false;
      couple1=random;
      players[couple1].isCoupled = true;
    }
    if(!players[couple2].isAlive){
      do{
        random = rand()%16;
      }while(!players[random].isAlive || random == couple1 || random == couple2 || random == cupid);
      if(!cupid){
        cout<<"Since "<<players[couple2].name<<" died, the couple has been redirected to player "<<players[random].name<<"\n\n";
      }
      players[couple2].isCoupled = false;
      couple2=random;
      players[couple2].isCoupled = true;
    }
    if(players[0].isCoupled){
      cout<<"You have been coupled!\n";
      cout<<players[couple1].name<<" and "<<players[couple2].name<<" are couples!\n\n";
      players[couple1].roleRevealed = true;
      players[couple2].roleRevealed = true;
    }
  }

  day++;

  string userDiscuss;
  if(players[0].isAlive){
    cout<<"What do you want to discuss?\n";
    getline(cin >> ws, userDiscuss);
  }

  for(int i=0;i<16;i++){
    if(!players[i].isAlive)
      continue;
    
    Sleep(1250);
    cout<<"\n[ "<<i+1<<" ] "<<players[i].name<<" says:\n";
    if(i==0){
      cout<<userDiscuss<<endl;
    }
    else{
      switch(players[i].role.id){ 

        case SEER:
        if(players[seerTarget].role.team == Village){
          cout<<players[seerTarget].name<<" has a village role\n";
        }
        else{
          cout<<players[seerTarget].name<<"'s role is "<<players[seerTarget].role.name<<endl;
        }
        break;

        case AURA_SEER:
        cout<<players[auraSeerTarget].name<<" has "<<auraSeerString<<" aura\n";
        break;

        case DETECTIVE:
        if(players[det1Target].role.team == players[det2Target].role.team){
          cout<<players[det1Target].name<<" and "<<players[det2Target].name<<" have the same team!\n";
        }
        else{
          cout<<players[det1Target].name<<" and "<<players[det2Target].name<<" have a different team!\n";
        }
        break;

        default:
        const string lines[]={
          "Who should we vote?", "Goodmorning Villagers!", "Does anyone have any info?", "Im good trust me lol", "I have a bad feeling about this", "Might as well vote random lol", "I have evidence", "Guys let's calm down.","Skipping is smarter here.","I think the seer is fake.","The moon spoke to me.","I am definitely evil."
        };//12 lines
        cout<<lines[rand()%12]<<endl;
        break;
      }
    }
  }
}

void Game::voting(){
  int votesReq= (totalAlive()/2),highest=0,tied=-1;
  cout<<"\nGet ready to vote! "<<votesReq<<" votes required!\n";
  Sleep(7.5*SleepCD);

  showPlayers();

  if(players[0].isAlive){
    cout<<"Which player do you want to vote: ";
    cin>>players[0].voted;
    players[0].voted--;
  }

  for(int i=1;i<16;i++){
    int random = rand()%100;
    if(!players[0].isAlive){
      random=100;
    }
    if(random<25){
      players[i].voted = players[0].voted;
    }
    else{
      if(players[i].role.team == Werewolves){
        do{
          players[i].voted = rand()%16;
        }while(players[players[i].voted].role.team == Werewolves || players[i].voted == i || !players[players[i].voted].isAlive|| (players[i].isCoupled && players[players[i].voted].isCoupled));
      
        if(players[0].role.team==Werewolves && players[0].isAlive){
          players[i].voted=players[0].voted;
        }
      }
      else{
        do{
          players[i].voted = rand()%16;
        }while(players[i].voted == i || !players[players[i].voted].isAlive || (players[i].isCoupled && players[players[i].voted].isCoupled));
      }
    }
  }
  for(int i=0;i<16;i++){
      if(players[i].isAlive){
      players[players[i].voted].votes++;
      Sleep(5*SleepCD);
      cout<<"\n"<<players[i].name<<" voted for "<<players[players[i].voted].name<<".\n";
    }
  }
  for(int i=1;i<16;i++){
    if(players[i].votes>players[highest].votes && players[i].isAlive){
      highest = i;
    }
  }
  for(int i=0;i<16;i++){
      if(players[i].votes==players[highest].votes){
          tied++;
      }
  }
  cout<<endl;
  showPlayers(2);

  if(tied && players[highest].votes>=votesReq){
    cout<<"\nIts a tie!\n";
  }
  else if(players[highest].votes>=votesReq){
    int choice=0;
    if(players[guardian].isAlive && !guardianProtected && (players[highest].role.team == Werewolves || (players[guardian].isCoupled && players[highest].isCoupled))){
      choice=1;
      if(!guardian){
        cout<<"Do you want the protect "<<players[highest].name<<" from being lynched? yes(1) no(0)\n";
        cin>>choice;
      }
    }
    if(choice){
      Sleep(1000);
      cout<<"\nThe Guardian Wolf has protected "<<players[highest].name<<" from being lynched!\n";
      guardianProtected = true;
    }
    else{
      players[highest].killedBy = Lynch;
        players[highest].kill();
        extraKills(highest);
        gameRunning=winCheck(highest);
    }
  }
  else{
    cout<<"The village could not decide whom to lynch.\n";
  }

}

void Game::extraKills(int id){
  switch(players[id].role.id){
    case JUNIOR_WOLF:
    if(players[jrWolfTarget].isAlive){
      players[jrWolfTarget].killedBy= JrWolf;
      players[jrWolfTarget].kill();

      if((players[jrWolfTarget].role.id == AVENGER && players[avengerTarget].isAlive )|| players[jrWolfTarget].isCoupled){
        extraExtraKills(jrWolfTarget);
      }
    }
    break;

    case AVENGER:
    if(players[avengerTarget].isAlive){
      players[avengerTarget].killedBy= Avenger;
      players[avengerTarget].kill();

      if((players[avengerTarget].role.id == JUNIOR_WOLF && players[jrWolfTarget].isAlive)|| players[avengerTarget].isCoupled){
        extraExtraKills(avengerTarget);
      }
    }
    break;
    
  }

  if(players[id].isCoupled && day!=1){
    int x = (id==couple1) ? couple2:couple1;
    players[x].killedBy = Cupid;
    cout<<"\n"<<players[couple1].name<<" and "<<players[couple2].name<<" were couples!\n";
    players[x].kill();
    extraExtraKills(x,0);
  }
}

void Game::extraExtraKills(int id, int flag){
  switch(players[id].role.id){
    case JUNIOR_WOLF:
    if(players[jrWolfTarget].isAlive){
      players[jrWolfTarget].killedBy= JrWolf;
      players[jrWolfTarget].kill();
    }
    break;

    case AVENGER:
    if(players[avengerTarget].isAlive){
      players[avengerTarget].killedBy= Avenger;
      players[avengerTarget].kill();
    }
    break;

  }
  if(players[id].isCoupled && day!=1 && flag){
    int x = (id==couple1) ? couple2:couple1;
    players[x].killedBy = Cupid;
    cout<<"\n"<<players[couple1].name<<" and "<<players[couple2].name<<" were couples!\n";
    players[x].kill();
  }
}

int Game::alivePlayers(RoleType team){
  int count=0;
  for(int i=0;i<16;i++){
    if(players[i].isAlive && players[i].role.team == team)
      count++;
  }
  return count;
}

int Game::totalAlive(){
  int count=0;
  for(int i=0;i<16;i++){
    if(players[i].isAlive){
      count++;
    }
  }
  return count;
}

bool Game::winCheck(int id){
  int killers=0,player;

  for(int i=0;i<16;i++){
    if(players[i].role.team == Killer && players[i].isAlive){
      player=i;
      killers++;
    }
    if(id!=-1){
      if(players[id].role.id == FOOL && players[id].killedBy == Lynch){
        Sleep(1000);
        cout<<"\nThe Fool '"<<players[id].name<<"' Wins!\n";
        return false;
      }
      else if(id == hhTarget && players[hh].isAlive){
        Sleep(1000);
        cout<<"\n'"<<players[hhTarget].name<<"' was the headhunters target!";
        Sleep(7.5*SleepCD);
        cout<<"\nThe Headhunter '"<<players[hh].name<<"' Wins!\n";
        return false;
      }
    }
  }

  if((totalAlive()==3 && players[couple1].isAlive && players[cupid].isAlive) || 
  (totalAlive()==2 && players[couple1].isAlive)){
    Sleep(1000);
    cout<<"\nThe Couples "<<players[couple1].name<<" and "<<players[couple2].name<<" win!\n";
      return false;
  }
  else if(alivePlayers(Werewolves)>=alivePlayers(Village) && !killers){
    Sleep(1000);
    cout<<"\nThe Werewolves win!\n";
      return false;
  }
  else if(alivePlayers(Village) && !killers && !alivePlayers(Werewolves)){
    Sleep(1000);
    cout<<"\nThe Village wins!\n";
      return false;
  }
  else if(killers && !alivePlayers(Werewolves) && !alivePlayers(Village)){
    Sleep(1000);
    cout<<"\nThe "<<players[player].role.name<<" '"<<players[player].name<<"' wins!\n";
      return false;
  }
  else if(!totalAlive()){
    cout<<"\nIts a tie!\n";
  }
  return true;
}

void title(){
    cout << R"(  ⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  ⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⣤⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⢠⣤⡀⡀⡀⣤⡄⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  ⠸⣿⡄⡀⡀⡀⡀⣸⣿⡀⡀⡀⡀⡀⢸⣿⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⣿⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⣿⡇⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  ⡀⣿⣷⡀⡀⡀⡀⣿⢻⣷⡀⡀⡀⡀⣿⡏⡀⡀⡀⡀⡀⡀⡀⡀⣀⡀⡀⡀⡀⡀⡀⡀⣿⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  ⡀⠸⣿⡀⡀⡀⢸⣿⡀⣿⡀⡀⡀⢰⣿⡀ ⡀⢠⣿⣿⡄  ⣿⣿⡇ ⢀⣤⡄ ⣿⡀⡀⢿⣷⡀⡀⡀⡀⢰⣿⡀⡀⢀⣾⣿⠿⢿⣷⣄⡀⡀⡀⣾⡿⠿⢿⣷⡄⠈⣿⣆⡀⡀⡀⡀⣾⡿⡀⡀⣿⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⡀⣠⣿⡿⠿⣿⣶⡀⡀⡀
  ⡀⡀⣿⣷⡀⡀⣿⠇⡀⢻⣷⡀⡀⣿⡏⡀ ⡀⢸⣿⣿⠃ ⠈⠛⠁  ⢸⣿⡇ ⣿⡀⡀⠈⣿⡆⡀⡀⡀⣿⠇⡀⢠⣿⠋⡀⡀⡀⠈⣿⡄⡀⠸⣿⡀⡀⡀⠈⡀⡀⠹⣿⡀⡀⡀⢰⣿⡀⡀⡀⣿⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⣼⣿⠁⡀⡀⡀⠹⣿⡀⡀
  ⡀⡀⠘⣿⡀⢰⣿⡀⡀⠈⣿⡀⢠⣿⡀⡀⣶⣶⣴⠉  ⢠⣿⣿⣿⣤ ⣉⠈  ⣿⡀⡀⡀⠸⣿⡀⡀⣾⡟⡀⡀⢸⣿⠿⠿⠿⠿⠿⠿⠧⡀⡀⠙⠿⣶⣤⣀⡀⡀⡀⢿⣷⡀⡀⣿⠃⡀⡀⡀⣿⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⣿⡿⠿⠿⠿⠿⠿⠿⠄⡀
  ⡀⡀⡀⣿⣇⣿⠇⡀⡀⡀⢻⣷⣿⡏⡀⡀ ⠃⠟⠻⢀⣿⣿⣿⣿⣿⣿⣾ ⡀⡀⡀⣿⡀⡀⡀⡀⢿⣷⢠⣿⡀⡀⡀⢸⣿⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⠈⢿⣧⡀⡀⡀⣿⡆⣼⡟⡀⡀⡀⡀⣿⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⣿⣷⡀⡀⡀⡀⡀⡀⡀⡀
  ⡀⡀⡀⠘⣿⣿⡀⡀⡀⡀⠈⣿⣿⡀⡀⡀⡀⡀⡀ ⢸⣿⣿⣿⡿⠻⠛⠙ ⡀⡀⡀⣿⡀⡀⡀⡀⡀⣿⣿⠃⡀⡀⡀⡀⠻⣿⣦⣀⣀⣤⣿⠃⡀⢴⣷⣄⣀⣀⣿⡟⡀⡀⡀⠘⣿⣿⡀⡀⡀⡀⡀⣿⡀⡀⡀⢸⣿⡀⡀⡀⣿⡇⡀⡀⠈⢿⣷⣤⣀⣀⣴⡿⡀⡀
  ⡀⡀⡀⡀⠛⠃⡀⡀⡀⡀⡀⠛⠋⡀⡀⡀⡀⡀⡀⡀⠈⠙⠋⠁⡀⡀⡀⡀⡀⡀⡀⡀⠉⡀⡀⡀⡀⡀⠘⠛⡀⡀⡀⡀⡀⡀⡀⠙⠛⠛⠉⡀⡀⡀⡀⠈⠙⠛⠛⠉⡀⡀⡀⡀⡀⠛⠃⡀⡀⡀⡀⡀⠉⡀⡀⡀⠈⠉⡀⡀⡀⠉⠁⡀⡀⡀⡀⠈⠛⠛⠛⠁⡀⡀⡀
  ⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  ⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀⡀
  )" << endl;
}

void menu(){
  Game game;
  game.createRoles();
  int choice;
  cout<<"\nWelcome to Wolvesville!\n";
  cout<<"\n1. New Game";
  cout<<"\n2. Roles";
  cout<<"\n0. Exit\n";
  cout<<"\nWhat would you like to do: ";
  cin>>choice;

  switch(choice){
    case 1:
      game.newGame();
      break;

    case 2:
      cout<<endl;
      for(int i=0;i<20;i++){
        cout<<i+1<<". "<<game.allRoles[i].name<<" "<<game.allRoles[i].icon<<endl;
      }
      cout<<"Which roles description would you like to check: ";
      cin>>choice;
      cout<<"\nRole: "<<game.allRoles[choice-1].name<<" "<<game.allRoles[choice-1].icon<<"\n"<<game.allRoles[choice-1].description;
      break;

    case 0:
      cout<<"\nThanks for playing!";
      exit(0);
  }

    cout << R"(

  ⠀⠀⠀⠀⠀⣀⣠⣤⣤⣤⣤⣄⣀⠀⠀⠀⠀⠀
  ⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀
  ⠀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢿⣿⣷⡀⠀
  ⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⣴⢿⣿⣧⠀
  ⣿⣿⣿⣿⣿⡿⠛⣩⠍⠀⠀⠀⠐⠉⢠⣿⣿⡇
  ⣿⡿⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿
  ⢹⣿⣤⠄⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⡏
  ⠀⠻⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⠟⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⠟⠁⠀⠀

  )" << endl;
}

void Game::newGame(){
  createPlayers();
  assignRoles();
  showPlayers();
  beginning();

  do{
    nightRoleActivity();
    wolvesActivity();
    killPhase();
    effectsRemoval();
    if(!gameRunning)
      break;
    discussion();
    dayRoleActivity();
    if(!gameRunning)
      break;
    voting();
    if(!gameRunning)
      break;
  }while(true);
  cout<<endl;
  showPlayers(1);
}
