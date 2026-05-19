# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防遊戲。

## 遊戲說明

- 左側有一座城堡，需要保護它
- 中間是玩家角色，可以移動和射擊
- 右邊會依照波次生成敵人，往城堡方向前進
- 擊敗敵人獲得金幣，在商店升級能力
- 打完所有波次即可獲勝

## 功能

- 玩家：移動、跳躍、朝滑鼠方向射擊（支援多重子彈散射）
- 多種敵人：5 種地面怪 + 3 種飛行怪，各有不同血量、速度和大小
- 敵人行為系統：跳躍、Buff 光環、治療，使用策略模式組合
- 飛行怪以 sin 波飄動，增加射擊難度
- 波次系統：從 txt 檔讀取關卡設定，支援 `SG*3` 語法和中途換 cooldown
- 金幣系統：敵人死亡掉落金幣，物理掉落動畫，15 秒後消失
- 商店系統：暫停時可花金幣升級 7 項能力（攻擊力、血量、攻速、多重子彈、移動速度等）
- 升級等級系統：每項升級有等級，價格隨等級遞增
- Buff 系統：藍色史萊姆光環（減傷 50%、增傷 200%、加速 10%）
- 治療系統：灰天使治療附近敵人（有冷卻時間）
- 碰撞偵測（子彈 vs 敵人、敵人 vs 城堡、敵人 vs 玩家、玩家 vs 金幣）
- 血條 HUD（城堡 HP、玩家 HP、擊殺數、波次進度、金幣數量）
- 遊戲狀態（開始、教學、遊玩中、暫停/商店、勝利、失敗、重新開始）
- 教學系統：多頁教學畫面，介紹操作和敵人
- 波次公告：每波開始前閃爍提示
- Debug 碰撞箱顯示（遊戲中按 H 切換）
- 回饋表單（結束畫面按 F 開啟）

## 敵人種類

### 地面怪（史萊姆）
| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 綠色史萊姆 | 5 | 普通 | 規律小跳 |
| 黑色史萊姆 | 30 | 慢 | 高血量坦克 |
| 紅色史萊姆 | 3 | 快 | 小而快速，會小跳 |
| 紫色史萊姆 | 10 | 普通 | 小跳 + 大跳交替 |
| 藍色史萊姆 | 10 | 普通 | 光環 Buff（減傷 + 增傷 + 加速） |

### 飛行怪
| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 灰天使 | 10 | 普通 | 上下飄動 + 治療附近敵人 |
| 小鳥 | 5 | 快 | 小型快速飄動 |
| 飛龍（Boss） | 50 | 慢 | 大型 Boss |

## 操作方式

- `A` / `D` — 左右移動
- `W` — 跳躍
- 滑鼠左鍵 — 朝游標方向射擊（支援多重子彈）
- `ESC` — 暫停 / 商店
- `1~7` — 暫停時購買升級
- `Enter` — 開始 / 繼續 / 重新開始
- `G` — 開始畫面進入教學
- `H` — 遊戲中切換碰撞箱顯示
- `F` — 結束畫面開啟回饋表單
- `Q` — 結束畫面離開遊戲

## 商店系統

暫停時可購買升級（價格隨等級遞增）：

| 按鍵 | 項目 | 效果 |
|------|------|------|
| 1 | 攻擊力 +1 | 子彈傷害提升 |
| 2 | 玩家血量 +10 | 最大血量提升並回滿 |
| 3 | 城堡血量 +20 | 城堡最大血量提升並回滿 |
| 4 | 金幣上限 +50 | 可攜帶更多金幣 |
| 5 | 攻速提升 | 射擊冷卻時間縮短 |
| 6 | 多重子彈 +1 | 一次射出更多子彈（散射） |
| 7 | 移動速度 +10 | 移動更快 |

## 波次系統

波次設定從 `resources/levels.txt` 讀取，支援進階語法：

```
cooldown 怪物*數量 新cooldown 怪物*數量 ...
```

範例：
```
1.5 SG*5
1.2 SG*3 0.8 SK*2
1.0 SK*3 0.5 SR*5 FA*2
0.3 SG*10 0.7 SK*2 1.0 FB*8 0.1 FD*1
```

代號對照：
| 代號 | 怪物 |
|------|------|
| SG | 綠色史萊姆 |
| SK | 黑色史萊姆 |
| SR | 紅色史萊姆 |
| SP | 紫色史萊姆 |
| SB | 藍色史萊姆 |
| FA | 灰天使 |
| FB | 小鳥 |
| FD | 飛龍 |

## 架構

```
main.cpp            → 進入點：g.init() → g.run() → g.close()
Game.h/.cpp         → 遊戲主迴圈、碰撞偵測、狀態管理、波次系統、商店
Player.h            → 玩家（移動、跳躍、攻擊）
Enemy.h             → 地面敵人基礎類別（含行為系統）
FlyingEnemy.h       → 飛行敵人（繼承 Enemy，sin 波飄動）
EnemyBehavior.h     → 敵人行為介面（抽象類別）
JumpBehavior.h      → 跳躍行為（小跳 / 小大跳交替）
BuffBehavior.h      → Buff 行為（減傷 + 增傷 + 加速光環）
HealBehavior.h      → 治療行為（範圍治療 + 冷卻）
Wave.h              → 波次資料類別（wave_data 結構）
Coin.h              → 金幣（物理掉落 + 自動消失）
Castle.h            → 城堡（被保護的目標）
Projectile.h        → 子彈（支援多重散射）
Character.h         → 角色基礎類別（Player、Enemy 繼承）
Building.h          → 建築基礎類別（Castle 繼承）
GameObject.h        → 所有物件的抽象基礎類別
Health.h            → 血量系統（組合模式）
GameFactory.h       → 工廠模式，統一建立所有遊戲物件
```

## OOP 概念使用

- **繼承**：GameObject → Character → Player / Enemy → FlyingEnemy
- **多型**：virtual update() 被各子類別 override；dynamic_cast 辨識行為類型
- **封裝**：private 成員 + public getter/setter + const 修飾
- **組合**：Character 擁有 Health；Enemy 擁有多個 Behavior
- **策略模式**：EnemyBehavior 介面 + JumpBehavior / BuffBehavior / HealBehavior 實作
- **工廠模式**：GameFactory 統一建立物件並組裝行為
- **狀態機**：game_statement 管理遊戲流程（START → TUTORIAL → PLAYING → PAUSE / WIN / LOSE）

## UML Class Diagram

- `UML/assignment5_UML.pdf` — Assignment 5 繳交時的架構
- `UML/TowerDefense_UML.pdf` — 目前整個遊戲的完整架構

## 編譯與執行

已經有 build 好的檔案，在 `build/TowerDefenseGame.exe`

```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```

## 聲明

所有圖片皆由 PixelLab AI 生成
