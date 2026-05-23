# Tower Defense Game

使用 C++ 和 raylib 製作的 2D 塔防射擊遊戲。

## 遊戲說明

- 保護左邊的城堡不被怪物摧毀
- 玩家可以移動、跳躍、朝游標射擊
- 波次制：撐過所有波次即獲勝
- 擊殺敵人掉落金幣，升級能力和購買武器/藥水

## 操作方式

| 按鍵 | 功能 |
|------|------|
| A / D | 左右移動 |
| W | 跳躍 |
| 滑鼠左鍵 | 射擊 |
| ESC | 暫停 / 升級商店 |
| 1~7 | 購買升級 |
| Enter | 開始 / 繼續 |
| G | 教學 |
| H | Debug 碰撞箱 |
| F | 回饋表單 |

## 敵人（8 種）

| 名稱 | 血量 | 速度 | 特色 |
|------|------|------|------|
| 綠色史萊姆 | 70 | 快 | 小跳 |
| 黑色史萊姆 | 500 | 慢 | 高血量坦克 |
| 紅色史萊姆 | 40 | 極快 | 小跳 |
| 紫色史萊姆 | 200 | 中 | 小跳 + 大跳交替 |
| 藍色史萊姆 | 150 | 中 | 光環：增傷 + 加速 + 減傷 |
| 天使 | 170 | 中 | 飛行 + 範圍治療 |
| 小鳥 | 40 | 快 | 快速飛行 |
| 飛龍 | 1000 | 慢 | Boss 級飛行 |

## 武器（11 種）

| 武器 | 費用 | 效果 |
|------|------|------|
| 泥巴球 | 免費 | 基礎武器，微緩速 |
| 弓箭 | 150 | 高射速 |
| 石箭 | 150 | 高傷害 |
| 冰緩箭 | 100 | 60% 緩速 |
| 毒箭 | 100 | 疊加毒傷 |
| 穿透箭 | 150 | 穿透所有敵人 |
| 鐵球 | 100 | 3% 暴擊吃 90% 血量 |
| 火球 | 150 | 範圍爆炸 |
| 火箭 | 100 | 10% 暴擊 ×5 |
| 冰凍箭 | 100 | 冰凍 1 秒 |
| 飛彈 | 150 | 大範圍爆炸，暴擊 ×5，冷卻較長 |

## 藥水（7 種）

| 藥水 | 費用 | 效果 |
|------|------|------|
| 治療玩家 | 20 | 回滿玩家血量 |
| 治療城堡 | 20 | 回滿城堡血量 |
| 攻擊強化 | 30 | 傷害倍率提升（持續數波） |
| 攻速強化 | 30 | 攻擊速度提升（持續數波） |
| 護盾 | 40 | 減少受到傷害（持續數波） |
| 移速強化 | 20 | 移動速度提升（持續數波） |
| 再生 | 30 | 持續回血（持續數波） |

## 升級商店（ESC 開啟）

| 按鍵 | 項目 | 基礎費用 |
|------|------|---------|
| 1 | 攻擊力 +1 | 20 |
| 2 | 玩家血量 +30 | 20 |
| 3 | 城堡血量 +100 | 20 |
| 4 | 金幣上限 +50 | 50 |
| 5 | 攻擊速度 | 80 |
| 6 | 多重射擊 +1 | 150 |
| 7 | 移動速度 | 50 |

每項可多次升級，費用隨等級遞增。

## 波次系統

從 `resources/levels.txt` 讀取，可用 `resources/level_editor.html` 視覺化編輯。

語法：`cooldown 怪物代號*數量`

代號：SG(綠) SK(黑) SR(紅) SP(紫) SB(藍) FA(天使) FB(小鳥) FD(飛龍)

## 架構

```
main.cpp         → game g; g.init(); g.run(); g.close();
Game.h/.cpp      → 遊戲主邏輯、狀態管理、碰撞、商店、音樂、RenderTexture
GameFactory.h    → 工廠模式 + 所有遊戲數值 constexpr 定義
Player.h/.cpp    → 玩家（移動、跳躍、攻擊）
Enemy.h          → 地面敵人（含狀態效果：緩速/冰凍/中毒）
FlyingEnemy.h    → 飛行敵人（sin 波飄動）
EnemyBehavior.h  → 行為介面（策略模式）
JumpBehavior.h   → 跳躍行為
BuffBehavior.h   → 光環 Buff
HealBehavior.h   → 範圍治療
Projectile.h     → 子彈（穿透/緩速/冰凍/毒/範圍/暴擊）
DamageText.h     → 傷害數字飄出
Wave.h           → 波次資料
Coin.h           → 金幣
Goblin.h         → 哥布林商店 NPC
Castle.h / Building.h / Character.h / GameObject.h / Health.h → 基礎類別
```

## OOP 概念

- **繼承**：GameObject → Character → Player / Enemy → FlyingEnemy
- **多型**：virtual update()、virtual ~enemy()
- **組合**：Character has-a Health、Enemy has-a Behavior[]
- **策略模式**：EnemyBehavior + Jump / Buff / Heal
- **工廠模式**：GameFactory 統一建立物件
- **狀態機**：game_statement（START → TUTORIAL → PLAYING → PAUSE → WIN / LOSE）

## 資源結構

```
resources/
├── background/     — 背景圖片
├── object/         — 所有遊戲物件圖片（玩家、敵人、武器、藥水、城堡、金幣、哥布林）
├── monster/        — 怪物動畫幀（angel / bird / dragon）
├── music/
│   ├── BGM/        — 7 首 GameBoy 風格背景音樂
│   └── SoundEffect/
│       ├── get-coin/   — 撿金幣音效
│       └── wave-horn/  — 波次開始號角
├── other/          — 備用素材
├── levels.txt      — 關卡設定
└── level_editor.html — HTML 關卡編輯器
```

## 編譯與執行

```console
cmake -B build
cmake --build build
./build/TowerDefenseGame
```

## 聲明

- 圖片素材：PixelLab AI / Lovart AI 生成
- 背景音樂：[OtoLogic GB-Shooter Series](https://otologic.jp/free/bgm/game-shooter-gb01.html)（免費授權）
- 音效：Pixabay（免費授權）
