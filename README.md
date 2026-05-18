# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防遊戲。

## 遊戲說明

- 左側有一座城堡，需要保護它
- 中間是玩家角色，可以移動和射擊
- 右邊會依照波次生成敵人，往城堡方向前進
- 在敵人到達城堡之前擊敗他們！
- 打完所有波次即可獲勝

## 功能

- 玩家：移動、跳躍、朝滑鼠方向射擊（有冷卻時間）
- 多種敵人：5 種地面怪 + 3 種飛行怪，各有不同血量、速度和大小
- 敵人行為系統：跳躍、Buff 光環等，使用策略模式（Strategy Pattern）組合
- 飛行怪以 sin 波飄動，增加射擊難度
- 波次系統：從 txt 檔讀取關卡設定，可自由編輯
- 碰撞偵測（子彈 vs 敵人、敵人 vs 城堡、敵人 vs 玩家）
- Buff 系統：藍色史萊姆為附近敵人提供減傷 50% + 增傷 200%
- 血條 HUD（城堡 HP、玩家 HP、擊殺數、波次進度）
- 遊戲狀態（開始、遊玩中、暫停、勝利、失敗、重新開始）

## 敵人種類

### 地面怪（史萊姆）
| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 綠色史萊姆 | 5 | 普通 | 規律小跳 |
| 黑色史萊姆 | 30 | 慢 | 高血量坦克 |
| 紅色史萊姆 | 3 | 快 | 小而快速，會小跳 |
| 紫色史萊姆 | 10 | 普通 | 小跳 + 大跳交替 |
| 藍色史萊姆 | 10 | 普通 | 光環 Buff（附近敵人減傷 + 增傷） |

### 飛行怪
| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 灰天使 | 10 | 普通 | 上下飄動 |
| 小鳥 | 5 | 快 | 小型快速飄動 |
| 飛龍（Boss） | 50 | 慢 | 大型 Boss |

## 操作方式

- `A` / `D` — 左右移動
- `W` — 跳躍
- 滑鼠左鍵 — 朝游標方向射擊（有冷卻時間）
- `ESC` — 暫停
- `Enter` — 開始 / 繼續 / 重新開始
- `Q` — 結束畫面離開遊戲

## 波次系統

波次設定從 `resources/levels.txt` 讀取，格式如下：

```
cooldown 怪物1 怪物2 怪物3 ...
```

範例：
```
1.5 SG SG SG SG SG
1.2 SG SG SK SK SK
1.0 SG SK SK SR SR
0.8 SK SR SR FA FA
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
Game.h/.cpp         → 遊戲主迴圈、碰撞偵測、狀態管理、波次系統
Player.h/.cpp       → 玩家（移動、跳躍、攻擊）
Enemy.h             → 地面敵人基礎類別
FlyingEnemy.h       → 飛行敵人（繼承 Enemy，sin 波飄動）
EnemyBehavior.h     → 敵人行為
JumpBehavior.h      → 跳躍行為（小跳 / 小大跳交替）
BuffBehavior.h      → Buff 行為（減傷 + 增傷光環）
Wave.h              → 波次
Castle.h            → 城堡
Projectile.h        → 子彈
Character.h         → 角色基礎類別（Player、Enemy 繼承）
Building.h          → 建築基礎類別（Castle 繼承）
GameObject.h        → 所有物件的抽象基礎類別
Health.h            → 血量系統
GameFactory.h       → 工廠模式，統一建立所有遊戲物件
```

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
