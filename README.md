# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防遊戲。

## 遊戲說明

- 左側有一座城堡，需要保護它
- 中間是玩家角色，可以移動和射擊
- 右邊會生成敵人，往城堡方向前進
- 在敵人到達城堡之前擊敗他們！

## 功能

- 玩家：移動、跳躍、朝滑鼠方向射擊（有冷卻時間）
- 多種敵人：5 種地面怪 + 3 種飛行怪，各有不同血量、速度和大小
- 飛行怪以 sin 波飄動，增加射擊難度
- 碰撞偵測（子彈 vs 敵人、敵人 vs 城堡、敵人 vs 玩家）
- 血條 HUD（城堡 HP、玩家 HP、擊殺數）
- 遊戲狀態（開始、遊玩中、暫停、結束、重新開始）

## 敵人種類

### 地面怪（史萊姆）
| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 綠色史萊姆 | 5 | 普通 | 基本款 |
| 黑色史萊姆 | 30 | 慢 | 高血量坦克 |
| 紅色史萊姆 | 3 | 快 | 小而快速 |
| 紫色史萊姆 | 10 | 普通 | 會跳躍 |
| 藍色史萊姆 | 10 | 普通 | 為附近敵人提供 buff |

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

## 架構

```
main.cpp          → 進入點：g.init() → g.run() → g.close()
Game.h/.cpp       → 遊戲主迴圈、碰撞偵測、狀態管理
Player.h/.cpp     → 玩家（移動、跳躍、攻擊）
Enemy.h           → 地面敵人基礎類別
FlyingEnemy.h     → 飛行敵人（繼承 Enemy，sin 波飄動）
Castle.h          → 城堡（被保護的目標）
Projectile.h      → 子彈
Character.h       → 角色基礎類別（Player、Enemy 繼承）
Building.h        → 建築基礎類別（Castle 繼承）
GameObject.h      → 所有物件的抽象基礎類別
Health.h          → 血量系統（組合模式）
GameFactory.h     → 工廠模式，統一建立所有遊戲物件
```

## OOP 概念使用

- **繼承**：GameObject → Character → Player / Enemy → FlyingEnemy
- **多型**：virtual update() 被各子類別 override
- **封裝**：private 成員 + public getter/setter
- **組合**：Character 擁有 Health；Game 擁有所有物件
- **工廠模式**：GameFactory 統一建立物件
- **狀態機**：game_statement 管理遊戲流程

## UML Class Diagram

- `UML/assignment5_UML.pdf` — Assignment 5 繳交時的架構
- `UML/TowerDefense_UML.pdf` — 目前整個遊戲的完整架構

## 編譯與執行
已經有 build 好的檔案，在 \build\TowerDefenseGame.exe
```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```