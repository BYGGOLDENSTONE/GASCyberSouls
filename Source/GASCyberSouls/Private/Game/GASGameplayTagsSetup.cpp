#include "Game/GASGameplayTagsSetup.h"

// Abilities
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability,                            "Ability");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack,                     "Ability.Attack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Block,                      "Ability.Block");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dodge,                      "Ability.Dodge");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Hack,                       "Ability.Hack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Slash,                      "Ability.Slash");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_QuickHack,                  "Ability.QuickHack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_QuickHack_InterruptProtocol,"Ability.QuickHack.InterruptProtocol");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_QuickHack_SystemFreeze,     "Ability.QuickHack.SystemFreeze");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_QuickHack_FirewallBarrier,  "Ability.QuickHack.FirewallBarrier");

// States
UE_DEFINE_GAMEPLAY_TAG(TAG_State,                              "State");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Stunned,                      "State.Stunned");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Frozen,                       "State.Frozen");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_PreventMovement,              "State.PreventMovement");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Blocking,                     "State.Blocking");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dodging,                      "State.Dodging");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Hacking,                      "State.Hacking");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_FirewallBarrier,              "State.FirewallBarrier");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_PreventHackProgress,          "State.PreventHackProgress");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_ImmuneToQuickHacks,           "State.ImmuneToQuickHacks");

// Cooldowns
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack_Cooldown,            "Ability.Attack.Cooldown");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Block_Cooldown,             "Ability.Block.Cooldown");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Dodge_Cooldown,             "Ability.Dodge.Cooldown");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Slash_Cooldown,             "Ability.Slash.Cooldown");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_QuickHack_Cooldown,         "Ability.QuickHack.Cooldown");
