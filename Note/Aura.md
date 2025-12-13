### GameplayAbilitySystem
![alt text](InitAbilityActorInfo.png)
### Attributes
- ![alt text](DerivedAtttibutes.png)
- Primary Attributes
  1. **Strength**
  2. **Intelligence**
  3. **Resilience**
  4. **Vigor**
- Secondary Attributes
  1. **Armor** = ( 2 + Resilience ) * 0.25 + 6
  2. **ArmorPenetration** = ( 1 + Resilience) * 0.15 + 3
  3. **BlockChance** = Armor * 0.25 + 4
  4. **CriticalHitChance** = ArmorPenetration * 0.25 + 2
  5. **CriticalHitResistance** = Armor * 0.25 + 10
  6. **CriticalHitDamage** = ArmorPenetration * 1.5 + 10
  7. **HealthGeneration** = Vigor * 0.1 + 1
  8. **ManaGeneration** = Intelligence * 0.1 + 1
  9. **MaxHealth** = Vigor * 2.5 + 80 + Level * 10
  10. **MaxMana** = Intelligence * 2.5 + 50 + Level * 15

### GameplayAbilities
- Initialize GA
- ![alt text](InstancingPolicy.PNG)
- ![alt text](NetExecutionPolicy.PNG)
- ![alt text](GameplayAbilityThingsNotToUse.PNG)
- ![alt text](ClientNavigation.PNG)
###  于DefaultGame.ini中添加
[/Script/GameplayAbilities.AbilitySystemGlobals]
+AbilitySystemGlobalsClassName="/Script/Aura.AuraAbilitySystemGlobals"
+GameplayCueNotifyPaths=/Game/Blueprint/AbilitySystem/GameplayCueNotifies
###  于DefaultEngine.ini中添加
[/Script/Engine.RenderSettings]
r.TextureStreaming=True
r.Streaming.PoolSize=1000

[ConsoleVariables]
net.MaxRPCPerNetUpdate=10

在ASC中获取GetActivatableAbilities()时，
应该添加锁FScopedAbilityListLock ActiveScopeLock(*this);